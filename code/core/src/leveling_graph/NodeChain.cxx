

#include <algorithm>

#include <bpm/core/leveling_graph/NodeChain.hxx>
#include <bpm/core/Logger.hxx>


bool bpm::core::NodeChain::addNode(Node& node)
{
   // Check for multiple inputs and multiple outputs
   const auto multiInOutNodeIn = ((node.numUpstreamNodes() > 1) &&
                                  (node.numDownstreamNodes() > 1));
   
   // Check for no inputs and no outputs
   const auto noInOutNodeIn = ((node.numUpstreamNodes() == 0) &&
                               (node.numDownstreamNodes() == 0));

   // Detect a single node chain
   const auto singleNodeChainIn = multiInOutNodeIn | noInOutNodeIn;

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
                     "), multiInOutNodeIn (" + BPM_LOG_BOOL(multiInOutNodeIn) +
                     "), noInOutNodeIn (" + BPM_LOG_BOOL(noInOutNodeIn) +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Already added");

      return false;
   }

   // Check if the chain is already set for a single node
   if (true == singleNodeChain_)
   {
      BPM_ERROR_COUT("Node (" + node.name() +
                     "), multiInOutNodeIn (" + BPM_LOG_BOOL(multiInOutNodeIn) +
                     "), noInOutNodeIn (" + BPM_LOG_BOOL(noInOutNodeIn) +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Chain with node (" + nodes_.front()->name() +
                     ") is a single node chain, cannot add node");

      return false;
   }

   // Check if the input node is for a single node chain
   if (true == singleNodeChainIn)
   {
      BPM_ERROR_COUT("Node (" + node.name() +
                     "), multiInOutNodeIn (" + BPM_LOG_BOOL(multiInOutNodeIn) +
                     "), noInOutNodeIn (" + BPM_LOG_BOOL(noInOutNodeIn) +
                     "), singleNodeChainIn (" + BPM_LOG_BOOL(singleNodeChainIn) +
                     ") - Cannot add node since it would be for a single node chain");

      return false;
   }

   // The node is valid for this chain
   nodes_.emplace_back(&node);

   return true;
}


bool bpm::core::NodeChain::sortNodesByLevel()
{
   // Sort in descending order
   std::sort(nodes_.begin(),
             nodes_.end(),
             [](const auto a, auto b)
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
      BPM_ERROR_COUT("Duplicate level value found in chain");

      level_ = 0;

      return false;
   }

   if (false == verifyContinuity())
   {
      BPM_ERROR_COUT("Failed to verify continuity");

      level_ = 0;

      return false;
   }

   // The level of this chain is the highest node level
   level_ = (nodes_.size() > 0) ?
            nodes_.front()->level() : 0;

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

      // If this node has no downstream node or 
      // more than one downstream nodes, it is
      // the end of the chain
      if (1 != iter->numDownstreamNodes()) break;

      // Move to the next downstream node
      iter = iter->downstreamNode();
   }
   while (true);

   // The chain is continuos if we counted all the nodes
   if (nodeCount != nodes_.size())
   {
      BPM_ERROR_COUT(std::string("Detected an invalid chain, ") +
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
