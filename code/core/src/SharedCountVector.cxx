

#include <bpm/core/Logger.hxx>
#include <bpm/core/SharedCountVector.hxx>


bpm::core::SharedCountVector::SharedCountVector(size_t size)
   : counts_(size)
{
   BPM_SCOPED_TRACE_COUT("SharedCountVector - size = " + std::to_string(size));

   if (0 == counts_.size())
   {
      throw("SharedCountVector size is 0");
   }

   clear();
}


void bpm::core::SharedCountVector::clear() noexcept
{
   for (auto& count : counts_)
   {
      count.data.store(0, std::memory_order_relaxed);
   }
}


bpm::core::SharedCountVectorLock::SharedCountVectorLock(size_t size)
   : counts_(size)
{
   BPM_SCOPED_TRACE_COUT("SharedCountVectorLock - size = " + std::to_string(size));

   if (0 == counts_.size())
   {
      throw("SharedCountVectorLock size is 0");
   }

   clear();
}


void bpm::core::SharedCountVectorLock::clear() noexcept
{
   for (auto& count : counts_)
   {
      std::lock_guard<std::mutex> lock(count.data.mutex_);
      count.data.count_ = 0;
   }
}
