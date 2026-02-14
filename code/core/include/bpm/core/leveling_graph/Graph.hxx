
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <bpm/core/leveling_graph/NodeChain.hxx>

namespace bpm
{
   namespace core
   {
      class Graph
      {
         public:

            Graph() = default;

            bool addNode(const std::string& name,
                         bool hasInputPort = false);

            void assignMaxLevel();

            bool circularDependency() const
            {
               return circularDependency_;
            }

            bool connectNodes(const std::string& downstreamName,
                              const std::string& upstreamName);

            const Node::PtrVector& getNodesByLevel()
            {
               return nodesByLevel_;
            }

            void sortNodesByLevel();

            void reLevel();

            std::string toString(const std::string& leadingText = "") const;

         private:

            std::unordered_map<std::string, Node> graph_;
            Node::PtrUSet nodesWithoughUpstream_;
            Node::PtrUSet nodesWithoughDownstream_;
            std::size_t globalLevelPhase_;
            std::size_t maxLevel_;
            Node::PtrVector nodesByLevel_;
            bool circularDependency_;

            friend class TestGraph;
      };
   }
}

#endif
