

#include <bpm/core/Timer.hxx>


bpm::core::Timer::Timer()
   : start_(Clock::now())
   , end_()
   , ended_(false)
{
}


void bpm::core::Timer::end()
{
   end_ = Clock::now();
   ended_ = true;
}


double bpm::core::Timer::elapsed()
{
   if (false == ended_)
   {
      end();
   }
   std::chrono::duration<double> diff = end_ - start_;
   return diff.count();
}
