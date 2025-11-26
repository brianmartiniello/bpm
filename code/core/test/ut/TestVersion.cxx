

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/Version.hxx>


class TestVersion : public ::testing::Test
{
   public:

      TestVersion()
      {
      }

      ~TestVersion()
      {
      }

      void test()
      {
         BPM_TRACE_COUT(bpm::core::version());
      }

   private:

};


TEST_F(TestVersion, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
