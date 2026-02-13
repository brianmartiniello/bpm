

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/leveling_graph/NodeChain.hxx>
#include <bpm/core/Logger.hxx>


namespace bpm
{
   namespace core
   {
      class TestNodeChain : public ::testing::Test
      {
         public:

            TestNodeChain() = default;

            void test()
            {
               BPM_SCOPED_TRACE_COUT("test");
            }

         private:

      };


      TEST_F(TestNodeChain, test)
      {
         test();
      }
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
