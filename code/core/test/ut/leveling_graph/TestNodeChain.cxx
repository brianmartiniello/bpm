

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

               // CASE - Num Upstream - Num Downstream - Expected
               // 0      0              0                true
               // 1      0              1                false
               // 2      0              2                true
               // 3      1              0                false
               // 4      1              1                false
               // 5      1              2                false
               // 6      2              0                true
               // 7      2              1                false
               // 8      2              2                true

               {
                  auto node0 = createNode("node0");
                  BPM_TRACE_COUT("CASE 0");
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");

                  // node0 ---> node1 ---> node2
                  node0.addUpstreamNode(node1);
                  node1.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 1");
                  EXPECT_FALSE(NodeChain::singeNodeChain(node0));
                  BPM_TRACE_COUT("CASE 4");
                  EXPECT_FALSE(NodeChain::singeNodeChain(node1));
                  BPM_TRACE_COUT("CASE 3");
                  EXPECT_FALSE(NodeChain::singeNodeChain(node2));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");
                  auto node3 = createNode("node3");

                  // node0 --
                  //        |
                  // node1 ---> node2
                  node0.addUpstreamNode(node2);
                  node1.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 2");
                  EXPECT_TRUE(NodeChain::singeNodeChain(node2));

                  // node0 --
                  //        |
                  // node1 ---> node2 ---> node3
                  node2.addUpstreamNode(node3);

                  BPM_TRACE_COUT("CASE 5");
                  EXPECT_FALSE(NodeChain::singeNodeChain(node2));
               }

               {
                  auto node0 = createNode("node0");
                  auto node1 = createNode("node1");
                  auto node2 = createNode("node2");
                  auto node3 = createNode("node3");
                  auto node4 = createNode("node4");

                  //        --> node1
                  //        |
                  // node0 ---> node2
                  node0.addUpstreamNode(node1);
                  node0.addUpstreamNode(node2);

                  BPM_TRACE_COUT("CASE 6");
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));

                  //                   --> node1
                  //                   |
                  // node3 ---> node0 ---> node2
                  node3.addUpstreamNode(node0);

                  BPM_TRACE_COUT("CASE 7");
                  EXPECT_FALSE(NodeChain::singeNodeChain(node0));

                  // node4 --          --> node1
                  //        |          |
                  // node3 ---> node0 ---> node2
                  node4.addUpstreamNode(node0);

                  BPM_TRACE_COUT("CASE 8");
                  EXPECT_TRUE(NodeChain::singeNodeChain(node0));
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
               node0.addUpstreamNode(node2);
               node1.addUpstreamNode(node2);
               node2.addUpstreamNode(node3);
               node2.addUpstreamNode(node4);

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

               // node3
               // node0 ---> node1 ---> node2
               node0.addUpstreamNode(node1);
               node1.addUpstreamNode(node2);

               bpm::core::NodeChain nodeChain;

               EXPECT_TRUE(nodeChain.addNode(node0));
               EXPECT_TRUE(nodeChain.addNode(node1));
               EXPECT_TRUE(nodeChain.addNode(node2));

               // Sorting node chain with all same level resets level
               nodeChain.level_ = 100;
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               // Sorting sets level to the level of the node
               node0.level_ = 10;
               node1.level_ = node0.level_ + 1;
               node2.level_ = node1.level_ + 1;
               node3.level_ = node2.level_ + 1;
               EXPECT_TRUE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), node2.level());

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }

               // Node 3 would be a single node chain
               EXPECT_FALSE(nodeChain.addNode(node3));
               // node3 --
               //        |
               // node0 ---> node1 ---> node2
               node3.addUpstreamNode(node1);
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

               // node0 ---> node1 ---> node2
               node0.addUpstreamNode(node1);
               node1.addUpstreamNode(node2);

               bpm::core::NodeChain nodeChain;

               EXPECT_TRUE(nodeChain.addNode(node0));
               EXPECT_TRUE(nodeChain.addNode(node2));

               // Sorting node chain with no continuity resets level
               nodeChain.level_ = 10;
               EXPECT_FALSE(nodeChain.sortNodesByLevel());
               EXPECT_EQ(nodeChain.level(), 0);

               {
                  BPM_SCOPED_TRACE_COUT("toString");
                  BPM_TRACE_COUT("\n" + nodeChain.toString("   "));
               }
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
