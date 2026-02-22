

#include <algorithm>

#include <bpm/core/leveling_graph/NodeChain.hxx>
#include <bpm/core/Logger.hxx>


/* static */ bool bpm::core::NodeChain::isSingleNodeChain(Node& node)
{
   return false;
}


bool bpm::core::NodeChain::addNode(Node& /* node */)
{
   return false;
}


bool bpm::core::NodeChain::addNodeAsSingle(Node& /* node */)
{
   return false;
}


bool bpm::core::NodeChain::sortNodesByLevel()
{
   return false;
}


bool bpm::core::NodeChain::verifyContinuity() const
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
