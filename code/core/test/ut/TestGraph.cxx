

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
         BPM_SCOPED_TRACE_COUT("test");

         bpm::core::Graph graph;

         {
            BPM_SCOPED_TRACE_COUT("addNode");
            EXPECT_FALSE(graph.addNode(""));
            EXPECT_TRUE(graph.addNode("node_level_0"));
            EXPECT_FALSE(graph.addNode("node_level_0"));
            EXPECT_TRUE(graph.addNode("node_level_1"));
         }

         {
            BPM_SCOPED_TRACE_COUT("connectNodes");
            EXPECT_FALSE(graph.connectNodes("", "node_level_1"));
            EXPECT_FALSE(graph.connectNodes("node_level_0", ""));
            EXPECT_TRUE(graph.connectNodes("node_level_0", "node_level_1"));
         }

         {
            BPM_SCOPED_TRACE_COUT("toString");
            BPM_TRACE_COUT("\n" + graph.toString("   "));
         }
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
