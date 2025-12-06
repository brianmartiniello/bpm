
#ifndef BPM_CORE_TYPES_HXX
#define BPM_CORE_TYPES_HXX

#include <string>

namespace bpm
{
   namespace core
   {
      static constexpr size_t CACHE_LINE_SIZE =
      #ifdef __cpp_lib_hardware_interference_size
              std::hardware_destructive_interference_size;
      #else
              64;
      #endif   


      template<typename DataType>
      struct DataTypePadded
      {
         static constexpr uint32_t REMAINDER_SIZE = sizeof(DataType) % CACHE_LINE_SIZE; //!< Remainder compared to cache size
         static constexpr uint32_t PAD_SIZE = CACHE_LINE_SIZE - REMAINDER_SIZE; //!< Pad size to increase to cache line

         alignas(CACHE_LINE_SIZE) DataType data; //!< Actual data to use
         std::byte pad[PAD_SIZE]; //!< Pad size to align to cache
      };
   }
}

#endif
