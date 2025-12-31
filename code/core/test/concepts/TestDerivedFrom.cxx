

#include <concepts>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>


class TestDerivedFrom : public ::testing::Test
{
   public:

      TestDerivedFrom() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");
      }

   private:

};


TEST_F(TestDerivedFrom, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
