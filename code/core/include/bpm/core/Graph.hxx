
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace bpm
{
   namespace core
   {
      struct Node
      {
         public:

            using PtrVector = std::vector<Node*>;
            using PtrUSet = std::unordered_set<Node*>;

            Node(const std::string& name,
                 bool hasInputPort,
                 std::size_t& globalLevelPhase,
                 std::size_t& maxLevel,
                 bool& circularDependency);
            
            const std::string& name() const
            {
               return name_;
            }
            
            std::size_t level() const
            {
               return level_;
            }

            bool hasUpstreamNodes() const
            {
               return upstreamNodes_.size() > 0;
            }

            bool hasUpstreamNode(Node& node) const
            {
               return upstreamNodes_.find(&node) != upstreamNodes_.end();
            }

            void addUpstreamNode(Node& node);

            bool hasDownstreamNodes() const
            {
               return downstreamNodes_.size() > 0;
            }

            bool hasDownstreamNode(Node& node) const
            {
               return downstreamNodes_.find(&node) != downstreamNodes_.end();
            }

            void addDownstreamNode(Node& node);

            std::string toString(const std::string& leadingText = "") const;

            bool hasInputPort() const
            {
               return hasInputPort_;
            }

            void updateLevel(std::size_t newLevel);

            void reLevel();

         private:

            bool setLevel(std::size_t newLevel);

            void updateLevel(std::size_t newLevel,
                             const std::string& previousNodeName);

            const std::string name_;
            std::size_t level_;
            std::size_t levelPhase_;
            std::size_t& globalLevelPhase_;
            PtrUSet upstreamNodes_;
            PtrUSet downstreamNodes_;
            std::size_t& maxLevel_;
            bool& circularDependency_;
            const bool hasInputPort_;

            friend class TestNode;
            friend class TestGraph;
      };


      struct NodePtrLevelCompare
      {
         bool operator()(const Node* a,
                         const Node* b) const
         {
            // Descending order
            return a->level() > b->level();
         }
      };


      using NodePtrLevelSet = std::multiset<Node*, NodePtrLevelCompare>;


      static std::string toString(const NodePtrLevelSet& set,
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


      struct NodeChain
      {
         public:

            NodeChain() = default;
            
            std::size_t level() const
            {
               return level_;
            }

            std::string toString(const std::string& leadingText = "") const;

         private:

            std::size_t level_;

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

            const NodePtrLevelSet& getNodesByLevel()
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
            NodePtrLevelSet nodesByLevel_;
            bool circularDependency_;

            friend class TestGraph;
      };
   }
}

#endif
