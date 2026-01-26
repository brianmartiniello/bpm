
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

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

         private:

            bool setLevel(std::size_t newLevel);

            void updateLevel(std::size_t newLevel,
                             const std::string& originatingNodeName);

            const std::string name_;
            std::size_t level_;
            PtrUSet upstreamNodes_;
            PtrUSet downstreamNodes_;
            std::size_t& maxLevel_;
            bool& circularDependency_;
            const bool hasInputPort_;

            friend class TestNode;
      };

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

            bool connectNodes(const std::string& upstreamName,
                              const std::string& downstreamName);

            const Node::PtrVector& getNodesByLevel()
            {
               return nodesByLevel_;
            }

            void listNodesByLevel();

            std::string toString(const std::string& leadingText = "") const;

         private:

            std::unordered_map<std::string, Node> graph_;
            Node::PtrUSet nodesWithoughUpstream_;
            Node::PtrUSet nodesWithoughDownstream_;
            std::size_t maxLevel_;
            Node::PtrVector nodesByLevel_;
            bool circularDependency_;

            friend class TestGraph;
      };
   }
}

#endif
