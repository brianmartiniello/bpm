

#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <bpm/core/Timer.hxx>
#include <bpm/core/Logger.hxx>

#include <omp.h>


class TestOpenMP : public ::testing::Test
{
   public:

      TestOpenMP() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");

         BPM_TRACE_COUT("Using " + std::to_string(omp_get_max_threads()) + " threads");

         const auto NUM_LOOPS = 10;
         const auto N = 1;  // Work doesn't matter — we measure region overhead only.

         // --- Measure first-call (cold start) ---
         {
            BPM_SCOPED_TRACE_COUT("Cold start");

            bpm::core::Timer timer;

            #pragma omp parallel for
            for (auto i = 0; i < N; i++)
            {
            }

            BPM_TRACE_COUT_ELAPSED(timer, "Cold start (first parallel region)")
         }

         {
            BPM_SCOPED_TRACE_COUT("Hot loops");

            // --- Measure hot-team regions ---
            std::vector<double> hotTimes;
            hotTimes.reserve(NUM_LOOPS);

            for (auto k = 0; k < NUM_LOOPS; k++)
            {
               bpm::core::Timer timer;

               #pragma omp parallel for
               for (auto i = 0; i < N; i++)
               {
               }

               hotTimes.push_back(timer.elapsed());
            }

            BPM_TRACE_COUT("Hot-team parallel region costs:");
            for (auto k = 0; k < NUM_LOOPS; k++)
            {
               BPM_TRACE_COUT("  hotTimes[" + std::to_string(k) + "] (" + std::to_string(hotTimes[k]) + ") sec");
            }

            // --- Compute average hot time ---
            double hotTimesSum = 0;
            for (auto hotTime : hotTimes)
            {
               hotTimesSum += hotTime;
            }
            
            auto hotTimesAvg = hotTimesSum / NUM_LOOPS;
            BPM_TRACE_COUT("Average hot-team cost: " + std::to_string(hotTimesAvg) + " sec");
         }
      }

   private:

};


TEST_F(TestOpenMP, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
