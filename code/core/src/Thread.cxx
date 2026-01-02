

#include <bpm/core/Logger.hxx>
#include <bpm/core/Thread.hxx>


bpm::core::Thread::Thread()
   : thread_()
{
}


bpm::core::Thread::~Thread()
{
}


void bpm::core::Thread::start()
{
   // jthread passes a stop_token automatically if the function accepts it
   thread_ = std::jthread
   (
      [this]
      (std::stop_token stoken)
      {
         execute(stoken);
      }
   );
}


void bpm::core::Thread::stop()
{
   thread_.request_stop(); 

   // jthread joins automatically in its destructor or when reassigned.
}


void bpm::core::Thread::execute(std::stop_token stoken)
{
   BPM_SCOPED_TRACE_COUT("Execute");

   while (!stoken.stop_requested())
   {
      BPM_SCOPED_TRACE_COUT("Sleep");
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
   }
}
