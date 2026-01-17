

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


bpm::core::Graph::Node::Node(const std::string& name,
                             std::vector<Node*>& nodesWithoughUpstream,
                             std::vector<Node*>& nodesWithoughDownstream)
   : name(name)
   , level()
   , upstreamNodes()
   , downstreamNodes()
   , nodesWithoughUpstream(nodesWithoughUpstream)
   , nodesWithoughDownstream(nodesWithoughDownstream)
{
   nodesWithoughUpstream.emplace_back(this);
   nodesWithoughDownstream.emplace_back(this);
}


void bpm::core::Graph::Node::addUpstreamNode(Node& node)
{
   const auto foundIter = std::find(upstreamNodes.begin(),
                                    upstreamNodes.end(),
                                    &node);
   if (upstreamNodes.end() != foundIter)
   {
      // Already connected
      return;
   }

   upstreamNodes.emplace_back(&node);

   // This node now has an upstream node, so remove it
   const auto eraseIter = std::find(nodesWithoughUpstream.begin(),
                                    nodesWithoughUpstream.end(),
                                    this);
   if (nodesWithoughUpstream.end() != eraseIter)
   {
      nodesWithoughUpstream.erase(eraseIter);
   }
}


void bpm::core::Graph::Node::addDownstreamNode(Node& node)
{
   const auto foundIter = std::find(downstreamNodes.begin(),
                                    downstreamNodes.end(),
                                    &node);
   if (downstreamNodes.end() != foundIter)
   {
      // Already connected
      return;
   }

   downstreamNodes.emplace_back(&node);

   // This node now has a downstream node, so remove it
   const auto eraseIter = std::find(nodesWithoughDownstream.begin(),
                                    nodesWithoughDownstream.end(),
                                    this);
   if (nodesWithoughDownstream.end() != eraseIter)
   {
      nodesWithoughDownstream.erase(eraseIter);
   }
}


std::string bpm::core::Graph::Node::toString(const std::string& leadingText) const
{
   auto out = leadingText + "name (" + name + ")" +
              "\n" + leadingText + "level (" + std::to_string(level) + ")";

   out += "\n" + leadingText + "upstreamNodes.size() (" + std::to_string(upstreamNodes.size()) + ")";
   auto index = -1U;
   for (const auto upstreamNode : upstreamNodes)
   {
      ++index;
      out += "\n" + leadingText + "upstreamNodes[" + std::to_string(index) + "] (" + upstreamNode->name + ")";
   }

   out += "\n" + leadingText + "downstreamNodes.size() (" + std::to_string(downstreamNodes.size()) + ")";
   index = -1U;
   for (const auto downstreamNode : downstreamNodes)
   {
      ++index;
      out += "\n" + leadingText + "downstreamNodes[" + std::to_string(index) + "] (" + downstreamNode->name + ")";
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


bool bpm::core::Graph::addNode(const std::string& name)
{
   BPM_SCOPED_TRACE_COUT("Name (" + name + ")");

   if (true == name.empty())
   {
      BPM_TRACE_COUT("Name is empty");

      return false;
   }

   auto pair = graph_.emplace(name,
                              Node(name,
                                   nodesWithoughUpstream_,
                                   nodesWithoughDownstream_));
   if (pair.second == false)
   {
      BPM_TRACE_COUT("Name (" + name + "): Already found in map");

      return false;
   }

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

   // Add the upstream node to the downstream node
   downstreamNode.addUpstreamNode(upstreamNode);

   return true;
}
