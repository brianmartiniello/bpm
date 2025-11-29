
#ifndef BPM_CORE_TIMER_HXX
#define BPM_CORE_TIMER_HXX

#include <chrono>

#define BPM_TRACE_COUT_ELAPSED(timer, hdr) \
   BPM_TRACE_COUT(hdr + std::string(": elapsed (") + std::to_string(timer.elapsed()) + ") sec")

namespace bpm
{
   namespace core
   {
      class Timer
      {
         public:

            using Clock = std::chrono::high_resolution_clock;
            using TimePoint = Clock::time_point;
            
            Timer();

            void end();

            // Return elapsed time in seconds as a double
            double elapsed();

         private:
    
            TimePoint start_;
            TimePoint end_;
            bool ended_;

      };
   }
}

#endif
