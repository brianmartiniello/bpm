
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

            bool joinable();

            void stop();

         protected:

            Thread();

            ~Thread() = default;

            Thread(const Thread&) = delete;

            Thread& operator=(const Thread&) = delete;

            Thread(Thread&&) noexcept = delete;

            Thread& operator=(Thread&&) noexcept = delete;

            void start();

            bool keepRunning() const;

         private:

            void execute();

            std::condition_variable conditionVariable_;
            std::mutex mutex_;
            std::jthread thread_;
            bool running_;
            std::stop_token stopToken_;

      };
   }
}

#include <bpm/core/Thread.tmpl.hxx>

#endif
