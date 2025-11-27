

#include <bpm/core/Graph.hxx>
#include <bpm/core/Logger.hxx>


bpm::core::Graph::Node::Node(const std::string& name)
   : name(name)
   , level()
   , upstreamNodes()
   , downstreamNodes()
{
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

   auto pair = graph_.emplace(name, Node(name));
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

   return true;
}
