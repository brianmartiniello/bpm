

#include <algorithm>

#include <bpm/core/leveling_graph/Node.hxx>
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
