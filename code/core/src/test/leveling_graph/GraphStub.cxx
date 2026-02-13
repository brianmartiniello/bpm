

#include <algorithm>

#include <bpm/core/leveling_graph/Graph.hxx>
#include <bpm/core/Logger.hxx>


bool bpm::core::NodeChain::addNode(Node& node)
{
   return false;
}


bool bpm::core::NodeChain::sortNodesByLevel()
{
   return false;
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


bool bpm::core::Graph::addNode(const std::string& /* name */,
                               bool /* hasInputPort */)
{
   return false;
}


bool bpm::core::Graph::connectNodes(const std::string& /* upstreamName */,
                                    const std::string& /* downstreamName */)
{
   return false;
}


void bpm::core::Graph::sortNodesByLevel()
{
}


void bpm::core::Graph::assignMaxLevel()
{
}


void bpm::core::Graph::reLevel()
{
}
