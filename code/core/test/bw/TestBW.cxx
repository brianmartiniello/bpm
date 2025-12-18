
#include <chrono>
#include <cmath>

#include <bpm/core/Logger.hxx>
#include <bpm/core/TaskRunner.hxx>

void firstTouchWork(double* data,
                    std::size_t start,
                    std::size_t end)
{
   // BPM_SCOPED_TRACE_COUT("start (" + std::to_string(start) +
   //                       "), end (" + std::to_string(end) + ")");

   for (auto i = start; i < end; ++i)
   {
      data[i] = 1.0;
   }
}

void benchmarkWork(double* data,
                   std::size_t start,
                   std::size_t end,
                   bool computeHeavy)
{
   // BPM_SCOPED_TRACE_COUT("start (" + std::to_string(start) +
   //                       "), end (" + std::to_string(end) +
   //                       "), computeHeavy (" + std::to_string(computeHeavy) + ")");

   if (true == computeHeavy)
   {
      for (auto i = start; i < end; ++i)
      {
         auto val = data[i];
         for (auto j = 0; j < 100; ++j)
         {
            val = std::sqrt(std::sin(val) + 2.0);
         }
         data[i] = val;
      }
   }
   else
   {
      for (auto i = start; i < end; ++i)
      {
         data[i] = data[i] * 0.5 + 0.2;
      }
   }
}

int main(int argc,
         char** argv)
{
   BPM_SCOPED_TRACE_COUT("main");

   try
   {
      BPM_SCOPED_TRACE_COUT("try");

      if (argc < 4)
      {
         BPM_ERROR_COUT("Usage: " + std::string(argv[0]) + " <threads> <bytes> <mode>");
         return EXIT_FAILURE;
      }

      // Input
      const std::size_t numThreads = std::stoi(argv[1]);
      const std::size_t numBytesTotal = std::stoull(argv[2]);
      const std::string mode = std::string(argv[3]);

      BPM_TRACE_COUT("Input - numThreads (" + std::to_string(numThreads) +
                     "), numBytesTotal (" + std::to_string(numBytesTotal) +
                     "), mode (" + mode + ")");

      // Check input validity
      if ((numThreads == 0) ||
          (numBytesTotal == 0))
      {
         BPM_ERROR_COUT("numThreads (" + std::to_string(numThreads) +
                        ") or numBytesTotal (" + std::to_string(numBytesTotal) +
                        ") is 0");
         return EXIT_FAILURE;
      }

      // Derived
      const auto numBytesPerThread = (numBytesTotal / numThreads) +
                                     ((numBytesTotal % numThreads) > 0);
      const auto numElemsPerThread = (numBytesPerThread / sizeof(double)) +
                                     ((numBytesPerThread % sizeof(double)) > 0);
      const auto numElemsTotal = numElemsPerThread * numThreads;
      const auto computeHeavy = (mode == "compute");

      BPM_TRACE_COUT("Derived - numBytesPerThread (" + std::to_string(numBytesPerThread) +
                     "), numElemsPerThread (" + std::to_string(numElemsPerThread) +
                     "), numElemsTotal (" + std::to_string(numElemsTotal) +
                     "), computeHeavy (" + std::to_string(computeHeavy) + ")");

      std::vector<double> data(numElemsTotal);
      bpm::core::TaskRunner taskRunner;
      const auto WAIT_SECONDS = 3U;

      for (auto phase = 0; phase < 2; ++phase)
      {
         for (auto threadIndex = 0; threadIndex < numThreads; ++threadIndex)
         {
            const auto start = threadIndex * numElemsPerThread;
            const auto end = start + numElemsPerThread;
            taskRunner.addTask([&, threadIndex, start, end]
                               ()
                               {
                                  // --- PHASE 1: FIRST TOUCH INITIALIZATION ---
                                  if (0 == phase)
                                  {
                                     firstTouchWork(data.data(),
                                                    start,
                                                    end);
                                  }
                                  // --- PHASE 2: BENCHMARK ---
                                  else // (1 == phase)
                                  {
                                     benchmarkWork(data.data(),
                                                   start,
                                                   end,
                                                   computeHeavy);
                                  }
                               });
         }

         if (1 == phase)
         {
            std::cout << "ELAPSED[" << taskRunner.execute(WAIT_SECONDS) << "]" << std::endl;
         }
      }
   }
   catch (const std::exception& e)
   {
      BPM_ERROR_COUT(std::string("Caught a standard exceptions: ") + e.what());
      return EXIT_FAILURE;
   }
   catch (const std::string& e)
   {
      BPM_ERROR_COUT("Caught a string exception: " + e);
      return EXIT_FAILURE;
   }
   catch (...)
   {
      BPM_ERROR_COUT("Caught an unknown exception");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}