

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/SharedCountVector.hxx>
#include <bpm/core/Timer.hxx>


class TestSharedCountVector : public ::testing::Test
{
   public:

      TestSharedCountVector() = default;

      void test()
      {
      }

   private:

};


TEST_F(TestSharedCountVector, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
