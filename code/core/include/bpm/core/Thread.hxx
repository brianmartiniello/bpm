
#ifndef BPM_CORE_THREAD_HXX
#define BPM_CORE_THREAD_HXX

#include <condition_variable>
#include <mutex>
#include <thread>

namespace bpm
{
   namespace core
   {
      template<class Derived>
      class Thread
      {
         public:

            Thread();

            ~Thread() = default;

            bool joinable();

            void stop();

         protected:

            Thread(const Thread&) = delete;

            Thread& operator=(const Thread&) = delete;

            Thread(Thread&&) = delete;

            Thread& operator=(Thread&&) = delete;

         private:
     
            void start();

            void execute(std::stop_token stoken);

            std::condition_variable conditionVariable_;
            std::mutex mutex_;
            std::jthread thread_;

      };
   }
}

#include <bpm/core/Thread.tmpl.hxx>

#endif
