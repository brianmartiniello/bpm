
#ifndef BPM_CORE_NODE_CHAIN_HXX
#define BPM_CORE_NODE_CHAIN_HXX

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

            using Vector = std::vector<NodeChain>;
            using PtrVector = std::vector<NodeChain*>;
            using PtrUSet = std::unordered_set<NodeChain*>;

            NodeChain() = default;

            static bool singeNodeChain(Node& node);

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

            std::size_t numNodes() const
            {
               return nodes_.size();
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


      static std::string toString(const NodeChain::Vector& vec,
                                  const std::string& leadingText)
      {
         auto out = leadingText + "vec.size() (" + std::to_string(vec.size()) + ")";
         auto index = -1U;
         for (const auto& node : vec)
         {
            ++index;
            const auto prefix = leadingText + "vec[" + std::to_string(index) + "].";
            out += "\n" + node.toString(prefix);
         }
         return out;
      }


      static std::string toString(const NodeChain::PtrVector& vec,
                                  const std::string& leadingText)
      {
         auto out = leadingText + "vec.size() (" + std::to_string(vec.size()) + ")";
         auto index = -1U;
         for (const auto nodePtr : vec)
         {
            ++index;
            const auto prefix = leadingText + "vec[" + std::to_string(index) + "].";
            out += "\n" + nodePtr->toString(prefix);
         }
         return out;
      }


      static std::string toString(const NodeChain::PtrUSet& uset,
                                  const std::string& leadingText)
      {
         auto out = leadingText + "uset.size() (" + std::to_string(uset.size()) + ")";
         auto index = -1U;
         for (const auto nodePtr : uset)
         {
            ++index;
            const auto prefix = leadingText + "uset[" + std::to_string(index) + "].";
            out += "\n" + nodePtr->toString(prefix);
         }
         return out;
      }


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
   }
}

#endif
