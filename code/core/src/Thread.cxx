

#include <bpm/core/Logger.hxx>
#include <bpm/core/Thread.hxx>


bpm::core::Thread::Thread()
   : conditionVariable_()
   , mutex_()
   , thread_()
{
   start();
}


bpm::core::Thread::~Thread()
{
   // jthread joins automatically in its destructor or when reassigned.
}


void bpm::core::Thread::start()
{
   BPM_SCOPED_TRACE_COUT("Start");

   std::unique_lock<std::mutex> lock(mutex_);

   // jthread passes a stop_token automatically if the function accepts it
   thread_ = std::jthread
   (
      [this]
      (std::stop_token stoken)
      {
         execute(stoken);
      }
   );

   conditionVariable_.wait(lock);
}


void bpm::core::Thread::stop()
{
   BPM_SCOPED_TRACE_COUT("Stop");

   thread_.request_stop();
   thread_.join();
}


bool bpm::core::Thread::joinable()
{
   return thread_.joinable();
}


void bpm::core::Thread::execute(std::stop_token stoken)
{
   BPM_SCOPED_TRACE_COUT("Execute");

   {
      std::lock_guard<std::mutex> lock(mutex_);
      conditionVariable_.notify_one();
   }

   while (!stoken.stop_requested())
   {
      BPM_SCOPED_TRACE_COUT("Sleep");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }
}
