
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <vector>

namespace bpm
{
   namespace core
   {
      class TaskRunner
      {
         public:

            TaskRunner();

            void addTask(std::function<void()> task);

            double execute(std::size_t waitSeconds);

         private:

            void workerThread(std::size_t taskIndex);

            std::mutex mutex_;
            std::condition_variable conditionVar_;
            std::size_t threadCount_;
            std::atomic<bool> taskRun_;
            std::vector<std::function<void()>> tasks_;

      };
   }
}
