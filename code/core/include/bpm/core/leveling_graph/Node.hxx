
#ifndef BPM_CORE_NODE_HXX
#define BPM_CORE_NODE_HXX

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

            std::size_t numUpstreamNodes() const
            {
               return upstreamNodes_.size();
            }

            void addUpstreamNode(Node& node);

            Node* upstreamNode(std::size_t index = 0) const
            {
               if (index > upstreamNodes_.size()) return nullptr;
               auto iter = upstreamNodes_.begin();
               std::advance(iter, index);
               return *iter;
            }

            bool hasDownstreamNodes() const
            {
               return downstreamNodes_.size() > 0;
            }

            bool hasDownstreamNode(Node& node) const
            {
               return downstreamNodes_.find(&node) != downstreamNodes_.end();
            }

            std::size_t numDownstreamNodes() const
            {
               return downstreamNodes_.size();
            }

            void addDownstreamNode(Node& node);

            Node* downstreamNode(std::size_t index = 0) const
            {
               if (index > downstreamNodes_.size()) return nullptr;
               auto iter = downstreamNodes_.begin();
               std::advance(iter, index);
               return *iter;
            }

            std::string toString(const std::string& leadingText = "") const;

            bool hasInputPort() const
            {
               return hasInputPort_;
            }

            void updateLevel(std::size_t newLevel);

            void reLevel();

            bool visited() const
            {
               return visited_;
            }

            void clearVisited()
            {
               visited_ = false;
            }

            void markVisited()
            {
               visited_ = true;
            }

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
            bool visited_;

            friend class TestNode;
            friend class TestNodeChain;
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


      static std::string toString(const Node::PtrVector& vec,
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


      static std::string toString(const Node::PtrUSet& uset,
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
   }
}

#endif
