

#include <algorithm>

#include <bpm/core/leveling_graph/NodeChain.hxx>
#include <bpm/core/Logger.hxx>


std::string bpm::core::NodeChainSimple::toString(const std::string& leadingText) const
{
   auto out = leadingText + "level_ (" + std::to_string(level_) + ")\n" +
              leadingText + "nodeNames_.size() (" + std::to_string(nodeNames_.size()) + ")";
   auto index = -1U;
   for (const auto& nodeName : nodeNames_)
   {
      ++index;
      out += "\n" + leadingText +
             "nodeNames_[" + std::to_string(index) +
             "] (" + nodeName + ")";
   }
   return out;
}


namespace
{
   bool checkMultiUpstream(bpm::core::Node& node)
   {
      // Check for multiple outputs
      if (node.numDownstreamNodes() > 1)
      {
         // Fan-in and fan-out
         // nodeA --          --> nodeE
         //        |          |
         // nodeB ---> nodeC ---> nodeD
         //            *****
         return true;
      }
      // Check for single output
      else if (node.numDownstreamNodes() == 1)
      {
         // Check input of output node
         const auto outHasMultiIn = node.downstreamNode()->numUpstreamNodes() > 1;

         BPM_TRACE_COUT("Node (" + node.name() +
                        "), downstream node (" + node.downstreamNode()->name() +
                        "), outHasMultiIn (" + BPM_LOG_BOOL(outHasMultiIn) +
                        ")");

         if (true == outHasMultiIn)
         {
            // Downstream is a fan-in
            // nodeA --   nodeE --
            //        |          |
            // nodeB ---> nodeC ---> nodeD
            //            *****
            return true;
         }
         else // (false == outHasMultiIn)
         {
            // Start of chain from fan-in
            // nodeA --
            //        |
            // nodeB ---> nodeC ---> nodeD
            //            *****
            return false;
         }
      }
      // No ouputs
      else // (node.numDownstreamNodes() == 1)
      {
         // nodeA --
         //        |
         // nodeB ---> nodeC
         //            *****
         return true;
      }
   }


   bool checkSingleUpstream(bpm::core::Node& node)
   {
      // Check output of input node
      const auto inHasMultiOut = node.upstreamNode()->numDownstreamNodes() > 1;

      BPM_TRACE_COUT("Node (" + node.name() +
                     "), upstream node (" + node.upstreamNode()->name() +
                     "), inHasMultiOut (" + BPM_LOG_BOOL(inHasMultiOut) +
                     ")");

      // Check for multiple outputs
      if (node.numDownstreamNodes() > 1)
      {
         if (true == inHasMultiOut)
         {
            // Upstream is a fan-out and this is a fan-out
            //        --> nodeE   --> nodeD
            //        |           |
            // nodeA ---> nodeB ----> nodeC
            //            *****
            return true;
         }
         else // (false == inHasMultiOut)
         {
            // End of chain at a fan-out
            //                   --> nodeD
            //                   |
            // nodeA ---> nodeB ---> nodeC
            //            *****
            return false;
         }
      }
      // Check for single output
      else if (node.numDownstreamNodes() == 1)
      {
         // Check input of output node
         const auto outHasMultiIn = node.downstreamNode()->numUpstreamNodes() > 1;

         BPM_TRACE_COUT("Node (" + node.name() +
                        "), downstream node (" + node.downstreamNode()->name() +
                        "), outHasMultiIn (" + BPM_LOG_BOOL(outHasMultiIn) +
                        ")");

         if ((false == inHasMultiOut) &&
             (true == outHasMultiIn))
         {
            // Upstream is not a fan-out, downstream is a fan-in, end of chain
            //            nodeD --
            //                   |
            // nodeA ---> nodeB ---> nodeC
            //            *****
            return false;
         }
         else if ((false == inHasMultiOut) &&
                  (false == outHasMultiIn))
         {
            // Upstream is not a fan-out, downstream is not a fan-in, middle of chain
            // nodeA ---> nodeB ---> nodeC
            //            *****
            return false;
         }
         else if ((true == inHasMultiOut) &&
                  (true == outHasMultiIn))
         {
            // Upstream is a fan-out, downstream is a fan-in, single node chain
            //        --> nodeE  nodeD --
            //        |                 |
            // nodeA ---> nodeB ----------> nodeC
            //            *****
            return true;
         }
         else // ((true == inHasMultiOut) &&
              //  (false == outHasMultiIn))
         {
            // Upstream is a fan-out, downstream is not a fan-in, start of chain
            //        --> nodeE
            //        |
            // nodeA ---> nodeB ----> nodeC
            //            *****
            return false;
         }
      }
      // No outputs
      else // (node.numUpstreamNodes() == 0)
      {
         if (true == inHasMultiOut)
         {
            // No output and upstream is a fan-out
            //        --> nodeE
            //        |
            // nodeA ---> nodeB
            //            *****
            return true;
         }
         else // (false == inHasMultiOut)
         {
            // No output and upstream is not a fan-out, end of chain
            // nodeA ---> nodeB
            //            *****
            return false;
         }
      }
   }


   bool checkNoUpstream(bpm::core::Node& node)
   {
      // Check for multiple outputs
      if (node.numDownstreamNodes() > 1)
      {
         // Fan-out
         //        --> nodeC
         //        |
         // nodeA ---> nodeB
         // *****
         return true;
      }
      // Check for single output
      else if (node.numDownstreamNodes() == 1)
      {
         // Check input of output node
         const auto outHasMultiIn = node.downstreamNode()->numUpstreamNodes() > 1;

         BPM_TRACE_COUT("Node (" + node.name() +
                        "), downstream node (" + node.downstreamNode()->name() +
                        "), outHasMultiIn (" + BPM_LOG_BOOL(outHasMultiIn) +
                        ")");

         if (true == outHasMultiIn)
         {
            // Downstream is a fan-in
            // nodeC --
            //        |
            // nodeA ---> nodeB
            // *****
            return true;
         }
         else // (false == outHasMultiIn)
         {
            // Downstream is a not fan-in, start of chain
            // nodeA ---> nodeB
            // *****
            return false;
         }
      }
      // No outputs
      else // (node.numDownstreamNodes() == 0)
      {
         // Standalone
         // nodeA
         // *****
         return true;
      }
   }
}


/* static */ bool bpm::core::NodeChain::isSingleNodeChain(Node& node)
{
   BPM_TRACE_COUT("Node (" + node.name() +
                  "), numUpstreamNodes (" + std::to_string(node.numUpstreamNodes()) +
                  "), numDownstreamNodes (" + std::to_string(node.numDownstreamNodes()) +
                  ")");
 
   // Check for multiple inputs
   if (node.numUpstreamNodes() > 1)
   {
      return checkMultiUpstream(node);
   }
   // Check for single input
   else if (node.numUpstreamNodes() == 1)
   {
      return checkSingleUpstream(node);
   }
   // No inputs
   else // (node.numUpstreamNodes() == 0)
   {
      return checkNoUpstream(node);
   }
}


bool bpm::core::NodeChain::addNode(Node& node)
{
   // Detect a single node chain
   const auto singleNodeChainIn = isSingleNodeChain(node);

   // If no current nodes, add and retrurn
   if (0 == nodes_.size())
   {
      // First node of the chain
      nodes_.emplace_back(&node);

      // Store the single node chain flag
      singleNodeChain_ = singleNodeChainIn;

      return true;
   }

   // Check for duplicate node
   const auto iter = std::find(nodes_.begin(),
                               nodes_.end(),
                               &node);
   if (iter != nodes_.end())
   {
      BPM_ERROR_COUT("Node (" + node.name() +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Already added");

      return false;
   }

   // Check if the chain is already set for a single node
   if (true == singleNodeChain_)
   {
      BPM_ERROR_COUT("Node (" + node.name() +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Chain with node (" + nodes_.front()->name() +
                     ") is a single node chain, cannot add node");

      return false;
   }

   // Check if the input node is for a single node chain
   if (true == singleNodeChainIn)
   {
      BPM_ERROR_COUT("Node (" + node.name() +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Cannot add node since it would be for a single node chain");

      return false;
   }

   // The node is valid for this chain
   nodes_.emplace_back(&node);

   return true;
}


bool bpm::core::NodeChain::addNodeAsSingle(Node& node)
{
   if (false == addNode(node))
   {
      return false;
   }

   singleNodeChain_ = true;

   return true;
}


bool bpm::core::NodeChain::sortNodesByLevel()
{
   level_ = 0;

   if (0 == nodes_.size())
   {
      return true;
   }

   // Sort in descending order
   std::sort(nodes_.begin(),
             nodes_.end(),
             [](const auto a, const auto b)
             {
                return a->level() > b->level();
             });

   // Look for duplicate levels
   const auto iter = std::adjacent_find(nodes_.begin(),
                                        nodes_.end(),
                                        [](const auto a, auto b)
                                        {
                                           return a->level() == b->level();
                                        });
   if (iter != nodes_.end())
   {
      BPM_ERROR_COUT("Front node (" + nodes_.front()->name() +
                     ") - Duplicate level value found in chain");

      return false;
   }

   if (false == verifyContinuity())
   {
      BPM_ERROR_COUT("Front node (" + nodes_.front()->name() +
                     ") - Failed to verify continuity");

      return false;
   }

   // The level of this chain is the highest node level
   level_ = nodes_.front()->level();

   return true;
}


bool bpm::core::NodeChain::verifyContinuity() const
{
   if (0 == nodes_.size())
   {
      return true;
   }

   // Count nodes
   auto nodeCount = 0;

   // Start at the top of the chain
   auto iter = nodes_.front();

   do
   {
      // Count this node
      ++nodeCount;

      // If this node has no downstream node, it is
      // the end of the chain. If it has more than
      // one downstream node, it is the end of the
      // chain due to fan-out.
      if (1 != iter->numDownstreamNodes()) break;

      // Move to the next downstream node
      iter = iter->downstreamNode();

      // This node has at least one upstream node
      // due to the node previous. However, it *may*
      // have more than one upstream due to fan-in.
      // If so, this node is not part of the chain.
      if (1 != iter->numUpstreamNodes()) break;
   }
   while (true);

   // The chain is continuos if we counted all the nodes
   if (nodeCount != nodes_.size())
   {
      BPM_ERROR_COUT("Front node (" + nodes_.front()->name() +
                     ") - Detected an invalid chain, " +
                     "node count after traversal is (" +
                     std::to_string(nodeCount) +
                     ") while total number of nodes is (" +
                     std::to_string(nodes_.size()) + ")");
      return false;
   }

   return true;
}


std::string bpm::core::NodeChain::toString(const std::string& leadingText) const
{
   auto out = leadingText + "level_ (" + std::to_string(level_) + ")\n" +
              leadingText + "nodes_.size() (" + std::to_string(nodes_.size()) + ")";
   auto index = -1U;
   for (const auto nodePtr : nodes_)
   {
      ++index;
      const auto prefix = leadingText + "nodes_[" + std::to_string(index) + "].";
      out += "\n" + nodePtr->toString(prefix);
   }
   return out;
}


bool bpm::core::NodeChain::operator==(const NodeChainSimple& nodeChainSimple) const
{
   if (level_ != nodeChainSimple.level_)
   {
      return false;
   }
   
   if (nodes_.size() != nodeChainSimple.nodeNames_.size())
   {
      return false;
   }

   for (const auto nodePtr : nodes_)
   {
      const auto iter = std::find(nodeChainSimple.nodeNames_.begin(),
                                  nodeChainSimple.nodeNames_.end(),
                                  nodePtr->name());
      if (iter == nodeChainSimple.nodeNames_.end())
      {
         return false;
      }
   }

   return true;
}
