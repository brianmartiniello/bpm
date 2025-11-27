

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


class TestGraph : public ::testing::Test
{
   public:

      TestGraph() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("");

         bpm::core::Graph graph;

         EXPECT_FALSE(graph.addNode(""));
         EXPECT_TRUE(graph.addNode("node_level_0"));
         EXPECT_FALSE(graph.addNode("node_level_0"));

         BPM_TRACE_COUT("\n" + graph.toString("   "));
      }

   private:

};


TEST_F(TestGraph, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
