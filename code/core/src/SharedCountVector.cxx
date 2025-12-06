

#include <bpm/core/SharedCountVector.hxx>


bpm::core::SharedCountVector::SharedCountVector(size_t size)
   : counts_(size)
{
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
