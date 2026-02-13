

#include <algorithm>

#include <bpm/core/leveling_graph/Graph.hxx>
#include <bpm/core/Logger.hxx>


bpm::core::Node::Node(const std::string& name,
                      bool hasInputPort,
                      std::size_t& globalLevelPhase,
                      std::size_t& maxLevel,
                      bool& circularDependency)
   : name_(name)
   , level_()
   , levelPhase_(0)
   , globalLevelPhase_(globalLevelPhase)
   , upstreamNodes_()
   , downstreamNodes_()
   , maxLevel_(maxLevel)
   , circularDependency_(circularDependency)
   , hasInputPort_(hasInputPort)
{
}


void bpm::core::Node::addUpstreamNode(Node& /* node */)
{
}


void bpm::core::Node::addDownstreamNode(Node& /* node */)
{
}


void bpm::core::Node::updateLevel(std::size_t /* newLevel */,
                                  const std::string& /* previousNodeName */)
{
}


void bpm::core::Node::updateLevel(std::size_t /* newLevel */)
{
}


void bpm::core::Node::reLevel()
{
}


bool bpm::core::Node::setLevel(std::size_t /* newLevel */)
{
   return false;
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
