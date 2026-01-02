

#include <bpm/core/Logger.hxx>


template<class Derived>
bpm::core::Thread<Derived>::Thread()
   : conditionVariable_()
   , mutex_()
   , thread_()
{
   start();
}


// jthread joins automatically in its destructor or when reassigned
// template<class Derived>
// bpm::core::Thread<Derived>::~Thread()
// {
// }


template<class Derived>
void bpm::core::Thread<Derived>::start()
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


template<class Derived>
void bpm::core::Thread<Derived>::stop()
{
   BPM_SCOPED_TRACE_COUT("Stop");

   thread_.request_stop();
   thread_.join();
}


template<class Derived>
bool bpm::core::Thread<Derived>::joinable()
{
   return thread_.joinable();
}


template<class Derived>
void bpm::core::Thread<Derived>::execute(std::stop_token stoken)
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
