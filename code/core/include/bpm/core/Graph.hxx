
#ifndef BPM_CORE_GRAPH_HXX
#define BPM_CORE_GRAPH_HXX

#include <string>
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

         private:

            struct Node
            {
               std::string name;
               std::size_t level;
               std::vector<Node*> upstreamNodes;
               std::vector<Node*> downstreamNodes;

               std::string toString() const;
            };

            std::vector<Node> nodeVec_;

            friend class TestGraph;
      };
   }
}

#endif
