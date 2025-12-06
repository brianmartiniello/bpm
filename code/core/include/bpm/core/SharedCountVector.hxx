
#ifndef BPM_CORE_SHARED_COUNT_VECTOR_HXX
#define BPM_CORE_SHARED_COUNT_VECTOR_HXX

#include <atomic>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <vector>

#include <bpm/core/CoreTypes.hxx>

namespace bpm
{
   namespace core
   {
      class SharedCountVector
      {
         public:

            explicit SharedCountVector(size_t size);
            
            ~SharedCountVector() = default;

            SharedCountVector(const SharedCountVector&) = delete;

            SharedCountVector& operator=(const SharedCountVector&) = delete;

            SharedCountVector(SharedCountVector&&) = default;

            SharedCountVector& operator=(SharedCountVector&&) = default;

            int64_t increment(size_t index = 0,
                              int64_t value = 1);

            int64_t decrement(size_t index = 0,
                              int64_t value = 1);

            void set(size_t index = 0,
                     int64_t value = 0);

            int64_t get(size_t index = 0) const;

            size_t size() const noexcept;

            void clear() noexcept;

         private:

            std::vector<DataTypePadded<std::atomic<int64_t>>> counts_;

      };

      inline int64_t SharedCountVector::increment(size_t index,
                                                  int64_t value)
      {
         return counts_[index].data.fetch_add(value, std::memory_order_relaxed) + value;
      }

      inline int64_t SharedCountVector::decrement(size_t index,
                                                  int64_t value)
      {
         return counts_[index].data.fetch_sub(value, std::memory_order_relaxed) + value;
      }

      inline void SharedCountVector::set(size_t index,
                                         int64_t value)
      {
         counts_[index].data.store(value, std::memory_order_relaxed);
      }

      inline int64_t SharedCountVector::get(size_t index) const
      {
         return counts_[index].data.load(std::memory_order_relaxed);
      }

      class SharedCountVectorLock
      {
         public:

            explicit SharedCountVectorLock(size_t size);
            
            ~SharedCountVectorLock() = default;

            SharedCountVectorLock(const SharedCountVectorLock&) = delete;

            SharedCountVectorLock& operator=(const SharedCountVectorLock&) = delete;

            SharedCountVectorLock(SharedCountVectorLock&&) = default;

            SharedCountVectorLock& operator=(SharedCountVectorLock&&) = default;

            int64_t increment(size_t index = 0,
                              int64_t value = 1);

            int64_t decrement(size_t index = 0,
                              int64_t value = 1);

            void set(size_t index = 0,
                     int64_t value = 0);

            int64_t get(size_t index = 0) const;

            size_t size() const noexcept;

            void clear() noexcept;

         private:

            struct CountType
            {
               mutable std::mutex mutex_;
               int64_t count_;
            };

            std::vector<DataTypePadded<CountType>> counts_;

      };

      inline int64_t SharedCountVectorLock::increment(size_t index,
                                                      int64_t value)
      {
         auto& count = counts_[index];
         std::lock_guard<std::mutex> lock(count.data.mutex_);
         count.data.count_ += value;
         return count.data.count_;
      }

      inline int64_t SharedCountVectorLock::decrement(size_t index,
                                                      int64_t value)
      {
         auto& count = counts_[index];
         std::lock_guard<std::mutex> lock(count.data.mutex_);
         count.data.count_ -= value;
         return count.data.count_;
      }

      inline void SharedCountVectorLock::set(size_t index,
                                             int64_t value)
      {
         auto& count = counts_[index];
         std::lock_guard<std::mutex> lock(count.data.mutex_);
         count.data.count_ = value;
      }

      inline int64_t SharedCountVectorLock::get(size_t index) const
      {
         auto& count = counts_[index];
         std::lock_guard<std::mutex> lock(count.data.mutex_);
         return count.data.count_;
      }
   }
}

#endif
