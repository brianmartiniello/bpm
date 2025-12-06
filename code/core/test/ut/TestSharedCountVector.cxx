

#include <cstdint>
#include <iostream>
#include <thread>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/SharedCountVector.hxx>
#include <bpm/core/Timer.hxx>


class TestSharedCountVector : public ::testing::Test
{
   public:

      TestSharedCountVector() = default;

      void test(const std::vector<std::size_t>& threadCountIndices)
      {
         BPM_SCOPED_TRACE_COUT("test - threadCountIndices.size() = " + std::to_string(threadCountIndices.size()));

         std::mutex threadMutex;
         std::condition_variable threadConditionVar;
         auto threadCount = 0;
         std::atomic<bool> threadRun(false);
         std::vector<std::thread> threads;

         std::vector<double> durations(threadCountIndices.size());

         bpm::core::SharedCountVector sharedCountVector(threadCountIndices.size());

         auto thread =
            [&](std::size_t threadIndex, std::size_t countIndex)
            {
               BPM_TRACE_COUT("WORKER - threadIndex (" + std::to_string(threadIndex) +
                              "), countIndex (" + std::to_string(countIndex) + ")");

               auto& duration = durations[threadIndex];

               {
                  std::lock_guard<std::mutex> guard(threadMutex);
                  ++threadCount;
                  threadConditionVar.notify_all();
               }

               while (false == threadRun.load(std::memory_order_relaxed)) {}

               bpm::core::Timer timer;

               for (auto i = 0; i < 1000000; ++i)
               {
                  sharedCountVector.increment(countIndex);
               }

               duration = timer.elapsed();
            };

         for (auto threadIndex = 0; threadIndex < threadCountIndices.size(); ++threadIndex)
         {
            const auto countIndex = threadCountIndices[threadIndex];
            // BPM_TRACE_COUT("MAIN - threadIndex (" + std::to_string(threadIndex) +
            //                "), countIndex (" + std::to_string(countIndex) + ")");
            ASSERT_TRUE(countIndex < threadCountIndices.size()) << "threadIndex = " << threadIndex;
            threads.emplace_back(thread,
                                   threadIndex,
                                   threadCountIndices[threadIndex]);
         }

         {
            std::unique_lock<std::mutex> lock(threadMutex);
            const auto WAIT_SECONDS = 3;
            auto waitSuccess = threadConditionVar.wait_for(lock,
                                                           std::chrono::seconds(WAIT_SECONDS),
                                                           [&]
                                                           {
                                                              return threadCount >= threads.size();
                                                           });
            ASSERT_TRUE(waitSuccess);
         }

         threadRun.store(true, std::memory_order_relaxed);

         for (auto& thread : threads)
         {
            thread.join();
         }

         BPM_TRACE_COUT("durations.size() = " + std::to_string(durations.size()));
         for (auto k = 0; k < durations.size(); k++)
         {
            BPM_TRACE_COUT("  durations[" + std::to_string(k) + "] (" + std::to_string(durations[k]) + ") sec");
         }

         double durationsSum = 0;
         for (auto duration : durations)
         {
            durationsSum += duration;
         }
         
         auto durationsAvg = durationsSum / durations.size();
         BPM_TRACE_COUT("Average duration: " + std::to_string(durationsAvg) + " sec");
      }

      void test4Threads1Index()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads1Index");
         const std::vector<std::size_t> threadCountIndices = {0, 0, 0, 0};
         test(threadCountIndices);
      }

      void test4Threads2Indices()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads2Indices");
         const std::vector<std::size_t> threadCountIndices = {0, 1, 0, 1};
         test(threadCountIndices);
      }

      void test4Threads4Indices()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads4Indices");
         const std::vector<std::size_t> threadCountIndices = {0, 1, 2, 3};
         test(threadCountIndices);
      }

   private:

};


TEST_F(TestSharedCountVector, test4Threads1Index)
{
   test4Threads1Index();
}


TEST_F(TestSharedCountVector, test4Threads2Indices)
{
   test4Threads2Indices();
}


TEST_F(TestSharedCountVector, test4Threads4Indices)
{
   test4Threads4Indices();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
