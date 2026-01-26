

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


namespace bpm
{
   namespace core
   {
      class TestNode : public ::testing::Test
      {
         public:

            TestNode() = default;

            void test()
            {
               BPM_SCOPED_TRACE_COUT("test");

               auto createNode = [&](const std::string& name,
                                     bool hasInputPort = false)
                                 {
                                    return bpm::core::Node(name,
                                                           hasInputPort,
                                                           maxLevel_,
                                                           circularDependency_);
                                 };

               auto node0 = createNode("node0");
               node0.addUpstreamNode(node0);
               node0.addDownstreamNode(node0);
               EXPECT_EQ(node0.name(), "node0");
               EXPECT_EQ(node0.level(), 0);
               EXPECT_FALSE(node0.hasInputPort());
               EXPECT_FALSE(node0.hasDownstreamNodes());
               EXPECT_FALSE(node0.hasUpstreamNodes());
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               auto node1 = createNode("node1",
                                       true);
               EXPECT_EQ(node1.name(), "node1");
               EXPECT_EQ(node1.level(), 0);
               EXPECT_TRUE(node1.hasInputPort());
               EXPECT_FALSE(node1.hasDownstreamNodes());
               EXPECT_FALSE(node1.hasUpstreamNodes());
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               auto node2 = createNode("node2");
               EXPECT_EQ(node2.name(), "node2");
               EXPECT_EQ(node2.level(), 0);
               EXPECT_FALSE(node2.hasInputPort());
               EXPECT_FALSE(node2.hasDownstreamNodes());
               EXPECT_FALSE(node2.hasUpstreamNodes());
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               // node0 -> node1
               node0.addUpstreamNode(node1);
               node1.addDownstreamNode(node0);
               // node0
               EXPECT_EQ(node0.level(), 0);
               EXPECT_FALSE(node0.hasDownstreamNodes());
               EXPECT_FALSE(node0.hasDownstreamNode(node1));
               EXPECT_FALSE(node0.hasDownstreamNode(node2));
               EXPECT_TRUE(node0.hasUpstreamNodes());
               EXPECT_TRUE(node0.hasUpstreamNode(node1));
               EXPECT_FALSE(node0.hasUpstreamNode(node2));
               // node1
               EXPECT_EQ(node1.level(), 1);
               EXPECT_TRUE(node1.hasDownstreamNodes());
               EXPECT_TRUE(node1.hasDownstreamNode(node0));
               EXPECT_FALSE(node1.hasDownstreamNode(node2));
               EXPECT_FALSE(node1.hasUpstreamNodes());
               EXPECT_FALSE(node1.hasUpstreamNode(node0));
               EXPECT_FALSE(node1.hasUpstreamNode(node2));
               // node2
               EXPECT_EQ(node2.level(), 0);
               EXPECT_FALSE(node2.hasDownstreamNodes());
               EXPECT_FALSE(node2.hasDownstreamNode(node0));
               EXPECT_FALSE(node2.hasDownstreamNode(node1));
               EXPECT_FALSE(node2.hasUpstreamNodes());
               EXPECT_FALSE(node2.hasUpstreamNode(node0));
               EXPECT_FALSE(node2.hasUpstreamNode(node1));
               // overall
               EXPECT_EQ(maxLevel_, 1);
               EXPECT_FALSE(circularDependency_);

               // node2 -> node0 -> node1
               node0.addDownstreamNode(node2);
               node2.addUpstreamNode(node0);
               // node0
               EXPECT_EQ(node0.level(), 1);
               EXPECT_TRUE(node0.hasDownstreamNodes());
               EXPECT_FALSE(node0.hasDownstreamNode(node1));
               EXPECT_TRUE(node0.hasDownstreamNode(node2));
               EXPECT_TRUE(node0.hasUpstreamNodes());
               EXPECT_TRUE(node0.hasUpstreamNode(node1));
               EXPECT_FALSE(node0.hasUpstreamNode(node2));
               // node1
               EXPECT_EQ(node1.level(), 2);
               EXPECT_TRUE(node1.hasDownstreamNodes());
               EXPECT_TRUE(node1.hasDownstreamNode(node0));
               EXPECT_FALSE(node1.hasDownstreamNode(node2));
               EXPECT_FALSE(node1.hasUpstreamNodes());
               EXPECT_FALSE(node1.hasUpstreamNode(node0));
               EXPECT_FALSE(node1.hasUpstreamNode(node2));
               // node2
               EXPECT_EQ(node2.level(), 0);
               EXPECT_FALSE(node2.hasDownstreamNodes());
               EXPECT_FALSE(node2.hasDownstreamNode(node0));
               EXPECT_FALSE(node2.hasDownstreamNode(node1));
               EXPECT_TRUE(node2.hasUpstreamNodes());
               EXPECT_TRUE(node2.hasUpstreamNode(node0));
               EXPECT_FALSE(node2.hasUpstreamNode(node1));
               // overall
               EXPECT_EQ(maxLevel_, 2);
               EXPECT_FALSE(circularDependency_);

               node0.updateLevel(node0.level() + 1);
               // node0
               EXPECT_EQ(node0.level(), 2);
               // node1
               EXPECT_EQ(node1.level(), 3);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // overall
               EXPECT_EQ(maxLevel_, 3);
               EXPECT_FALSE(circularDependency_);

               auto node3 = createNode("node3");
               EXPECT_FALSE(node3.setLevel(node3.level()));
               EXPECT_TRUE(node3.setLevel(maxLevel_ + 1));

               // node0
               EXPECT_EQ(node0.level(), 2);
               // node1
               EXPECT_EQ(node1.level(), 3);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 4);
               // overall
               EXPECT_EQ(maxLevel_, 4);
               EXPECT_FALSE(circularDependency_);

               // node3 --
               //        |
               // node2 ---> node0 -> node1
               node3.addUpstreamNode(node0);
               // node0
               EXPECT_EQ(node0.level(), 5);
               // node1
               EXPECT_EQ(node1.level(), 6);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 4);
               // overall
               EXPECT_EQ(maxLevel_, 6);
               EXPECT_FALSE(circularDependency_);

               //  --------------------------------
               //  |                              |
               //  --> node3 --                   |
               //             |                   |
               //      node2 ---> node0 -> node1 --
               node3.addDownstreamNode(node1);
               // node0
               EXPECT_EQ(node0.level(), 8);
               // node1
               EXPECT_EQ(node1.level(), 6);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 7);
               // overall
               EXPECT_EQ(maxLevel_, 8);
               EXPECT_TRUE(circularDependency_);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + node0.toString("   "));
                  BPM_TRACE_COUT("\n" + node1.toString("   "));
                  BPM_TRACE_COUT("\n" + node2.toString("   "));
                  BPM_TRACE_COUT("\n" + node3.toString("   "));
               }
            }

         private:

            std::size_t maxLevel_;
            bool circularDependency_;

      };


      TEST_F(TestNode, test)
      {
         test();
      }


#if 0
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
#endif
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
