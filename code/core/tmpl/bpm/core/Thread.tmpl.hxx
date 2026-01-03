

#include <bpm/core/Logger.hxx>


template<class Derived>
bpm::core::Thread<Derived>::Thread()
   : conditionVariable_()
   , mutex_()
   , thread_()
   , running_(false)
   , stopToken_()
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
   BPM_SCOPED_TRACE_COUT("");

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
         stopToken_ = stopToken;
         execute();
      }
   );

   conditionVariable_.wait(lock,
                           [this]{ return running_; });
}


template<class Derived>
bool bpm::core::Thread<Derived>::keepRunning() const
{
   // BPM_TRACE_COUT("stopToken_.stop_requested() = " +
   //                std::to_string(stopToken_.stop_requested()));

   // Add stop conditions here
   bool stop = stopToken_.stop_requested();

   return !stop;
}


template<class Derived>
void bpm::core::Thread<Derived>::stop()
{
   BPM_SCOPED_TRACE_COUT("");

   thread_.request_stop();
   thread_.join();
}


template<class Derived>
bool bpm::core::Thread<Derived>::joinable()
{
   return thread_.joinable();
}


template<class Derived>
void bpm::core::Thread<Derived>::execute()
{
   BPM_SCOPED_TRACE_COUT("");

   {
      std::lock_guard<std::mutex> lock(mutex_);
      running_ = true;
      conditionVariable_.notify_all();
   }

   auto& derived = static_cast<Derived&>(*this);
   while (keepRunning())
   {
      BPM_SCOPED_TRACE_COUT("threadFunction");
      derived.threadFunction();
   }
}
