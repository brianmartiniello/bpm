

#include <bpm/core/Logger.hxx>


bpm::core::ScopedLog::ScopedLog(const char* out)
   : out_(out)
{
   std::cout << "(cout) BPM " << BPM_LOG_TIMESTAMP << " START " << out_ << std::endl;
}


bpm::core::ScopedLog::ScopedLog(const std::string& out)
   : out_(out.c_str())
{
   std::cout << "(cout) BPM " << BPM_LOG_TIMESTAMP << " START " << out_ << std::endl;
}


bpm::core::ScopedLog::~ScopedLog()
{
   std::cout << "(cout) BPM " << BPM_LOG_TIMESTAMP << " END " << out_ << std::endl;
}
