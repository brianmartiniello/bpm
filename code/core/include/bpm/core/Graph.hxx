
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <string>
#include <unordered_map>
#include <vector>

namespace bpm
{
   namespace core
   {
      class Graph
      {
         public:

            Graph() = default;

            bool addNode(const std::string& name);

            bool connectNodes(const std::string& upstreamName,
                              const std::string& downstreamName);

            std::string toString(const std::string& leadingText = "") const;

         private:

            struct Node
            {
               const std::string name_;
               std::size_t level_;
               std::vector<Node*> upstreamNodes_;
               std::vector<Node*> downstreamNodes_;
               std::vector<Node*>& nodesWithoughUpstream_;
               std::vector<Node*>& nodesWithoughDownstream_;

               Node(const std::string& name,
                    std::vector<Node*>& nodesWithoughUpstream,
                    std::vector<Node*>& nodesWithoughDownstream);

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

               void updateLevel(std::size_t newLevel);

               std::string toString(const std::string& leadingText = "") const;
            };

            std::unordered_map<std::string, Node> graph_;
            std::vector<Node*> nodesWithoughUpstream_;
            std::vector<Node*> nodesWithoughDownstream_;

            friend class TestGraph;
      };
   }
}

#endif
