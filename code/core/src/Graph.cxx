

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


bpm::core::Node::Node(const std::string& name,
                      bool hasInputPort,
                      std::size_t& maxLevel,
                      bool& circularDependency)
   : name_(name)
   , level_()
   , upstreamNodes_()
   , downstreamNodes_()
   , maxLevel_(maxLevel)
   , circularDependency_(circularDependency)
   , hasInputPort_(hasInputPort)
{
}


void bpm::core::Node::addUpstreamNode(Node& node,
                                      bool updateLevels)
{
   // Exit if already connected
   if (this == &node)
   {
      BPM_TRACE_COUT("Attempting to connected node (" + name_ +
                     ") to itself");
      return;
   }

   // Exit if already connected
   auto insterPair = upstreamNodes_.emplace(&node);
   if (false == insterPair.second)
   {
      BPM_TRACE_COUT("Already connected node (" + name_ +
                     ") to upstream node (" + node.name() + ")");
      return;
   }

   // Add this node as downstream to the upstream node
   node.addDownstreamNode(*this,
                          updateLevels);

   // Exit if not updating level
   if (false == updateLevels) return;

   // Exit if there is a circular dependency, cannot update level
   if (true == circularDependency_) return;

   // Make sure the upstream node has a higher level than this node
   node.updateLevel(level_ + 1,
                    name_);

   if (true == circularDependency_)
   {
      BPM_TRACE_COUT("Circular dependency detected when connecting node (" + name_ +
                     ") to upstream node (" + node.name() + ")");
   }
}


void bpm::core::Node::addDownstreamNode(Node& node,
                                        bool updateLevels)
{
   // Exit if already connected
   if (this == &node)
   {
      BPM_TRACE_COUT("Attempting to connected node (" + name_ +
                     ") to itself");
      return;
   }

   // Exit if already connected
   auto insterPair = downstreamNodes_.emplace(&node);
   if (false == insterPair.second)
   {
      BPM_TRACE_COUT("Already connected node (" + name_ +
                     ") to downstream node (" + node.name() + ")");
      return;
   }

   // Add this node as upstream to the downstream node
   node.addUpstreamNode(*this,
                        updateLevels);

   // Exit if not updating level
   if (false == updateLevels) return;

   // Exit if there is a circular dependency, cannot update level
   if (true == circularDependency_) return;

   // Make sure this node has a higher level than the downstream node
   updateLevel(node.level() + 1,
               node.name());

   if (true == circularDependency_)
   {
      BPM_TRACE_COUT("Circular dependency detected when connecting node (" + name_ +
                     ") to downstream node (" + node.name() + ")");
   }
}


void bpm::core::Node::updateLevel(std::size_t newLevel,
                                  const std::string& originatingNodeName)
{
   // Exit if match with originating node, circular dependency detected
   if (originatingNodeName == name_)
   {
      circularDependency_ = true;
      return;
   }

   // Exit if no change
   if (false == setLevel(newLevel)) return;

   // The upsteam nodes now need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                originatingNodeName);
   }
}


void bpm::core::Node::updateLevel(std::size_t newLevel)
{
   // Exit if no change
   if (false == setLevel(newLevel)) return;

   // The upsteam nodes now need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                name_);
   }
}


void bpm::core::Node::reLevel()
{
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


std::string bpm::core::Graph::toString(const std::string& leadingText) const
{
   auto out = leadingText + "graph_.size() (" + std::to_string(graph_.size()) + ")";
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
      BPM_TRACE_COUT("Name is empty");

      return false;
   }

   auto pair = graph_.emplace(name,
                              Node(name,
                                   hasInputPort,
                                   maxLevel_,
                                   circularDependency_));
   if (false == pair.second)
   {
      BPM_TRACE_COUT("Name (" + name + "): Already found in map");

      return false;
   }

   nodesWithoughUpstream_.emplace(&pair.first->second);
   nodesWithoughDownstream_.emplace(&pair.first->second);

   return true;
}


bool bpm::core::Graph::connectNodes(const std::string& upstreamName,
                                    const std::string& downstreamName)
{
   BPM_SCOPED_TRACE_COUT("");

   // Find upstream node name
   const auto upstreamIter = graph_.find(upstreamName);
   if (graph_.end() == upstreamIter)
   {
      BPM_TRACE_COUT("Upstream name (" + upstreamName + ") not found");

      return false;
   }
   auto& upstreamNode = upstreamIter->second;

   // Find downstream node name
   const auto downstreamIter = graph_.find(downstreamName);
   if (graph_.end() == downstreamIter)
   {
      BPM_TRACE_COUT("Downstream name (" + downstreamName + ") not found");

      return false;
   }
   auto& downstreamNode = downstreamIter->second;

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


void bpm::core::Graph::listNodesByLevel()
{
   assignMaxLevel();

   // Clear current list
   nodesByLevel_.clear();

   // Reserve space for all nodes of graph
   nodesByLevel_.reserve(graph_.size());

   // Walk down through levels from max
   for (auto i = maxLevel_ + 1; i > 0; --i)
   {
      const auto level = i - 1;

      // Walk through graph
      for (auto& pair : graph_)
      {
         auto& node = pair.second;

         // Skip mimatched levels
         if (node.level() != level) continue;

         nodesByLevel_.emplace_back(&node);
      }
   }
}


void bpm::core::Graph::assignMaxLevel()
{
   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes without an input port
      if (false == nodePtr->hasInputPort()) continue;

      // Skip if less than max level
      if (nodePtr->level() < maxLevel_) continue;

      // Max level is one greater
      maxLevel_ = nodePtr->level() + 1;
      break;
   }

   for (const auto nodePtr : nodesWithoughUpstream_)
   {
      // Skip nodes with an input port
      if (true == nodePtr->hasInputPort()) continue;

      nodePtr->updateLevel(maxLevel_);
   }
}


void bpm::core::Graph::reLevel()
{
   for (auto nodePtr : nodesWithoughDownstream_)
   {
      nodePtr->reLevel();
   }
}
