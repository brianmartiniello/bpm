
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <string>
#include <unordered_map>
#include <vector>

namespace bpm
{
   namespace core
   {
      struct Node
      {
         public:

            using PtrVector = std::vector<Node*>;

            const std::string name_;
            std::size_t level_;
            PtrVector upstreamNodes_;
            PtrVector downstreamNodes_;
            PtrVector& nodesWithoughUpstream_;
            PtrVector& nodesWithoughDownstream_;
            std::size_t& maxLevel_;
            bool& circularDependency_;

            Node(const std::string& name,
                 PtrVector& nodesWithoughUpstream,
                 PtrVector& nodesWithoughDownstream,
                 std::size_t& maxLevel,
                 bool& circularDependency);

            bool hasUpstreamNodes() const
            {
               return upstreamNodes_.size() > 0;
            }

            void addUpstreamNode(Node& node);

            bool hasDownstreamNodes() const
            {
               return downstreamNodes_.size() > 0;
            }

            void addDownstreamNode(Node& node);

            std::string toString(const std::string& leadingText = "") const;

         private:

            void updateLevel(std::size_t newLevel,
                             const std::string& originatingNodeName);

      };

      class Graph
      {
         public:

            Graph() = default;

            bool addNode(const std::string& name);

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
            Node::PtrVector nodesWithoughUpstream_;
            Node::PtrVector nodesWithoughDownstream_;
            std::size_t maxLevel_;
            Node::PtrVector nodesByLevel_;
            bool circularDependency_;

            friend class TestGraph;
      };
   }
}

#endif
