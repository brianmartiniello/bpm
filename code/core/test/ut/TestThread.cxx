

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Thread.hxx>


class TestThread : public ::testing::Test
{
   public:

      TestThread() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");

         {
            BPM_SCOPED_TRACE_COUT("thread");
            bpm::core::Thread thread;
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
