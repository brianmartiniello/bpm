

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

#if 0 // Leveling disabled by default
            void testWithLevelUpdate()
            {
               BPM_SCOPED_TRACE_COUT("testWithLevelUpdate");

               const auto updateLevel = false;

               resetSharedVariables();

               auto node0 = createNode("node0");
               node0.addUpstreamNode(node0, updateLevel);
               node0.addDownstreamNode(node0, updateLevel);
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
               node0.addUpstreamNode(node1, updateLevel);
               node1.addDownstreamNode(node0, updateLevel);
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
               node0.addDownstreamNode(node2, updateLevel);
               node2.addUpstreamNode(node0, updateLevel);
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
               node3.addUpstreamNode(node0, updateLevel);
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
               node3.addDownstreamNode(node1, updateLevel);
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
#endif

            void testNoLevelUpdate()
            {
               BPM_SCOPED_TRACE_COUT("testNoLevelUpdate");

               resetSharedVariables();

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
               EXPECT_EQ(node1.level(), 0);
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
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               // node2 -> node0 -> node1
               node0.addDownstreamNode(node2);
               node2.addUpstreamNode(node0);
               // node0
               EXPECT_EQ(node0.level(), 0);
               EXPECT_TRUE(node0.hasDownstreamNodes());
               EXPECT_FALSE(node0.hasDownstreamNode(node1));
               EXPECT_TRUE(node0.hasDownstreamNode(node2));
               EXPECT_TRUE(node0.hasUpstreamNodes());
               EXPECT_TRUE(node0.hasUpstreamNode(node1));
               EXPECT_FALSE(node0.hasUpstreamNode(node2));
               // node1
               EXPECT_EQ(node1.level(), 0);
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
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               // node3 --
               //        |
               // node2 ---> node0 -> node1
               auto node3 = createNode("node3");
               node3.addUpstreamNode(node0);
               // node0
               EXPECT_EQ(node0.level(), 0);
               // node1
               EXPECT_EQ(node1.level(), 0);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 0);
               // overall
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               // Re-level from node2 which has no downstream
               // Without re-leveling using node 3
               EXPECT_FALSE(node2.hasDownstreamNodes());
               node2.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 1);
               // node1
               EXPECT_EQ(node1.level(), 2);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 0);
               // overall
               EXPECT_EQ(maxLevel_, 2);
               EXPECT_FALSE(circularDependency_);

               // Re-level from node3 which has no downstream
               // Already re-leveled using node 2
               EXPECT_FALSE(node3.hasDownstreamNodes());
               node3.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 1);
               // node1
               EXPECT_EQ(node1.level(), 2);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 0);
               // overall
               EXPECT_EQ(maxLevel_, 2);
               EXPECT_FALSE(circularDependency_);

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

               // Re-level from node3 which has no downstream
               // Without re-leveling using node 2
               EXPECT_FALSE(node3.hasDownstreamNodes());
               // Distinguish node3 from node2 by level
               node3.level_ = 10;
               node3.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 11);
               // node1
               EXPECT_EQ(node1.level(), 12);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 10);
               // overall
               EXPECT_EQ(maxLevel_, 12);
               EXPECT_FALSE(circularDependency_);

               // Re-level from node2 which has no downstream
               // Already re-leveled using node 3
               EXPECT_FALSE(node2.hasDownstreamNodes());
               node2.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 11);
               // node1
               EXPECT_EQ(node1.level(), 12);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 10);
               // overall
               EXPECT_EQ(maxLevel_, 12);
               EXPECT_FALSE(circularDependency_);

               // Clear the data
               clear();

               //  --------------------------------
               //  |                              |
               //  --> node3 --                   |
               //             |                   |
               //      node2 ---> node0 -> node1 --
               node3.addDownstreamNode(node1);
               // node0
               EXPECT_EQ(node0.level(), 0);
               // node1
               EXPECT_EQ(node1.level(), 0);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 0);
               // overall
               EXPECT_EQ(maxLevel_, 0);
               EXPECT_FALSE(circularDependency_);

               // Re-level from node2 which has no downstream
               EXPECT_FALSE(node2.hasDownstreamNodes());
               node2.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 4);
               // node1
               EXPECT_EQ(node1.level(), 2);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 3);
               // overall
               EXPECT_EQ(maxLevel_, 4);
               EXPECT_TRUE(circularDependency_);

               // Re-level from node3 which is now has a circular dependency
               // Already re-leveled using node 2
               node3.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 4);
               // node1
               EXPECT_EQ(node1.level(), 2);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 3);
               // overall
               EXPECT_EQ(maxLevel_, 4);
               EXPECT_TRUE(circularDependency_);

               // Clear the data
               clear();

               // Re-level from node3 which has no downstream
               // Without re-leveling using node 2
               // Distinguish node3 from node2 by level
               node3.level_ = 10;
               node3.reLevel();
               // node0
               EXPECT_EQ(node0.level(), 14);
               // node1
               EXPECT_EQ(node1.level(), 12);
               // node2
               EXPECT_EQ(node2.level(), 0);
               // node3
               EXPECT_EQ(node3.level(), 13);
               // overall
               EXPECT_EQ(maxLevel_, 14);
               EXPECT_TRUE(circularDependency_);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + node0.toString("   "));
                  BPM_TRACE_COUT("\n" + node1.toString("   "));
                  BPM_TRACE_COUT("\n" + node2.toString("   "));
                  BPM_TRACE_COUT("\n" + node3.toString("   "));
               }

               // Clear the data
               clear();
            }

         private:

            bpm::core::Node createNode(const std::string& name,
                                       bool hasInputPort = false)
            {
               return bpm::core::Node(name,
                                      hasInputPort,
                                      globalLevelPhase_,
                                      maxLevel_,
                                      circularDependency_);
            };

            void resetSharedVariables()
            {
               globalLevelPhase_ = 0;
               maxLevel_ = 0;
               circularDependency_ = false;
            }

            std::size_t globalLevelPhase_;
            std::size_t maxLevel_;
            bool circularDependency_;

      };


#if 0 // Leveling disabled by default
      TEST_F(TestNode, testWithLevelUpdate)
      {
         testWithLevelUpdate();
      }
#endif


      TEST_F(TestNode, testNoLevelUpdate)
      {
         testNoLevelUpdate();
      }


      class TestGraph : public ::testing::Test
      {
         public:

            TestGraph() = default;

            void testNoLevelUpdate()
            {
               BPM_SCOPED_TRACE_COUT("testNoLevelUpdate");

               resetSharedVariables();

               graph_.addNode("node0");
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

               graph_.addNode("node1");
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

               graph_.addNode("node2");
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

               graph_.addNode("node3");
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

            void testAssignMaxLevel()
            {
               BPM_SCOPED_TRACE_COUT("testAssignMaxLevel");

               resetSharedVariables();

               graph_.addNode("node0");
               auto node0Iter = graph_.graph_.find("node0");
               ASSERT_TRUE(node0Iter != graph_.graph_.end());
               auto& node0 = node0Iter->second;

               graph_.addNode("node1");
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

               graph_.addNode("node2");
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

               graph_.addNode("node3",
                              true); // hasInputPort
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

            void testNodeChains()
            {
               BPM_SCOPED_TRACE_COUT("testNodeChains");

               resetSharedVariables();

               graph_.addNode("node0");
               graph_.addNode("node1");
               graph_.addNode("node2");
               graph_.addNode("node3");
               graph_.addNode("node4");
               graph_.addNode("node5");
               graph_.addNode("node6");
               graph_.addNode("node7");
               graph_.addNode("node8");

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

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + graph_.toString("   "));
                  graph_.sortNodesByLevel();
                  auto set = graph_.getNodesByLevel();
                  BPM_TRACE_COUT("\n" + toString(set, "   "));
               }
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


      TEST_F(TestGraph, testAssignMaxLevel)
      {
         testAssignMaxLevel();
      }


      TEST_F(TestGraph, testNodeChains)
      {
         testNodeChains();
      }
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
