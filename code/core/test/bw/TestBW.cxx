
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Timer.hxx>

class ThreadGroupRunner
{
   public:

      ThreadGroupRunner()
         : mutex_()
         , conditionVar_()
         , threadCount_(0)
         , taskRun_(false)
         , tasks_()
      {
      }

      void addTask(std::function<void()> task)
      {
         std::lock_guard<std::mutex> lock(mutex_);

         tasks_.push_back(std::move(task));
      }

      double execute(std::size_t waitSeconds)
      {
         std::unique_lock<std::mutex> lock(mutex_);

         std::vector<std::thread> threads;

         {
            BPM_SCOPED_TRACE_COUT("Starting threads");

            if (0 == tasks_.size())
            {
               return 0.0;
            }

            // Start threads
            for (auto taskIndex = 0; taskIndex < tasks_.size(); ++taskIndex)
            {
               threads.emplace_back([this, taskIndex]
                                    ()
                                    {
                                       this->workerThread(taskIndex);
                                    });
            }

            // Wait for threads to start
            auto waitSuccess = conditionVar_.wait_for(lock,
                                                      std::chrono::seconds(waitSeconds),
                                                      [&]
                                                      ()
                                                      {
                                                         return threadCount_ >= threads.size();
                                                      });
            if (false == waitSuccess)
            {
               const auto error = "Failed to wait for (" + std::to_string(waitSeconds) +
                                  ") seconds - threads.size() (" + std::to_string(threads.size()) +
                                  "), threadCount_ (" + std::to_string(threadCount_) + ")";
               BPM_ERROR_COUT(error);
               throw(error);
            }
         }

         BPM_SCOPED_TRACE_COUT("Run tasks");

         // Start timer
         bpm::core::Timer timer;

         // Run the tasks
         taskRun_.store(true, std::memory_order_relaxed);

         // Wait for threads to complete
         for (auto& thread : threads)
         {
            thread.join();
         }

         // End timer
         timer.end();

         // Reset the parameters
         threadCount_ = 0;
         taskRun_.store(false, std::memory_order_relaxed);
         tasks_.clear();

         // Return the elapsed time
         return timer.elapsed();
      }

   private:

      void workerThread(std::size_t taskIndex)
      {
         // Point to task
         auto& task = tasks_[taskIndex];

         // Indicate start
         {
            std::lock_guard<std::mutex> guard(mutex_);
            BPM_TRACE_COUT("WORKER - taskIndex (" + std::to_string(taskIndex) +
                           "), threadCount_ (" + std::to_string(threadCount_) + ")");
            ++threadCount_;
            conditionVar_.notify_all();
         }

         // Wait to run
         while (false == taskRun_.load(std::memory_order_relaxed)) {}

         // Run task
         task();
      }

      std::mutex mutex_;
      std::condition_variable conditionVar_;
      std::size_t threadCount_;
      std::atomic<bool> taskRun_;
      std::vector<std::function<void()>> tasks_;

};

void firstTouchWork(double* data,
                    std::size_t start,
                    std::size_t end)
{
   // BPM_SCOPED_TRACE_COUT("start (" + std::to_string(start) +
   //                       "), end (" + std::to_string(end) + ")");

   for (auto i = start; i < end; ++i)
   {
      data[i] = 1.0;
   }
}

void benchmarkWork(double* data,
                   std::size_t start,
                   std::size_t end,
                   bool computeHeavy)
{
   // BPM_SCOPED_TRACE_COUT("start (" + std::to_string(start) +
   //                       "), end (" + std::to_string(end) +
   //                       "), computeHeavy (" + std::to_string(computeHeavy) + ")");

   if (true == computeHeavy)
   {
      for (auto i = start; i < end; ++i)
      {
         auto val = data[i];
         for (auto j = 0; j < 100; ++j)
         {
            val = std::sqrt(std::sin(val) + 2.0);
         }
         data[i] = val;
      }
   }
   else
   {
      for (auto i = start; i < end; ++i)
      {
         data[i] = data[i] * 0.5 + 0.2;
      }
   }
}

int main(int argc,
         char** argv)
{
   BPM_SCOPED_TRACE_COUT("main");

   try
   {
      BPM_SCOPED_TRACE_COUT("try");

      if (argc < 4)
      {
         BPM_ERROR_COUT("Usage: " + std::string(argv[0]) + " <threads> <bytes> <mode>");
         return EXIT_FAILURE;
      }

      // Input
      const std::size_t numThreads = std::stoi(argv[1]);
      const std::size_t numBytesTotal = std::stoull(argv[2]);
      const std::string mode = std::string(argv[3]);

      BPM_TRACE_COUT("Input - numThreads (" + std::to_string(numThreads) +
                     "), numBytesTotal (" + std::to_string(numBytesTotal) +
                     "), mode (" + mode + ")");

      // Check input validity
      if ((numThreads == 0) ||
          (numBytesTotal == 0))
      {
         BPM_ERROR_COUT("numThreads (" + std::to_string(numThreads) +
                        ") or numBytesTotal (" + std::to_string(numBytesTotal) +
                        ") is 0");
         return EXIT_FAILURE;
      }

      // Derived
      const auto numBytesPerThread = (numBytesTotal / numThreads) +
                                     ((numBytesTotal % numThreads) > 0);
      const auto numElemsPerThread = (numBytesPerThread / sizeof(double)) +
                                     ((numBytesPerThread % sizeof(double)) > 0);
      const auto numElemsTotal = numElemsPerThread * numThreads;
      const auto computeHeavy = (mode == "compute");

      BPM_TRACE_COUT("Derived - numBytesPerThread (" + std::to_string(numBytesPerThread) +
                     "), numElemsPerThread (" + std::to_string(numElemsPerThread) +
                     "), numElemsTotal (" + std::to_string(numElemsTotal) +
                     "), computeHeavy (" + std::to_string(computeHeavy) + ")");

      std::vector<double> data(numElemsTotal);
      ThreadGroupRunner runner;
      const auto WAIT_SECONDS = 3U;

      for (auto phase = 0; phase < 2; ++phase)
      {
         for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex)
         {
            const auto start = threadIndex * numElemsPerThread;
            const auto end = start + numElemsPerThread;
            runner.addTask([&, threadIndex, start, end]
                           ()
                           {
                              // --- PHASE 1: FIRST TOUCH INITIALIZATION ---
                              if (0 == phase)
                              {
                                 firstTouchWork(data.data(),
                                                start,
                                                end);
                              }
                              // --- PHASE 2: BENCHMARK ---
                              else // (1 == phase)
                              {
                                 benchmarkWork(data.data(),
                                               start,
                                               end,
                                               computeHeavy);
                              }
                           });
         }
         std::cout << runner.execute(WAIT_SECONDS) << std::endl;
      }
   }
   catch (const std::exception& e)
   {
      BPM_ERROR_COUT(std::string("Caught a standard exceptions: ") + e.what());
      return EXIT_FAILURE;
   }
   catch (const std::string& e)
   {
      BPM_ERROR_COUT("Caught a string exception: " + e);
      return EXIT_FAILURE;
   }
   catch (...)
   {
      BPM_ERROR_COUT("Caught an unknown exception");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}