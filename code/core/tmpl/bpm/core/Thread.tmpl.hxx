

#include <bpm/core/Logger.hxx>


template<class Derived>
bpm::core::Thread<Derived>::Thread()
   : conditionVariable_()
   , mutex_()
   , thread_()
   , running_(false)
{
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

   if (running_)
   {
      BPM_ERROR_COUT("Already running");
      return;
   }

   // jthread passes a stop_token automatically if the function accepts it
   thread_ = std::jthread
   (
      [this]
      (std::stop_token stopToken)
      {
         execute(stopToken);
      }
   );

   conditionVariable_.wait(lock,
                           [this]{ return running_; });
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
void bpm::core::Thread<Derived>::execute(std::stop_token stopToken)
{
   BPM_SCOPED_TRACE_COUT("Execute");

   {
      std::lock_guard<std::mutex> lock(mutex_);
      running_ = true;
      conditionVariable_.notify_all();
   }

   auto& derived = static_cast<Derived&>(*this);
   while (!stopToken.stop_requested())
   {
      BPM_SCOPED_TRACE_COUT("threadFunction");
      derived.threadFunction(stopToken);
   }
}
