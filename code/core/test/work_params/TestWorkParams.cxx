

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>

struct Unrelated {};

///////////////////////////////////////////////
///////////////////////////////////////////////

class TestDerivedFrom : public ::testing::Test
{
   public:

      TestDerivedFrom() = default;

      void test()
      {
      }

   private:

};


TEST_F(TestDerivedFrom, test)
{
   test();
}


TEST_F(TestDerivedFrom, testT)
{
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
