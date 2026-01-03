

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Thread.hxx>

class MyThread : public bpm::core::Thread<MyThread>
{
   public:

      MyThread()
         : Thread<MyThread>()
         , count(0)
      {
         start();
      }

      std::uint64_t count;

   private:

      void threadFunction()
      {
         while (keepRunning())
         {
            ++count;
            const auto waitMs = 500U;
            BPM_SCOPED_TRACE_COUT("MyThread sleep, " + std::to_string(waitMs) + " ms");
            std::this_thread::sleep_for(std::chrono::milliseconds(waitMs));
         }
      }

      friend class bpm::core::Thread<MyThread>;
};

class TestThread : public ::testing::Test
{
   public:

      TestThread() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");

         {
            BPM_SCOPED_TRACE_COUT("thread");
            MyThread thread;
            EXPECT_TRUE(thread.joinable());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            EXPECT_TRUE(thread.joinable());
            EXPECT_GT(thread.count, 0);
         }

         {
            BPM_SCOPED_TRACE_COUT("thread stop");
            MyThread thread;
            EXPECT_TRUE(thread.joinable());
            std::this_thread::sleep_for(std::chrono::seconds(1));
            thread.stop();
            EXPECT_FALSE(thread.joinable());
            EXPECT_GT(thread.count, 0);
         }
      }

   private:

};


TEST_F(TestThread, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
