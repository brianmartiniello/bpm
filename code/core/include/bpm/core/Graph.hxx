
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
               const std::string name;
               std::size_t level;
               std::vector<Node*> upstreamNodes;
               std::vector<Node*> downstreamNodes;

               Node(const std::string& name);

               bool hasUpstreamNodes() const
               {
                  return upstreamNodes.size() > 0;
               }

               bool hasDownstreamNodes() const
               {
                  return downstreamNodes.size() > 0;
               }

               std::string toString(const std::string& leadingText = "") const;
            };

            std::unordered_map<std::string, Node> graph_;
            std::unordered_map<std::string, Node*> nodesWithoughUpstream_;
            std::unordered_map<std::string, Node*> nodesWithoughDownstream_;

            friend class TestGraph;
      };
   }
}

#endif
