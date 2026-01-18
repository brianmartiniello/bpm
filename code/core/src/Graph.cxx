

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


void bpm::core::Node::addUpstreamNode(Node& node)
{
   const auto foundIter = std::find(upstreamNodes_.begin(),
                                    upstreamNodes_.end(),
                                    &node);
   if (upstreamNodes_.end() != foundIter)
   {
      // Already connected
      return;
   }

   upstreamNodes_.emplace_back(&node);

   if (circularDependency_)
   {
      // Exit if there is a circular dependency, cannot update level
      return;
   }

   // Make sure the upstream node has a higher level than this node
   node.updateLevel(level_ + 1,
                    name_);

   if (circularDependency_)
   {
      BPM_TRACE_COUT("Circular dependency detected when connecting node (" + name_ +
                     ") to upstream node (" + node.name_ + ")");
   }
}


void bpm::core::Node::addDownstreamNode(Node& node)
{
   const auto foundIter = std::find(downstreamNodes_.begin(),
                                    downstreamNodes_.end(),
                                    &node);
   if (downstreamNodes_.end() != foundIter)
   {
      // Already connected
      return;
   }

   downstreamNodes_.emplace_back(&node);

   if (circularDependency_)
   {
      // Exit if there is a circular dependency, cannot update level
      return;
   }

   // Make sure this node has a higher level than the downstream node
   updateLevel(node.level_ + 1,
               node.name_);

   if (circularDependency_)
   {
      BPM_TRACE_COUT("Circular dependency detected when connecting node (" + name_ +
                     ") to downstream node (" + node.name_ + ")");
   }
}


void bpm::core::Node::updateLevel(std::size_t newLevel,
                                  const std::string& originatingNodeName)
{
   // Exit if match with originating node, circular dependency detected
   if (originatingNodeName == name_)
   {
      return;
   }

   // Exit if lower level or no change
   if (newLevel <= level_)
   {
      return;
   }

   // New level is greater than current
   level_ = newLevel;
   maxLevel_ = std::max(maxLevel_,
                        level_);

   // The other upsteam nodes now need to be re-leveled
   for (auto& upstreamNode : upstreamNodes_)
   {
      upstreamNode->updateLevel(level_ + 1,
                                originatingNodeName);
   }
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
      out += "\n" + leadingText + "upstreamNodes[" + std::to_string(index) + "] (" + upstreamNode->name_ + ")";
   }

   out += "\n" + leadingText + "downstreamNodes.size() (" + std::to_string(downstreamNodes_.size()) + ")";
   index = -1U;
   for (const auto downstreamNode : downstreamNodes_)
   {
      ++index;
      out += "\n" + leadingText + "downstreamNodes[" + std::to_string(index) + "] (" + downstreamNode->name_ + ")";
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
   if (pair.second == false)
   {
      BPM_TRACE_COUT("Name (" + name + "): Already found in map");

      return false;
   }

   nodesWithoughUpstream_.emplace_back(&pair.first->second);
   nodesWithoughDownstream_.emplace_back(&pair.first->second);

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
      const auto eraseIter = std::find(nodesWithoughDownstream_.begin(),
                                       nodesWithoughDownstream_.end(),
                                       &upstreamNode);
      if (nodesWithoughDownstream_.end() != eraseIter)
      {
         nodesWithoughDownstream_.erase(eraseIter);
      }
   }

   // Add the upstream node to the downstream node
   downstreamNode.addUpstreamNode(upstreamNode);

   // This node now has an upstream node, so remove it
   {
      const auto eraseIter = std::find(nodesWithoughUpstream_.begin(),
                                       nodesWithoughUpstream_.end(),
                                       &downstreamNode);
      if (nodesWithoughUpstream_.end() != eraseIter)
      {
         nodesWithoughUpstream_.erase(eraseIter);
      }
   }  

   return true;
}


void bpm::core::Graph::listNodesByLevel()
{
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
         if (node.level_ != level)
         {
            continue;
         }

         nodesByLevel_.emplace_back(&node);
      }
   }
}
