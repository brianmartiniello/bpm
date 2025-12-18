
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Timer.hxx>

struct ThreadGroupRunner
{
   public:

      ThreadGroupRunner()
         : threadMutex_()
         , threadConditionVar_()
         , threadCount_(0)
         , threadRun_(false)
         , threads_()
      {
      }

      std::vector<std::thread>& threads()
      {
         return threads_;
      }

      void reset()
      {
         std::lock_guard<std::mutex> guard(threadMutex_);
         threadCount_ = 0;
         threadRun_.store(false, std::memory_order_relaxed);
         for (auto& t : threads_)
         {
            t.join();
         }
         threads_.clear();
      }

      void workerStart(std::size_t threadIndex)
      {
         // Indicate start
         {
            std::lock_guard<std::mutex> guard(threadMutex_);
            BPM_TRACE_COUT("WORKER - threadIndex (" + std::to_string(threadIndex) +
                           "), threadCount_ (" + std::to_string(threadCount_) + ")");
            ++threadCount_;
            threadConditionVar_.notify_all();
         }

         // Wait to run
         while (false == threadRun_.load(std::memory_order_relaxed)) {}
      }

      double executeWorkers(std::size_t waitSeconds)
      {
         // Wait for threads to start
         {
            std::unique_lock<std::mutex> lock(threadMutex_);
            auto waitSuccess = threadConditionVar_.wait_for(lock,
                                                            std::chrono::seconds(waitSeconds),
                                                            [&]
                                                            {
                                                               return threadCount_ >= threads_.size();
                                                            });
            if (false == waitSuccess)
            {
               const auto error = "Failed to wait for (" + std::to_string(waitSeconds) +
                                  ") seconds - threads_.size() (" + std::to_string(threads_.size()) +
                                  "), threadCount_ (" + std::to_string(threadCount_) + ")";
               BPM_ERROR_COUT(error);
               throw(error);
            }
         }

         // Start timer
         bpm::core::Timer timer;

         // Run the threads
         threadRun_.store(true, std::memory_order_relaxed);

         // Wait for threads to complete
         for (auto& thread : threads_)
         {
            thread.join();
         }

         // End timer
         timer.end();

         // Reset the parameters
         reset();

         // Return the elapsed time
         return timer.elapsed();
      }

   private:

      std::mutex threadMutex_;
      std::condition_variable threadConditionVar_;
      std::size_t threadCount_;
      std::atomic<bool> threadRun_;
      std::vector<std::thread> threads_;

};

void firstTouchWork(double* data,
                    std::size_t start,
                    std::size_t end)
{
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
   if (argc < 4)
   {
      BPM_ERROR_COUT("Usage: " + std::string(argv[0]) + " <threads> <bytes> <mode>");
      return EXIT_FAILURE;
   }

   // Input
   std::size_t numThreads = 0;
   std::size_t numBytesTotal = 0;
   std::string mode;
   try
   {
      numThreads = std::stoi(argv[1]);
      numBytesTotal = std::stoull(argv[2]);
      mode = std::string(argv[3]);
   }
   catch (const std::exception& e)
   {
      BPM_ERROR_COUT(std::string("Caught an exception handling inputs: ") + e.what());
      return EXIT_FAILURE;
   }
   catch (...)
   {
      BPM_ERROR_COUT("Caught an unknown exception handling inputs");
      return EXIT_FAILURE;
   }

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

   try
   {
      // --- PHASE 1: FIRST TOUCH INITIALIZATION ---
      for (auto i = 0; i < numThreads; ++i)
      {
         const auto start = i * numElemsPerThread;
         const auto end = start + numElemsPerThread;
         runner.threads().emplace_back([&, i, start, end]()
                                       {
                                          runner.workerStart(i);
                                          firstTouchWork(data.data(),
                                                         start,
                                                         end);
                                       });
      }
      runner.executeWorkers(WAIT_SECONDS);

      // --- PHASE 2: BENCHMARK ---
      for (auto i = 0; i < numThreads; ++i)
      {
         const auto start = i * numElemsPerThread;
         const auto end = start + numElemsPerThread;
         runner.threads().emplace_back([&, i, start, end]()
                                       {
                                          runner.workerStart(i);
                                          benchmarkWork(data.data(),
                                                        start,
                                                        end,
                                                        computeHeavy);
                                       });
      }
      std::cout << runner.executeWorkers(WAIT_SECONDS) << std::endl;
   }
   catch (...)
   {
      BPM_ERROR_COUT("Caught an unknown exception executing workers");
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}