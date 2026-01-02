
#ifndef BPM_CORE_THREAD_HXX
#define BPM_CORE_THREAD_HXX

#include <thread>

namespace bpm
{
   namespace core
   {
      class Thread
      {
         public:

            Thread();

            ~Thread();

            void stop();

         private:
     
            void start();

            void execute(std::stop_token stoken);

            std::jthread thread_;

      };
   }
}

#endif
