

#include <algorithm>

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


bpm::core::Node::Node(const std::string& name,
                      bool hasInputPort,
                      std::size_t& globalLevelPhase,
                      std::size_t& maxLevel,
                      bool& circularDependency)
   : name_(name)
   , level_(0)
   , levelPhase_(0)
   , globalLevelPhase_(globalLevelPhase)
   , upstreamNodes_()
   , downstreamNodes_()
   , maxLevel_(maxLevel)
   , circularDependency_(circularDependency)
   , hasInputPort_(hasInputPort)
{
}


void bpm::core::Node::addUpstreamNode(Node& node)
{
   // Exit if already connected
   if (this == &node)
   {
      BPM_WARN_COUT("Attempting to connected node (" + name_ +
                    ") to itself");
      return;
   }

   // Exit if already connected
   auto insterPair = upstreamNodes_.emplace(&node);
   if (false == insterPair.second)
   {
      BPM_WARN_COUT("Already connected node (" + name_ +
                    ") to upstream node (" + node.name() + ")");
      return;
   }

   // Add this node as downstream to the upstream node
   node.addDownstreamNode(*this);
}


void bpm::core::Node::addDownstreamNode(Node& node)
{
   // Exit if already connected
   if (this == &node)
   {
      BPM_WARN_COUT("Attempting to connected node (" + name_ +
                    ") to itself");
      return;
   }

   // Exit if already connected
   auto insterPair = downstreamNodes_.emplace(&node);
   if (false == insterPair.second)
   {
      BPM_WARN_COUT("Already connected node (" + name_ +
                    ") to downstream node (" + node.name() + ")");
      return;
   }

   // Add this node as upstream to the downstream node
   node.addUpstreamNode(*this);
}


void bpm::core::Node::updateLevel(std::size_t newLevel,
                                  const std::string& previousNodeName)
{
   BPM_TRACE_COUT("Prev node (" + previousNodeName +
                  "), new level (" + std::to_string(newLevel) +
                  "), global phase (" + std::to_string(globalLevelPhase_) + 
                  "), node (" + name_ +
                  "), level (" + std::to_string(level_) +
                  "), phase (" + std::to_string(levelPhase_) +")");

   // Exit if no change
   if (false == setLevel(newLevel)) return;

   // Exit if match with phase, circular dependency detected
   if (levelPhase_ == globalLevelPhase_)
   {
      BPM_TRACE_COUT("Prev node (" + previousNodeName +
                     "), new level (" + std::to_string(newLevel) +
                     "), prev phase (" + std::to_string(globalLevelPhase_) + 
                     "), node (" + name_ +
                     "), level (" + std::to_string(level_) +
                     "), phase (" + std::to_string(levelPhase_) +
                     ") - Circular dependency detected");

      circularDependency_ = true;
      return;
   }

   // Match the phase
   levelPhase_ = globalLevelPhase_;

   // The upsteam nodes now need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                name_);
   }
}


void bpm::core::Node::updateLevel(std::size_t newLevel)
{
   // Exit if no change
   if (false == setLevel(newLevel)) return;

   // Indicate a new phase
   ++globalLevelPhase_;

   // The upsteam nodes now need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                name_);
   }
}


void bpm::core::Node::reLevel()
{
   // Indicate a new phase
   ++globalLevelPhase_;

   // Check if the upsteam nodes need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                name_);
   }
}


bool bpm::core::Node::setLevel(std::size_t newLevel)
{
   // Exit if lower level or no change
   if (newLevel <= level_) return false;

   // New level is greater than current
   level_ = newLevel;
   maxLevel_ = std::max(maxLevel_,
                        level_);

   return true;
}


std::string bpm::core::Node::toString(const std::string& leadingText) const
{
   auto out = leadingText + "name (" + name_ + ")" +
              "\n" + leadingText + "level (" + std::to_string(level_) + ")";

   out += "\n" + leadingText + "upstreamNodes.size() (" + std::to_string(upstreamNodes_.size()) + ")";
   auto index = -1U;
   for (const auto upstreamNode : upstreamNodes_)
   {
      ++index;
      out += "\n" + leadingText + "upstreamNodes[" + std::to_string(index) + "] (" + upstreamNode->name() + ")";
   }

   out += "\n" + leadingText + "downstreamNodes.size() (" + std::to_string(downstreamNodes_.size()) + ")";
   index = -1U;
   for (const auto downstreamNode : downstreamNodes_)
   {
      ++index;
      out += "\n" + leadingText + "downstreamNodes[" + std::to_string(index) + "] (" + downstreamNode->name() + ")";
   }

   return out;
}


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

   //The level of this chain is the highest node level
   level_ = (nodes_.size() > 0) ?
            nodes_.front()->level() : 0;

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

      return false;
   }

   if (false == verifyContinuity())
   {
      BPM_ERROR_COUT("Failed to verify continuity");

      return false;
   }

   return true;
}


bool bpm::core::NodeChain::verifyContinuity() const
{
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
             [](const auto a, auto b)
             {
                return a->level() > b->level();
             });
}


void bpm::core::Graph::assignMaxLevel()
{
   auto nodeFound = false;

   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes without an input port
      if (false == nodePtr->hasInputPort()) continue;

      // Skip if less than max level
      if (nodePtr->level() < maxLevel_) continue;

      nodeFound = true;

      // Max level is one greater
      maxLevel_ = nodePtr->level() + 1;
      break;
   }

   // Return if node not found, otherwise reset
   if (false == nodeFound) return;
   else nodeFound = false;

   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes with an input port
      if (true == nodePtr->hasInputPort()) continue;

      nodeFound = true;

      nodePtr->updateLevel(maxLevel_);
   }

   // If no nodes found, reset
   maxLevel_ -= (false == nodeFound);
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
