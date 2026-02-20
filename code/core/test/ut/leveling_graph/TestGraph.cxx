

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/leveling_graph/Graph.hxx>
#include <bpm/core/Logger.hxx>


namespace bpm
{
   namespace core
   {
      class TestGraph : public ::testing::Test
      {
         public:

            TestGraph() = default;

            void testNoLevelUpdate()
            {
               BPM_SCOPED_TRACE_COUT("testNoLevelUpdate");

               resetSharedVariables();

               EXPECT_FALSE(graph_.addNode(""));

               EXPECT_TRUE(graph_.addNode("node0"));
               EXPECT_FALSE(graph_.addNode("node0"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node0
               auto node0Iter = graph_.graph_.find("node0");
               ASSERT_TRUE(node0Iter != graph_.graph_.end());
               auto& node0 = node0Iter->second;
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node0) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node0) !=
                           graph_.nodesWithoughDownstream_.end());

               EXPECT_TRUE(graph_.addNode("node1"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node1
               auto node1Iter = graph_.graph_.find("node1");
               ASSERT_TRUE(node1Iter != graph_.graph_.end());
               auto& node1 = node1Iter->second;
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node1) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node1) !=
                           graph_.nodesWithoughDownstream_.end());

               EXPECT_TRUE(graph_.addNode("node2"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node2
               auto node2Iter = graph_.graph_.find("node2");
               ASSERT_TRUE(node2Iter != graph_.graph_.end());
               auto& node2 = node2Iter->second;
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node2) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node2) !=
                           graph_.nodesWithoughDownstream_.end());

               // node0 -> undefined
               EXPECT_FALSE(graph_.connectNodes("node0",
                                                "undefined"));
               // undefined -> node1
               EXPECT_FALSE(graph_.connectNodes("undefined",
                                                "node1"));

               // node0 -> node1
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node1"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node0
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node0) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node0) !=
                           graph_.nodesWithoughDownstream_.end());
               // node1
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node1) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_FALSE(graph_.nodesWithoughDownstream_.find(&node1) !=
                            graph_.nodesWithoughDownstream_.end());
               // node2
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node2) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node2) !=
                           graph_.nodesWithoughDownstream_.end());

               // node2 -> node0 -> node1
               EXPECT_TRUE(graph_.connectNodes("node2",
                                               "node0"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node0
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node0) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_FALSE(graph_.nodesWithoughDownstream_.find(&node0) !=
                            graph_.nodesWithoughDownstream_.end());
               // node1
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node1) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_FALSE(graph_.nodesWithoughDownstream_.find(&node1) !=
                            graph_.nodesWithoughDownstream_.end());
               // node2
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node2) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node2) !=
                           graph_.nodesWithoughDownstream_.end());

               EXPECT_TRUE(graph_.addNode("node3"));
               auto node3Iter = graph_.graph_.find("node3");
               ASSERT_TRUE(node3Iter != graph_.graph_.end());
               auto& node3 = node3Iter->second;
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node3) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node3) !=
                           graph_.nodesWithoughDownstream_.end());

               // node3 --
               //        |
               // node2 ---> node0 -> node1
               EXPECT_TRUE(graph_.connectNodes("node3",
                                               "node0"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               // node0
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node0) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_FALSE(graph_.nodesWithoughDownstream_.find(&node0) !=
                            graph_.nodesWithoughDownstream_.end());
               // node1
               EXPECT_TRUE(graph_.nodesWithoughUpstream_.find(&node1) !=
                           graph_.nodesWithoughUpstream_.end());
               EXPECT_FALSE(graph_.nodesWithoughDownstream_.find(&node1) !=
                            graph_.nodesWithoughDownstream_.end());
               // node2
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node2) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node2) !=
                           graph_.nodesWithoughDownstream_.end());
               // node3
               EXPECT_FALSE(graph_.nodesWithoughUpstream_.find(&node3) !=
                            graph_.nodesWithoughUpstream_.end());
               EXPECT_TRUE(graph_.nodesWithoughDownstream_.find(&node3) !=
                           graph_.nodesWithoughDownstream_.end());

               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 0);
               EXPECT_EQ(node2.level(), 0);
               EXPECT_EQ(node3.level(), 0);

               // Re-level from node2 and node3 which has no downstream
               EXPECT_FALSE(node2.hasDownstreamNodes());
               EXPECT_FALSE(node3.hasDownstreamNodes());
               graph_.reLevel();
               EXPECT_EQ(graph_.maxLevel_, 2);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 2);
               EXPECT_EQ(node0.level(), 1);
               EXPECT_EQ(node1.level(), 2);
               EXPECT_EQ(node2.level(), 0);
               EXPECT_EQ(node3.level(), 0);

               auto clear = [&]()
                            {
                               node0.level_ = 0;
                               node0.levelPhase_ = 0;
                               node1.level_ = 0;
                               node1.levelPhase_ = 0;
                               node2.level_ = 0;
                               node2.levelPhase_ = 0;
                               node3.level_ = 0;
                               node3.levelPhase_ = 0;
                               resetSharedVariables();
                            };

               // Clear the data
               clear();

               //  --------------------------------
               //  |                              |
               //  --> node3 --                   |
               //             |                   |
               //      node2 ---> node0 -> node1 --
               EXPECT_TRUE(graph_.connectNodes("node1",
                                               "node3"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);


               // Re-level from node2 which has no downstream
               EXPECT_FALSE(node2.hasDownstreamNodes());
               EXPECT_TRUE(node3.hasDownstreamNodes());
               graph_.reLevel();
               EXPECT_EQ(graph_.maxLevel_, 4);
               EXPECT_TRUE(graph_.circularDependency_);
               EXPECT_TRUE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 1);
               EXPECT_EQ(node0.level(), 4);
               EXPECT_EQ(node1.level(), 2);
               EXPECT_EQ(node2.level(), 0);
               EXPECT_EQ(node3.level(), 3);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   "));
                  graph_.sortNodesByLevel();
                  auto set = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(set, "   "));
               }

               // Clear the data
               clear();
            }

            void testReLevel()
            {
               BPM_SCOPED_TRACE_COUT("testReLevel");

               resetSharedVariables();

               EXPECT_TRUE(graph_.addNode("node0"));
               auto node0Iter = graph_.graph_.find("node0");
               ASSERT_TRUE(node0Iter != graph_.graph_.end());
               auto& node0 = node0Iter->second;

               EXPECT_TRUE(graph_.addNode("node1"));
               auto node1Iter = graph_.graph_.find("node1");
               ASSERT_TRUE(node1Iter != graph_.graph_.end());
               auto& node1 = node1Iter->second;

               // node0 -> node1
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node1"));
               EXPECT_EQ(graph_.maxLevel_, 0);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 0);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 0);

               graph_.reLevel();
               EXPECT_EQ(graph_.maxLevel_, 1);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 1);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 1);

               EXPECT_TRUE(graph_.addNode("node2"));
               auto node2Iter = graph_.graph_.find("node2");
               ASSERT_TRUE(node2Iter != graph_.graph_.end());
               auto& node2 = node2Iter->second;

               //        --> node2
               //        |
               // node0 ---> node1
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node2"));
               EXPECT_EQ(graph_.maxLevel_, 1);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 1);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 1);
               EXPECT_EQ(node2.level(), 0);

               graph_.reLevel();
               EXPECT_EQ(graph_.maxLevel_, 1);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 2);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 1);
               EXPECT_EQ(node2.level(), 1);

               EXPECT_TRUE(graph_.addNode("node3",
                                          true)); // hasInputPort
               auto node3Iter = graph_.graph_.find("node3");
               ASSERT_TRUE(node3Iter != graph_.graph_.end());
               auto& node3 = node3Iter->second;

               //        --> node3 (w/ input)
               //        |
               //        -----------> node2
               //        |
               // node0 ------------> node1
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node3"));
               EXPECT_EQ(graph_.maxLevel_, 1);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 2);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 1);
               EXPECT_EQ(node2.level(), 1);
               EXPECT_EQ(node3.level(), 0);

               graph_.reLevel();
               EXPECT_EQ(graph_.maxLevel_, 2);
               EXPECT_FALSE(graph_.circularDependency_);
               EXPECT_FALSE(graph_.circularDependency());
               EXPECT_EQ(graph_.globalLevelPhase_, 5);
               EXPECT_EQ(node0.level(), 0);
               EXPECT_EQ(node1.level(), 2);
               EXPECT_EQ(node2.level(), 2);
               EXPECT_EQ(node3.level(), 1);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   "));
                  graph_.sortNodesByLevel();
                  auto set = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(set, "   "));
               }
            }

            void testGetNodesByLevel()
            {
               BPM_SCOPED_TRACE_COUT("testGetNodesByLevel");

               resetSharedVariables();

               EXPECT_TRUE(graph_.addNode("node0"));
               EXPECT_TRUE(graph_.addNode("node1"));
               EXPECT_TRUE(graph_.addNode("node2"));
               EXPECT_TRUE(graph_.addNode("node3"));
               EXPECT_TRUE(graph_.addNode("node4"));
               EXPECT_TRUE(graph_.addNode("node5"));
               EXPECT_TRUE(graph_.addNode("node6"));
               EXPECT_TRUE(graph_.addNode("node7"));
               EXPECT_TRUE(graph_.addNode("node8"));

               // node0 --> node1 --            --> node5 --> node6
               //                   |           |
               // node2 --> node3 ----> node4 ----> node7 --> node8
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node1"));
               EXPECT_TRUE(graph_.connectNodes("node2",
                                               "node3"));
               EXPECT_TRUE(graph_.connectNodes("node1",
                                               "node4"));
               EXPECT_TRUE(graph_.connectNodes("node3",
                                               "node4"));
               EXPECT_TRUE(graph_.connectNodes("node4",
                                               "node5"));
               EXPECT_TRUE(graph_.connectNodes("node4",
                                               "node7"));
               EXPECT_TRUE(graph_.connectNodes("node5",
                                               "node6"));
               EXPECT_TRUE(graph_.connectNodes("node7",
                                               "node8"));

               graph_.reLevel();

               auto vec = graph_.getNodesByLevel();
               EXPECT_TRUE(vec.empty());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   "));
                  graph_.sortNodesByLevel();
                  vec = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(vec, "   "));
               }

               ASSERT_EQ(vec.size(), 9);
               EXPECT_EQ(vec[0]->level(), 4);
               EXPECT_TRUE((vec[0]->name() == "node8") ||
                           (vec[0]->name() == "node6"));
               EXPECT_EQ(vec[1]->level(), 4);
               EXPECT_TRUE((vec[1]->name() == "node8") ||
                           (vec[1]->name() == "node6"));
               EXPECT_EQ(vec[2]->level(), 3);
               EXPECT_TRUE((vec[2]->name() == "node7") ||
                           (vec[2]->name() == "node5"));
               EXPECT_EQ(vec[3]->level(), 3);
               EXPECT_TRUE((vec[3]->name() == "node7") ||
                           (vec[3]->name() == "node5"));
               EXPECT_EQ(vec[4]->level(), 2);
               EXPECT_EQ(vec[4]->name(), "node4");
               EXPECT_EQ(vec[5]->level(), 1);
               EXPECT_TRUE((vec[5]->name() == "node3") ||
                           (vec[5]->name() == "node1"));
               EXPECT_EQ(vec[6]->level(), 1);
               EXPECT_TRUE((vec[6]->name() == "node3") ||
                           (vec[6]->name() == "node1"));
               EXPECT_EQ(vec[7]->level(), 0);
               EXPECT_TRUE((vec[7]->name() == "node2") ||
                           (vec[7]->name() == "node0"));
               EXPECT_EQ(vec[8]->level(), 0);
               EXPECT_TRUE((vec[8]->name() == "node2") ||
                           (vec[8]->name() == "node0"));
            }

            // TAG  UP  DOWN
            // A    0   0
            // B    0   1
            // C    0   2
            // D    1   0
            // E    1   1
            // F    1   2
            // G    2   0
            // H    2   1
            // I    2   2

            //                               N17B---                     -->N22D
            //                                     |    **************** |
            //                          N16B---    ---->N18H->N19E->N20F--->N21D
            //                                |      |
            //                        N12C----->N13I--->N14E->N15D
            // ****************             |           **********
            // N03B->N04E->N05E--           ---->N11G
            //                  |             |
            //       N01B->N02E--->N06H->N07F--->N08E->09E->N10D
            //       **********    **********    ***************
            // N00A

            void testConstructNodeChains()
            {
               BPM_SCOPED_TRACE_COUT("testConstructNodeChains");

               resetSharedVariables();
            }

         private:

            void resetSharedVariables()
            {
               graph_.globalLevelPhase_ = 0;
               graph_.maxLevel_ = 0;
               graph_.circularDependency_ = false;
            }

            Graph graph_;

      };


      TEST_F(TestGraph, testNoLevelUpdate)
      {
         testNoLevelUpdate();
      }


      TEST_F(TestGraph, testReLevel)
      {
         testReLevel();
      }


      TEST_F(TestGraph, testGetNodesByLevel)
      {
         testGetNodesByLevel();
      }


      TEST_F(TestGraph, testConstructNodeChains)
      {
         testConstructNodeChains();
      }
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
