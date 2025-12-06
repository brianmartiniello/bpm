

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

      template<class SharedCountVectorType>
      void test(const std::vector<std::size_t>& threadCountIndices)
      {
         BPM_SCOPED_TRACE_COUT("test - threadCountIndices.size() = " + std::to_string(threadCountIndices.size()));

         std::mutex threadMutex;
         std::condition_variable threadConditionVar;
         auto threadCount = 0;
         std::atomic<bool> threadRun(false);
         std::vector<std::thread> threads;

         std::vector<double> durations(threadCountIndices.size());

         SharedCountVectorType sharedCountVector(threadCountIndices.size());

         const auto NUM_ITERATIONS = 1000000;

         auto thread =
            [&](std::size_t threadIndex, std::size_t countIndex)
            {

               auto& duration = durations[threadIndex];

               {
                  std::lock_guard<std::mutex> guard(threadMutex);
                  BPM_TRACE_COUT("WORKER - threadIndex (" + std::to_string(threadIndex) +
                                 "), countIndex (" + std::to_string(countIndex) + 
                                 "), threadCount (" + std::to_string(threadCount) + ")");
                  ++threadCount;
                  threadConditionVar.notify_all();
               }

               while (false == threadRun.load(std::memory_order_relaxed)) {}

               bpm::core::Timer timer;

               for (auto iiteration = 0; iiteration < NUM_ITERATIONS; ++iiteration)
               {
                  sharedCountVector.increment(countIndex);
               }

               duration = timer.elapsed();
            };

         std::vector<int64_t> expectedCounts(threadCountIndices.size());
         for (auto threadIndex = 0; threadIndex < threadCountIndices.size(); ++threadIndex)
         {
            const auto countIndex = threadCountIndices[threadIndex];
            // BPM_TRACE_COUT("MAIN - threadIndex (" + std::to_string(threadIndex) +
            //                "), countIndex (" + std::to_string(countIndex) + ")");
            ASSERT_TRUE(countIndex < threadCountIndices.size()) << "threadIndex = " << threadIndex;
            expectedCounts[countIndex] += NUM_ITERATIONS;
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

         for (auto countIndex = 0; countIndex < expectedCounts.size(); ++countIndex)
         {
            EXPECT_EQ(sharedCountVector.get(countIndex), expectedCounts[countIndex]) << "countIndex = " << countIndex;
         }

         BPM_TRACE_COUT("durations.size() = " + std::to_string(durations.size()));
         double durationsSum = 0;
         for (auto durationsIndex = 0; durationsIndex < durations.size(); ++durationsIndex)
         {
            const auto duration = durations[durationsIndex];
            BPM_TRACE_COUT("   durations[" + std::to_string(durationsIndex) + "] (" + std::to_string(duration) + ") sec");
            durationsSum += duration;
         }
         
         auto durationsAvg = durationsSum / durations.size();
         BPM_TRACE_COUT("Average duration: " + std::to_string(durationsAvg) + " sec");
      }

      void testTypes(const std::vector<std::size_t>& threadCountIndices)
      {
         BPM_SCOPED_TRACE_COUT("testTypes");

         {
            BPM_SCOPED_TRACE_COUT("Atomic");
            test<bpm::core::SharedCountVector>(threadCountIndices);
         }

         {
            BPM_SCOPED_TRACE_COUT("Lock");
            test<bpm::core::SharedCountVectorLock>(threadCountIndices);
         }
      }

      void test1Thread1Index()
      {
         BPM_SCOPED_TRACE_COUT("test1Thread1Index");
         const std::vector<std::size_t> threadCountIndices = {0};
         testTypes(threadCountIndices);
      }

      void test2Threads1Index()
      {
         BPM_SCOPED_TRACE_COUT("test2Threads1Index");
         const std::vector<std::size_t> threadCountIndices = {0, 0};
         testTypes(threadCountIndices);
      }

      void test2Threads2Indices()
      {
         BPM_SCOPED_TRACE_COUT("test2Threads2Indices");
         const std::vector<std::size_t> threadCountIndices = {0, 1};
         testTypes(threadCountIndices);
      }

      void test4Threads1Index()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads1Index");
         const std::vector<std::size_t> threadCountIndices = {0, 0, 0, 0};
         testTypes(threadCountIndices);
      }

      void test4Threads2Indices()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads2Indices");
         const std::vector<std::size_t> threadCountIndices = {0, 1, 0, 1};
         testTypes(threadCountIndices);
      }

      void test4Threads4Indices()
      {
         BPM_SCOPED_TRACE_COUT("test4Threads4Indices");
         const std::vector<std::size_t> threadCountIndices = {0, 1, 2, 3};
         testTypes(threadCountIndices);
      }

   private:

};


TEST_F(TestSharedCountVector, test1Thread1Index)
{
   test1Thread1Index();
}


TEST_F(TestSharedCountVector, test2Threads1Index)
{
   test2Threads1Index();
}


TEST_F(TestSharedCountVector, test2Threads2Indices)
{
   test2Threads2Indices();
}


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
