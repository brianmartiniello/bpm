

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
                  BPM_TRACE_COUT("\n" + graph_.toString("   graph_."));
                  graph_.sortNodesByLevel();
                  auto set = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(set, "   nodesByLevel."));
               }

               // Clear the data
               clear();
            }

            void testReLevel()
            {
               BPM_SCOPED_TRACE_COUT("testReLevel");

               resetSharedVariables();

               EXPECT_TRUE(graph_.addNode("node0"));
               const auto node0Iter = graph_.graph_.find("node0");
               ASSERT_TRUE(node0Iter != graph_.graph_.end());
               const auto& node0 = node0Iter->second;

               EXPECT_TRUE(graph_.addNode("node1"));
               const auto node1Iter = graph_.graph_.find("node1");
               ASSERT_TRUE(node1Iter != graph_.graph_.end());
               const auto& node1 = node1Iter->second;

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
               const auto node2Iter = graph_.graph_.find("node2");
               ASSERT_TRUE(node2Iter != graph_.graph_.end());
               const auto& node2 = node2Iter->second;

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
               const auto node3Iter = graph_.graph_.find("node3");
               ASSERT_TRUE(node3Iter != graph_.graph_.end());
               const auto& node3 = node3Iter->second;

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
                  BPM_TRACE_COUT("\n" + graph_.toString("   graph_."));
                  graph_.sortNodesByLevel();
                  const auto vec = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(vec, "   nodesByLevel."));
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
               EXPECT_FALSE(graph_.circularDependency());

               auto vec = graph_.getNodesByLevel();
               EXPECT_TRUE(vec.empty());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   graph_."));
                  graph_.sortNodesByLevel();
                  vec = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(vec, "   nodesByLevel."));
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

            //   TAG  UP  DOWN
            // X A    0   0
            // X B1   0   1 * outHasMultiIn = true
            // X B2   0   1 * outHasMultiIn = false
            // X C    0   2
            // X D1   1   0 * inHasMultiOut = true
            // X D2   1   0 * inHasMultiOut = false
            // X E1   1   1 * inHasMultiOut = true, outHasMultiIn = true
            // X E2   1   1 * inHasMultiOut = true, outHasMultiIn = false
            // X E3   1   1 * inHasMultiOut = false, outHasMultiIn = true
            // X E4   1   1 * inHasMultiOut = false, outHasMultiIn = false
            // X F1   1   2 * inHasMultiOut = true
            // X F2   1   2 * inHasMultiOut = false
            // X G    2   0
            // X H1   2   1 * outHasMultiIn = true
            // X H2   2   1 * outHasMultiIn = false
            // X I    2   2

            //                                          node25B1--
            //                                                   |
            //                                                   ---->node26H1--
            //                                                     |           |
            //                                                     |           --->N27G
            //                                                     |            |
            //                                         -->node23F1--->node24E1---
            //                                         |
            //                                  N17B1---                         -->N22D1
            //                                         |    *******************  |
            //                             N16B1---    ---->N18H2->N19E4->N20F2---->N21D1
            //                                    |      |
            //                            N12C----->N13I--->N14E2->N15D2
            // *****************                |           ************
            // N03B2->N04E->N05E3--             ---->N11G
            //                    |               |
            //       N01B2->N02E3--->N06H2->N07F2--->N08E->09E2->N10D2
            //       ************    ************    *****************
            //
            // N00A

            void testConstructNodeChains()
            {
               BPM_SCOPED_TRACE_COUT("testConstructNodeChains");

               resetSharedVariables();

               const auto NUM_NODES = 28;
               for (auto i = 0U; i < NUM_NODES; ++i)
               {
                  ASSERT_TRUE(graph_.addNode("node" + std::to_string(i)));
               }

               EXPECT_TRUE(graph_.connectNodes("node2",
                                               "node1"));
               EXPECT_TRUE(graph_.connectNodes("node6",
                                               "node2"));
               EXPECT_TRUE(graph_.connectNodes("node4",
                                               "node3"));
               EXPECT_TRUE(graph_.connectNodes("node5",
                                               "node4"));
               EXPECT_TRUE(graph_.connectNodes("node6",
                                               "node5"));
               EXPECT_TRUE(graph_.connectNodes("node7",
                                               "node6"));
               EXPECT_TRUE(graph_.connectNodes("node8",
                                               "node7"));
               EXPECT_TRUE(graph_.connectNodes("node9",
                                               "node8"));
               EXPECT_TRUE(graph_.connectNodes("node10",
                                               "node9"));
               EXPECT_TRUE(graph_.connectNodes("node11",
                                               "node7"));
               EXPECT_TRUE(graph_.connectNodes("node11",
                                               "node12"));
               EXPECT_TRUE(graph_.connectNodes("node13",
                                               "node12"));
               EXPECT_TRUE(graph_.connectNodes("node14",
                                               "node13"));
               EXPECT_TRUE(graph_.connectNodes("node15",
                                               "node14"));
               EXPECT_TRUE(graph_.connectNodes("node13",
                                               "node16"));
               EXPECT_TRUE(graph_.connectNodes("node18",
                                               "node17"));
               EXPECT_TRUE(graph_.connectNodes("node18",
                                               "node13"));
               EXPECT_TRUE(graph_.connectNodes("node19",
                                               "node18"));
               EXPECT_TRUE(graph_.connectNodes("node20",
                                               "node19"));
               EXPECT_TRUE(graph_.connectNodes("node21",
                                               "node20"));
               EXPECT_TRUE(graph_.connectNodes("node22",
                                               "node20"));
               EXPECT_TRUE(graph_.connectNodes("node23",
                                               "node17"));
               EXPECT_TRUE(graph_.connectNodes("node24",
                                               "node23"));
               EXPECT_TRUE(graph_.connectNodes("node26",
                                               "node23"));
               EXPECT_TRUE(graph_.connectNodes("node26",
                                               "node25"));
               EXPECT_TRUE(graph_.connectNodes("node27",
                                               "node24"));
               EXPECT_TRUE(graph_.connectNodes("node27",
                                               "node26"));

               graph_.reLevel();
               EXPECT_FALSE(graph_.circularDependency());
               graph_.sortNodesByLevel();
               EXPECT_TRUE(graph_.constructNodeChains());
               EXPECT_TRUE(graph_.allNodesVisited());
               EXPECT_EQ(graph_.numNodes(), NUM_NODES);
               // EXPECT_EQ(graph_.numNodeChains(), NUM_NODES);
               EXPECT_EQ(graph_.maxNodeChainLength(), 3);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   graph_."));
                  {
                     const auto vec = graph_.getNodesByLevel();
                     BPM_TRACE_COUT("\n" + toString(vec, "   nodesByLevel."));
                  }
                  {
                     const auto vec = graph_.getNodeChainsByLevel();
                     BPM_TRACE_COUT("\n" + toString(vec, "   nodeChainsByLevel."));
                  }
               }

               // Expected node chains
               std::vector<NodeChainSimple> nodeChainSimpleVec;
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node12"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node0"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node1", "node2"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node3", "node4", "node5"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node16"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node17"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(7, {"node25"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(4, {"node13"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(4, {"node6", "node7"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(3, {"node18", "node19", "node20"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(2, {"node8", "node9", "node10"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(2, {"node23"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(1, {"node14", "node15"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(1, {"node24"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(1, {"node26"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(0, {"node11"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(0, {"node21"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(0, {"node27"}));
               nodeChainSimpleVec.push_back(NodeChainSimple(0, {"node22"}));

               // Actual node chains
               auto nodeChainsByLevelVec = graph_.getNodeChainsByLevel();

               // Loop over actual node chains
               for (const auto& nodeChainSimple : nodeChainSimpleVec)
               {
                  // Find the expected node chain in the list of actual node chains
                  const auto iter = std::find(nodeChainsByLevelVec.begin(),
                                              nodeChainsByLevelVec.end(),
                                              nodeChainSimple);
                  ASSERT_TRUE(iter != nodeChainsByLevelVec.end());

                  // Remove from the list of actual node chains
                  nodeChainsByLevelVec.erase(iter);
               }

               // List of actual node chains added should now be empty
               EXPECT_EQ(nodeChainsByLevelVec.size(), 0);
            }

            void testConstructNodeChainsCircular()
            {
               BPM_SCOPED_TRACE_COUT("testConstructNodeChainsCircular");

               resetSharedVariables();

               const auto NUM_NODES = 4;
               std::vector<std::string> nodeNames(NUM_NODES);
               for (auto i = 0U; i < NUM_NODES; ++i)
               {
                  nodeNames[i] = "node" + std::to_string(i);
                  ASSERT_TRUE(graph_.addNode(nodeNames[i]));
               }

               // --- node3 <-- node2 <--
               // |                     |
               // --> node0 --> node1 ---
               EXPECT_TRUE(graph_.connectNodes("node1",
                                               "node0"));
               EXPECT_TRUE(graph_.connectNodes("node2",
                                               "node1"));
               EXPECT_TRUE(graph_.connectNodes("node3",
                                               "node2"));
               EXPECT_TRUE(graph_.connectNodes("node0",
                                               "node3"));

               graph_.reLevel();
               EXPECT_TRUE(graph_.circularDependency());
               graph_.sortNodesByLevel();
               EXPECT_TRUE(graph_.constructNodeChains());
               EXPECT_TRUE(graph_.allNodesVisited());
               EXPECT_EQ(graph_.numNodes(), NUM_NODES);
               EXPECT_EQ(graph_.numNodeChains(), NUM_NODES);
               EXPECT_EQ(graph_.maxNodeChainLength(), 1);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   graph_."));
                  {
                     const auto vec = graph_.getNodesByLevel();
                     BPM_TRACE_COUT("\n" + toString(vec, "   nodesByLevel."));
                  }
                  {
                     const auto vec = graph_.getNodeChainsByLevel();
                     BPM_TRACE_COUT("\n" + toString(vec, "   nodeChainsByLevel."));
                  }
               }

               for (const auto& nodeChain : graph_.nodeChainsByLevel_)
               {
                  ASSERT_EQ(nodeChain.numNodes(), 1);
                  EXPECT_TRUE(nodeChain.isSingleNodeChain());

                  // Find the actual node name in the list of expected node namess added
                  const auto nodePtr = nodeChain.nodePtr();
                  const auto iter = std::find(nodeNames.begin(),
                                              nodeNames.end(),
                                              nodePtr->name());
                  ASSERT_TRUE(iter != nodeNames.end()) << nodePtr->name();

                  // Remove from the list of expected node namess added
                  nodeNames.erase(iter);
               }

               // List of expected node names added should now be empty
               EXPECT_EQ(nodeNames.size(), 0);
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


      TEST_F(TestGraph, testConstructNodeChainsCircular)
      {
         testConstructNodeChainsCircular();
      }
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
