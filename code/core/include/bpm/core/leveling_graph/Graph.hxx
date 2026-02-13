
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <bpm/core/leveling_graph/Node.hxx>

namespace bpm
{
   namespace core
   {
      struct NodeChain
      {
         public:

            using PtrVector = std::vector<NodeChain*>;
            using PtrUSet = std::unordered_set<NodeChain*>;

            NodeChain() = default;

            bool addNode(Node& node);

            const Node::PtrVector& getNodes()
            {
               return nodes_;
            }

            bool sortNodesByLevel();
            
            std::size_t level() const
            {
               return level_;
            }

            bool singleNodeChain() const
            {
               return singleNodeChain_;
            }

            std::string toString(const std::string& leadingText = "") const;

         private:

            bool verifyContinuity() const;

            std::size_t level_;
            Node::PtrVector nodes_;
            bool singleNodeChain_;

            friend class TestNodeChain;
            friend class TestGraph;
      };


      struct NodeChainPtrLevelCompare
      {
         bool operator()(const NodeChain* a,
                         const NodeChain* b) const
         {
            // Descending order
            return a->level() > b->level();
         }
      };


      using NodeChainPtrLevelSet = std::multiset<NodeChain*, NodeChainPtrLevelCompare>;


      static std::string toString(const NodeChainPtrLevelSet& set,
                                  const std::string& leadingText)
      {
         auto out = leadingText + "set.size() (" + std::to_string(set.size()) + ")";
         auto index = -1U;
         for (const auto nodePtr : set)
         {
            ++index;
            const auto prefix = leadingText + "set[" + std::to_string(index) + "].";
            out += "\n" + nodePtr->toString(prefix);
         }
         return out;
      }


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
