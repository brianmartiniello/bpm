

#include <algorithm>

#include <bpm/core/leveling_graph/Graph.hxx>
#include <bpm/core/Logger.hxx>


std::string bpm::core::Graph::toString(const std::string& leadingText) const
{
   auto out = leadingText + "globalLevelPhase_ (" + std::to_string(globalLevelPhase_) + ")\n" +
              leadingText + "maxLevel_ (" + std::to_string(maxLevel_) + ")\n" +
              leadingText + "circularDependency_ (" + std::to_string(circularDependency_) + ")\n" +
              leadingText + "graph_.size() (" + std::to_string(graph_.size()) + ")";
   auto index = -1U;
   for (const auto& pair : graph_)
   {
      ++index;
      const auto prefix = leadingText + "graph_[" + std::to_string(index) + "].";
      out += "\n" + pair.second.toString(prefix);
   }
   return out;
}


bool bpm::core::Graph::addNode(const std::string& name,
                               bool hasInputPort)
{
   BPM_SCOPED_TRACE_COUT("Name (" + name + ")");

   if (true == name.empty())
   {
      BPM_ERROR_COUT("Name is empty");

      return false;
   }

   auto pair = graph_.emplace(name,
                              Node(name,
                                   hasInputPort,
                                   globalLevelPhase_,
                                   maxLevel_,
                                   circularDependency_));
   if (false == pair.second)
   {
      BPM_ERROR_COUT("Name (" + name + "): Already found in map");

      return false;
   }

   nodesWithoughUpstream_.emplace(&pair.first->second);
   nodesWithoughDownstream_.emplace(&pair.first->second);

   return true;
}


bool bpm::core::Graph::connectNodes(const std::string& downstreamName,
                                    const std::string& upstreamName)
{
   BPM_SCOPED_TRACE_COUT("");

   // Find downstream node name
   const auto downstreamIter = graph_.find(downstreamName);
   if (graph_.end() == downstreamIter)
   {
      BPM_ERROR_COUT("Downstream name (" + upstreamName +
                     "), upstream name (" + downstreamName +
                     "): Downstream name not found");

      return false;
   }
   auto& downstreamNode = downstreamIter->second;

   // Find upstream node name
   const auto upstreamIter = graph_.find(upstreamName);
   if (graph_.end() == upstreamIter)
   {
      BPM_ERROR_COUT("Downstream name (" + upstreamName +
                     "), upstream name (" + downstreamName +
                     "): Upsteam name not found");

      return false;
   }
   auto& upstreamNode = upstreamIter->second;

   // Add the downstream node to the upstream node
   upstreamNode.addDownstreamNode(downstreamNode);

   // This node now has a downstream node, so remove it
   {
      const auto eraseIter = nodesWithoughDownstream_.find(&upstreamNode);
      if (nodesWithoughDownstream_.end() != eraseIter)
      {
         nodesWithoughDownstream_.erase(eraseIter);
      }
   }

   // This node now has an upstream node, so remove it
   {
      const auto eraseIter = nodesWithoughUpstream_.find(&downstreamNode);
      if (nodesWithoughUpstream_.end() != eraseIter)
      {
         nodesWithoughUpstream_.erase(eraseIter);
      }
   }  

   return true;
}


void bpm::core::Graph::sortNodesByLevel()
{
   assignMaxLevel();

   // Clear current list
   nodesByLevel_.clear();

   // Walk through graph
   for (auto& pair : graph_)
   {
      auto& node = pair.second;

      nodesByLevel_.emplace_back(&node);
   }

   // Sort in descending order
   std::sort(nodesByLevel_.begin(),
             nodesByLevel_.end(),
             [](const auto a, const auto b)
             {
                return a->level() > b->level();
             });
}


void bpm::core::Graph::assignMaxLevel()
{
   auto nodeFoundWithInputPort = false;
   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes WITHOUT an upstream node and WITHOUT an input port
      if (false == nodePtr->hasInputPort()) continue;

      // This node WITHOUT an upstream node and WITH an input port
      nodeFoundWithInputPort = true;

      // Update its level so that it is at the max
      nodePtr->updateLevel(maxLevel_);
   }

   // Bump the max level if a node WITHOUT an upstream node
   // and WITH an input port was found so that nodes WITHOUT
   // an input port have a higher level
   maxLevel_ = (true == nodeFoundWithInputPort);

   auto nodeFoundWithoutInputPort = false;
   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes WITHOUT an upstream node and WITH an input port
      if (true == nodePtr->hasInputPort()) continue;

      // This node WITHOUT an upstream node and WITHOUT an input port
      nodeFoundWithoutInputPort = true;

      // Update its level so that it is at the max.
      // If the level was updated in the previous loop,
      // this node will have a higher level than those
      // WITH input ports.
      nodePtr->updateLevel(maxLevel_);
   }

   // If the max level was adjusted (nodeFoundWithInputPort) but
   // not used (nodeFoundWithoutInputPort), reset the max level
   // to its original value
   maxLevel_ -= ((true == nodeFoundWithInputPort) &&
                 (false == nodeFoundWithoutInputPort));
}


void bpm::core::Graph::reLevel()
{
   // Process all nodes without downstream nodes
   for (auto nodePtr : nodesWithoughDownstream_)
   {
      nodePtr->reLevel();
   }

   // Process all nodes without upstream nodes
   assignMaxLevel();
}


bool bpm::core::Graph::constructNodeChains()
{
   // Clear the visited flag
   std::for_each(nodesByLevel_.begin(),
                 nodesByLevel_.end(),
                 [](auto nodePtr)
                 {
                    nodePtr->clearVisited();
                 });

   // Clear the current data
   nodeChainsByLevel_.clear();

   // Loop over all nodes
   for (auto nodePtr : nodesByLevel_)
   {
      // Skip nodes already visited
      if (true == nodePtr->visited()) continue;

      // If single node chain
      if (true == NodeChain::singeNodeChain(*nodePtr))
      {
         // Create a node chain
         if (nullptr == createNodeChain(*nodePtr))
         {
            BPM_ERROR_COUT("Failed to create single node chain");

            // Exit if error
            return false;
         }

         // Move to next node on success
         continue;
      }

      // Continue if this node has more than one downstream node.
      // This is not the start of a chain.
      if (nodePtr->numDownstreamNodes() != 1) continue;

      // This node is the start of a chain.
      // Create a node chain.
      auto* nodeChainPtr = createNodeChain(*nodePtr);
      if (nullptr == nodeChainPtr)
      {
         BPM_ERROR_COUT("Failed to create node chain for a starting node");

         // Exit if error
         return false;
      }

      while (true)
      {
         // This node has one downstream node due to either
         // the check above prior to entering the loop,
         // or the check at the end of the loop.
         // Move to the next downstream node.
         nodePtr = nodePtr->downstreamNode();

         // Exit if this node does not exist.
         // The previous node was the end of the chain.
         if (nullptr == nodePtr) break;

         // The move to the next downstream node guarantees
         // that this node does not have zero upstream nodes.

         // Exit if this node has more than one upstream node.
         // This node is a fan-in node.
         if (nodePtr->numUpstreamNodes() != 1) break;

         // This node exists and has one upstream node.
         // Add this node to the node chain.
         if (false == nodeChainPtr->addNode(*nodePtr))
         {
            BPM_ERROR_COUT("Failed to add node (" + nodePtr->name() + ") to chain");

            return false;
         }

         // Mark the node as visited
         nodePtr->markVisited();

         // Exit if this node has more than one downstream node.
         // This node is a fan-out node.
         if (nodePtr->numDownstreamNodes() != 1) break;
      }
   }

   // Sort the nodes within a chain
   std::for_each(nodeChainsByLevel_.begin(),
                 nodeChainsByLevel_.end(),
                 [](auto& nodeChain)
                 {
                    nodeChain.sortNodesByLevel();
                 });

   // Sort the chains in descending order
   std::sort(nodeChainsByLevel_.begin(),
             nodeChainsByLevel_.end(),
             [](const auto& a, const auto& b)
             {
                return a.level() > b.level();
             });

   return true;
}


bpm::core::NodeChain* bpm::core::Graph::createNodeChain(Node& node)
{
   // Create the node chain
   nodeChainsByLevel_.emplace_back(NodeChain());
   auto nodeChainPtr = &nodeChainsByLevel_.back();

   // Add this node to the node chain
   if (false == nodeChainPtr->addNode(node))
   {
      BPM_ERROR_COUT("Failed to add node (" + node.name() + ") to chain");

      return nullptr;
   }

   // Mark the node as visited
   node.markVisited();

   return nodeChainPtr;
};
