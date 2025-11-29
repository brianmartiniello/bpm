

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>


class TestOpenMP : public ::testing::Test
{
   public:

      TestOpenMP() = default;

      void test()
      {
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
