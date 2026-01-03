

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Thread.hxx>

class MyThread : public bpm::core::Thread<MyThread>
{
   public:

      MyThread() = default;

   private:

      void threadFunction(std::stop_token /* stopToken */)
      {
         BPM_SCOPED_TRACE_COUT("MyThread Sleep");
         std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
         }

         {
            BPM_SCOPED_TRACE_COUT("thread stop");
            MyThread thread;
            EXPECT_TRUE(thread.joinable());
            thread.stop();
            EXPECT_FALSE(thread.joinable());
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
