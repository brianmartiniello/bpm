

#include <bpm/core/Logger.hxx>


bpm::core::ScopedLog::ScopedLog(const char* out)
   : out_(out)
{
   BPM_TRACE_COUT("START - " << out_);
}


bpm::core::ScopedLog::ScopedLog(const std::string& out)
   : out_(out.c_str())
{
   BPM_TRACE_COUT("START - " << out_);
}


bpm::core::ScopedLog::~ScopedLog()
{
   BPM_TRACE_COUT("END - " << out_);
}
