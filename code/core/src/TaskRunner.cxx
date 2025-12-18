

#include <string>
#include <thread>

#include <bpm/core/Logger.hxx>
#include <bpm/core/TaskRunner.hxx>
#include <bpm/core/Timer.hxx>


bpm::core::TaskRunner::TaskRunner()
   : mutex_()
   , conditionVar_()
   , threadCount_(0)
   , taskRun_(false)
   , tasks_()
{
}


void bpm::core::TaskRunner::addTask(std::function<void()> task)
{
   std::lock_guard<std::mutex> lock(mutex_);

   tasks_.push_back(std::move(task));
}


double bpm::core::TaskRunner::execute(std::size_t waitSeconds)
{
   std::unique_lock<std::mutex> lock(mutex_);

   if (0 == tasks_.size())
   {
      BPM_ERROR_COUT("Task vector is empty");
      return -1.0;
   }

   std::vector<std::thread> threads;

   {
      BPM_SCOPED_TRACE_COUT("Starting threads");

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


void bpm::core::TaskRunner::workerThread(std::size_t taskIndex)
{
   // BPM_SCOPED_TRACE_COUT("WORKER - taskIndex (" + std::to_string(taskIndex) + ")");

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
   while (false == taskRun_.load(std::memory_order_relaxed))
   {
      std::this_thread::yield();
   }

   // Run task
   task();
}
