

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

            void testSingleNodeChain()
            {
               BPM_SCOPED_TRACE_COUT("testSingleNodeChain");

               //   CASE - Num Upstream - Num Downstream - Expected
               // X 0      0              0                true
               // X 1A     0              1                true * outHasMultiIn = true
               // X 1B     0              1                false * outHasMultiIn = false
               // X 2      0              2                true
               // X 3A     1              0                true * inHasMultiOut = true
               // X 3B     1              0                false * inHasMultiOut = false
               // X 4A     1              1                true * outHasMultiIn = true
               // X 4B     1              1                false * outHasMultiIn = false
               // X 5A     1              2                true * inHasMultiOut = true
               // X 5B     1              2                false * inHasMultiOut = false
               // X 6      2              0                true
               // X 7A     2              1                true * outHasMultiIn = true
               // X 7B     2              1                false * outHasMultiIn = false
               // X 8      2              2                true

               {
                  auto node0 = createNode("node0");

                  BPM_TRACE_COUT("CASE 0");
                  EXPECT_EQ(node0.numUpstreamNodes(), 0);
                  EXPECT_EQ(node0.numDownstreamNodes(), 0);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");
                  // node2 ---> node1 ---> node0
                  node0.addUpstreamNode(node1);
                  node1.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 1B");
                  EXPECT_EQ(node2.numUpstreamNodes(), 0);
                  EXPECT_EQ(node2.numDownstreamNodes(), 1);
                  EXPECT_EQ(node1.numUpstreamNodes(), 1);
                  EXPECT_FALSE(NodeChain::singeNodeChain(node2));

                  BPM_TRACE_COUT("CASE 4B");
                  EXPECT_EQ(node1.numUpstreamNodes(), 1);
                  EXPECT_EQ(node1.numDownstreamNodes(), 1);
                  EXPECT_FALSE(NodeChain::singeNodeChain(node1));

                  BPM_TRACE_COUT("CASE 3B");
                  EXPECT_EQ(node0.numUpstreamNodes(), 1);
                  EXPECT_EQ(node0.numDownstreamNodes(), 0);
                  EXPECT_EQ(node2.numDownstreamNodes(), 1);
                  EXPECT_FALSE(NodeChain::singeNodeChain(node0));

                  auto node3 = createNode("node3");
                  // node3 --
                  //        |
                  // node2 ---> node1 ---> node0
                  node1.addUpstreamNode(node3);

                  BPM_TRACE_COUT("CASE 1A");
                  EXPECT_EQ(node2.numUpstreamNodes(), 0);
                  EXPECT_EQ(node2.numDownstreamNodes(), 1);
                  EXPECT_EQ(node1.numUpstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node2));

                  auto node4 = createNode("node4");
                  // node3 --          --> node4
                  //        |          |
                  // node2 ---> node1 ---> node0
                  node4.addUpstreamNode(node1);

                  BPM_TRACE_COUT("CASE 3A");
                  EXPECT_EQ(node0.numUpstreamNodes(), 1);
                  EXPECT_EQ(node0.numDownstreamNodes(), 0);
                  EXPECT_EQ(node1.numDownstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");
                  //        --> node1
                  //        |
                  // node0 ---> node2
                  node1.addUpstreamNode(node0);
                  node2.addUpstreamNode(node0);

                  BPM_TRACE_COUT("CASE 2");
                  EXPECT_EQ(node0.numUpstreamNodes(), 0);
                  EXPECT_EQ(node0.numDownstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));

                  auto node3 = createNode("node3");
                  //                  --> node1
                  //                  |
                  // node3 --> node0 ---> node2
                  node0.addUpstreamNode(node3);

                  BPM_TRACE_COUT("CASE 5B");
                  EXPECT_EQ(node0.numUpstreamNodes(), 1);
                  EXPECT_EQ(node0.numDownstreamNodes(), 2);
                  EXPECT_EQ(node3.numDownstreamNodes(), 1);
                  EXPECT_FALSE(NodeChain::singeNodeChain(node0));

                  auto node4 = createNode("node4");
                  //        --> node4  --> node1
                  //        |          |
                  // node3 ---> node0 ---> node2
                  node4.addUpstreamNode(node3);

                  BPM_TRACE_COUT("CASE 5A");
                  EXPECT_EQ(node0.numUpstreamNodes(), 1);
                  EXPECT_EQ(node0.numDownstreamNodes(), 2);
                  EXPECT_EQ(node3.numDownstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");
                  // node1 -- 
                  //        |
                  // node0 ---> node2
                  node2.addUpstreamNode(node0);
                  node2.addUpstreamNode(node1);

                  BPM_TRACE_COUT("CASE 6");
                  EXPECT_EQ(node2.numUpstreamNodes(), 2);
                  EXPECT_EQ(node2.numDownstreamNodes(), 0);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node2));

                  auto node3 = createNode("node3");
                  // node1 -- 
                  //        |
                  // node0 ---> node2 --> node3
                  node3.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 7B");
                  EXPECT_EQ(node2.numUpstreamNodes(), 2);
                  EXPECT_EQ(node2.numDownstreamNodes(), 1);
                  EXPECT_EQ(node3.numUpstreamNodes(), 1);
                  EXPECT_FALSE(NodeChain::singeNodeChain(node2));

                  auto node4 = createNode("node4");
                  // node1 --          --> node4
                  //        |          |
                  // node0 ---> node2 ---> node3
                  node4.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 8");
                  EXPECT_EQ(node2.numUpstreamNodes(), 2);
                  EXPECT_EQ(node2.numDownstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node2));

                  {
                     // Remove node4 from node2 downstream
                     const auto iter = node2.downstreamNodes_.find(&node4);
                     ASSERT_TRUE(iter != node2.downstreamNodes_.end());
                     node2.downstreamNodes_.erase(iter);
                  }
                  {
                     // Remove node2 from node4 upstream
                     const auto iter = node4.upstreamNodes_.find(&node2);
                     ASSERT_TRUE(iter != node4.upstreamNodes_.end());
                     node4.upstreamNodes_.erase(iter);
                  }

                  // node1 --   node4 -- 
                  //        |          |
                  // node0 ---> node2 ---> node3
                  node3.addUpstreamNode(node4);

                  BPM_TRACE_COUT("CASE 7A");
                  EXPECT_EQ(node2.numUpstreamNodes(), 2);
                  EXPECT_EQ(node2.numDownstreamNodes(), 1);
                  EXPECT_EQ(node3.numUpstreamNodes(), 2);
                  EXPECT_TRUE(NodeChain::singeNodeChain(node2));
               }
            }

            void testSingleNodeChainNoUpAndDown()
            {
               BPM_SCOPED_TRACE_COUT("testSingleNodeChainNoUpAndDown");

               auto node0 = createNode("node0");
               auto node1 = createNode("node1");

               bpm::core::NodeChain nodeChain;

               // Verifying empty node chain
               EXPECT_TRUE(nodeChain.verifyContinuity());

               // Sorting empty node chain resets level
               nodeChain.level_ = 10;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               // Single node chain, no upstream and downstream nodes
               EXPECT_TRUE(nodeChain.addNode(node0));
               EXPECT_TRUE(nodeChain.singleNodeChain());
               EXPECT_TRUE(nodeChain.verifyContinuity());

               // Duplicate
               EXPECT_FALSE(nodeChain.addNode(node0));

               // Cannot add to a single node chain
               EXPECT_FALSE(nodeChain.addNode(node1));

               // Sorting sets level to the level of the node
               node0.level_ = 10;
               node1.level_ = node0.level_ + 1;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), node0.level());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }
            }

            void testSingleNodeChainMultiUpAndDown()
            {
               BPM_SCOPED_TRACE_COUT("testSingleNodeChainMultiUpAndDown");

               auto node0 = createNode("node0");
               auto node1 = createNode("node1");
               auto node2 = createNode("node2");
               auto node3 = createNode("node3");
               auto node4 = createNode("node4");

               // node0 --          --> node3
               //        |          |
               // node1 ---> node2 ---> node4
               node2.addUpstreamNode(node0);
               node2.addUpstreamNode(node1);
               node3.addUpstreamNode(node2);
               node4.addUpstreamNode(node2);

               bpm::core::NodeChain nodeChain;

               // Verifying empty node chain
               EXPECT_TRUE(nodeChain.verifyContinuity());

               // Sorting empty node chain resets level
               nodeChain.level_ = 10;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               // Single node chain, multiple upstream and downstream nodes
               EXPECT_TRUE(nodeChain.addNode(node2));
               EXPECT_TRUE(nodeChain.singleNodeChain());
               EXPECT_TRUE(nodeChain.verifyContinuity());

               // Duplicate
               EXPECT_FALSE(nodeChain.addNode(node2));

               // Cannot add to a single node chain
               EXPECT_FALSE(nodeChain.addNode(node3));

               // Sorting sets level to the level of the node
               node0.level_ = 10;
               node1.level_ = node0.level_ + 1;
               node2.level_ = node1.level_ + 1;
               node3.level_ = node2.level_ + 1;
               node4.level_ = node3.level_ + 1;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), node2.level());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }
            }

            void testNodeChain()
            {
               BPM_SCOPED_TRACE_COUT("testNodeChain");

               auto node0 = createNode("node0");
               auto node1 = createNode("node1");
               auto node2 = createNode("node2");
               auto node3 = createNode("node3");
               auto node4 = createNode("node4");

               // node4
               // node3
               // node0 ---> node1 ---> node2
               node1.addUpstreamNode(node0);
               node2.addUpstreamNode(node1);

               bpm::core::NodeChain nodeChain;

               EXPECT_TRUE(nodeChain.addNode(node0));
               EXPECT_TRUE(nodeChain.addNode(node1));
               EXPECT_TRUE(nodeChain.addNode(node2));

               // Sorting node chain with all same level resets level
               nodeChain.level_ = 100;
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               // Sorting sets level to the level of the node
               node2.level_ = 10;
               node1.level_ = node2.level_ + 1;
               node0.level_ = node1.level_ + 1;
               node3.level_ = node0.level_ + 1;
               node4.level_ = node3.level_ + 1;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), node0.level());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }

               // Node 3 would be a single node chain
               EXPECT_FALSE(nodeChain.addNode(node3));
               // node3 ---> node4
               // node0 ---> node1 ---> node2
               node4.addUpstreamNode(node3);
               // Node 3 is added but it is a part of another chain
               // so the sorting fails
               EXPECT_TRUE(nodeChain.addNode(node3));
               nodeChain.level_ = 100;
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);
            }

            void testNodeChainNoContinuity()
            {
               BPM_SCOPED_TRACE_COUT("testNodeChainNoContinuity");

               auto node0 = createNode("node0");
               auto node1 = createNode("node1");
               auto node2 = createNode("node2");
               auto node3 = createNode("node3");

               // node0 ---> node1 ---> node2
               node1.addUpstreamNode(node0);
               node2.addUpstreamNode(node1);

               node3.level_ = 10;
               node2.level_ = node3.level_ + 1;
               node1.level_ = node2.level_ + 1;
               node0.level_ = node1.level_ + 1;

               bpm::core::NodeChain nodeChain;

               // Add each end of the chain. The lack
               // of middle makes it non-continuous.
               EXPECT_TRUE(nodeChain.addNode(node0));
               EXPECT_TRUE(nodeChain.addNode(node2));

               // Sorting node chain with no continuity resets level
               nodeChain.level_ = 100;
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }

               // Add the middle to complete the chain
               EXPECT_TRUE(nodeChain.addNode(node1));

               // Sorting node chain with continuity sets level
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), node0.level());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }

               //            node3 --
               //                   |
               // node0 ---> node1 ---> node2
               node2.addUpstreamNode(node3);

               // Adding an upstream node to node2 makes it
               // a fan-in so it should not be considered part
               // of the chain. Sorting node chain with continuity
               // but incorrect number of nodes resets level.
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);
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


      TEST_F(TestNodeChain, testSingleNodeChain)
      {
         testSingleNodeChain();
      }


      TEST_F(TestNodeChain, testSingleNodeChainNoUpAndDown)
      {
         testSingleNodeChainNoUpAndDown();
      }


      TEST_F(TestNodeChain, testSingleNodeChainMultiUpAndDown)
      {
         testSingleNodeChainMultiUpAndDown();
      }


      TEST_F(TestNodeChain, testNodeChain)
      {
         testNodeChain();
      }


      TEST_F(TestNodeChain, testNodeChainNoContinuity)
      {
         testNodeChainNoContinuity();
      }
   }
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
