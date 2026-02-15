

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
      // Skip nodes WITHOUT an upstream node and WITHOUT an input port
      if (false == nodePtr->hasInputPort()) continue;

      // Skip nodes WITHOUT an upstream node and WITH an input port if
      // its level is less than the max
      if (nodePtr->level() < maxLevel_) continue;

      // This node WITHOUT an upstream node, WITH an input port, 
      // is at the max level
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
      // Skip nodes WITHOUT an upstream node and WITH an input port
      if (true == nodePtr->hasInputPort()) continue;

      // This node WITHOUT an upstream node and WITHOUT an input port
      nodeFound = true;

      // Update its level so that it is higher than those WITH input ports
      nodePtr->updateLevel(maxLevel_);
   }

   // If no nodes were releveled, reset the max level to its original value
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
