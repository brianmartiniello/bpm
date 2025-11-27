
#ifndef BPM_CORE_LOGGER_HXX
#define BPM_CORE_LOGGER_HXX

#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <system_error>
#include <utility>

namespace bpm
{
   namespace core
   {
      class ScopedLog
      {
         public:

            ScopedLog(const char* out);

            ScopedLog(const std::string& out);

            ~ScopedLog();

         private:

            const std::string out_;

      };
   }
}

// Used for compiler logging
#define Stringize( L ) #L
#define StringizeMacro(macro) Stringize(macro)
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define BPM_COMPILE_LOG __FILE__ "(" $Line ") : "

#define BPM_LOG_PTR(pointer) \
   ([&] \
    { \
       if (nullptr == pointer) \
          return std::string("nullptr"); \
       std::stringstream ss; \
       ss << (void*) pointer; \
       return ss.str(); \
    } ())

#define BPM_LOG_HEX(value, width) \
   ([&] \
    { \
       std::stringstream ss; \
       ss << "0x" << std::hex << std::setw(width) << std::setfill('0') << std::uppercase << value; \
       return ss.str(); \
    } ())
#define BPM_LOG_HEX_64(value) BPM_LOG_HEX(value, 16)
#define BPM_LOG_HEX_56(value) BPM_LOG_HEX(value, 14)
#define BPM_LOG_HEX_48(value) BPM_LOG_HEX(value, 12)
#define BPM_LOG_HEX_40(value) BPM_LOG_HEX(value, 10)
#define BPM_LOG_HEX_32(value) BPM_LOG_HEX(value, 8)
#define BPM_LOG_HEX_24(value) BPM_LOG_HEX(value, 6)
#define BPM_LOG_HEX_16(value) BPM_LOG_HEX(value, 4)
#define BPM_LOG_HEX_8(value) BPM_LOG_HEX((uint16_t) value, 2)
#define BPM_LOG_HEX_FLOAT_64(value) BPM_LOG_HEX_64(*reinterpret_cast<const unsigned int*>(&value))
#define BPM_LOG_HEX_FLOAT_32(value) BPM_LOG_HEX_32(*reinterpret_cast<const unsigned int*>(&value))

#define BPM_PAD_OUT(out, num) std::setfill(' ') << std::setw(num) << out
#define BPM_PAD_OUT_5(out) PAD_OUT(out, 5)
#define BPM_PAD_OUT_10(out) PAD_OUT(out, 10)
#define BPM_PAD_OUT_20(out) PAD_OUT(out, 20)
#define BPM_PAD_OUT_HEX_64(out) PAD_OUT(out, 18)
#define BPM_PAD_OUT_HEX_56(out) PAD_OUT(out, 16)
#define BPM_PAD_OUT_HEX_48(out) PAD_OUT(out, 14)
#define BPM_PAD_OUT_HEX_40(out) PAD_OUT(out, 12)
#define BPM_PAD_OUT_HEX_32(out) PAD_OUT(out, 10)
#define BPM_PAD_OUT_HEX_24(out) PAD_OUT(out, 8)
#define BPM_PAD_OUT_HEX_16(out) PAD_OUT(out, 6)
#define BPM_PAD_OUT_HEX_8(out) PAD_OUT(out, 4)
#define BPM_PAD_OUT_FLOAT_64(out) PAD_OUT(out, 18)
#define BPM_PAD_OUT_FLOAT_32(out) PAD_OUT(out, 10)

#define BPM_LOG_NAME(name) std::string(#name) + std::string(" : ")

#define BPM_LOG_TERNARY(value, trueString, falseString) (value ? #trueString : #falseString) + std::string(" (") + std::to_string(value) + std::string(")")
#define BPM_LOG_TERNARY_NAME(value, trueString, falseString) BPM_LOG_NAME(value) + BPM_LOG_TERNARY(value, trueString, falseString)
#define BPM_LOG_BOOL(value) BPM_LOG_TERNARY(value, TRUE, FALSE)
#define BPM_LOG_BOOL_NAME(value) BPM_LOG_NAME(value) + BPM_LOG_BOOL(value)

   #define BPM_LOG_INPUT_TIMESTAMP(inputTime) \
      ([&inputTime] \
       { \
         auto duration = inputTime.time_since_epoch(); \
         auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration); \
         auto microsec = std::chrono::duration_cast<std::chrono::microseconds>(duration - seconds); \
         std::time_t timeInput = std::chrono::system_clock::to_time_t(inputTime); \
         std::tm localTime = *std::localtime(&timeInput); \
         std::ostringstream oss; \
         oss << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "." \
             << std::setw(6) << std::setfill('0') << microsec.count() << "]"; \
         return oss.str(); \
      } ())
#define BPM_LOG_TIMESTAMP \
   ([] \
    { \
      auto now = std::chrono::system_clock::now(); \
      return BPM_LOG_INPUT_TIMESTAMP(now); \
   } ())

// Determine filename, stripping off full path
#define BPM_LOG_FILENAME (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

// Header for all log statements
#define BPM_LOG_HEADER_NO_TIMESTAMP \
   BPM_LOG_FILENAME + std::string("::") + \
   __FUNCTION__ + std::string("()::") + \
   std::to_string(__LINE__) + std::string(" ")

// Header for all log statements
#define BPM_LOG_HEADER BPM_LOG_TIMESTAMP + " " + BPM_LOG_HEADER_NO_TIMESTAMP

#define BPM_COUT_BASE(out, end) std::cout << "(cout) BPM " << BPM_LOG_HEADER << out << end;
#define BPM_CERR_BASE(out, end) std::cerr << "(cerr) BPM " << BPM_LOG_HEADER << out << end;

#define BPM_TRACE_COUT(out) BPM_COUT_BASE("TRACE " << out, "\n")
#define BPM_INFO_COUT(out) BPM_COUT_BASE("INFO " << out, "\n")
#define BPM_WARN_COUT(out) BPM_COUT_BASE("WARN " << out, "\n")
#define BPM_ERROR_COUT(out) BPM_CERR_BASE("ERROR " << out, "\n")
#define BPM_FATAL_COUT(out) BPM_CERR_BASE("FATAL " << out, "\n")

#define BPM_TRACE_COUT_FLUSH(out) BPM_COUT_BASE("TRACE " << out, std::endl)
#define BPM_INFO_COUT_FLUSH(out) BPM_COUT_BASE("INFO " << out, std::endl)
#define BPM_WARN_COUT_FLUSH(out) BPM_COUT_BASE("WARN " << out, std::endl)
#define BPM_ERROR_COUT_FLUSH(out) BPM_CERR_BASE("ERROR " << out, std::endl)
#define BPM_FATAL_COUT_FLUSH(out) BPM_CERR_BASE("FATAL " << out, std::endl)

#define BPM_SCOPED_TRACE_COUT(out) bpm::core::ScopedLog scopedLog(BPM_LOG_HEADER_NO_TIMESTAMP + "TRACE " + out)

namespace
{
   template<typename T>
   std::string bpmTypeName()
   {
      typedef typename std::remove_reference<T>::type TR;
      std::unique_ptr<char, void(*)(void*)> own(nullptr, std::free);
      std::string r = own != nullptr ? own.get() : typeid(TR).name();
      if (std::is_const<TR>::value)
         r += " const";
      if (std::is_volatile<TR>::value)
         r += " volatile";
      if (std::is_lvalue_reference<TR>::value)
         r += "&";
      if (std::is_rvalue_reference<TR>::value)
         r += "&&";
      return r;
   }
}

#define BPM_LOG_TYPE(var) ::bpmTypeName<decltype(var)>()

#endif
