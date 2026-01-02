
#ifndef BPM_CORE_THREAD_HXX
#define BPM_CORE_THREAD_HXX

#include <condition_variable>
#include <mutex>
#include <thread>

namespace bpm
{
   namespace core
   {
      class Thread
      {
         public:

            Thread();

            ~Thread();

            bool joinable();

            void stop();

         private:
     
            void start();

            void execute(std::stop_token stoken);

            std::condition_variable conditionVariable_;
            std::mutex mutex_;
            std::jthread thread_;

      };
   }
}

#endif
