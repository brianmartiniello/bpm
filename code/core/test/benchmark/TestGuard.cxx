
#include <benchmark/benchmark.h>
#include <vector>

struct Data
{
   int value;
   bool valid;
};

void process(int in,
             int& out)
{
   out = in * 2;
}

// --- 1. Manual Approach ---
static void BM_Manual(benchmark::State& state)
{
   std::vector<Data> input(state.range(0),
                           {10, true});
   std::vector<Data> output(state.range(0));

   for (auto _ : state)
   {
      for (size_t i = 0; i < input.size(); ++i)
      {
         if (!input[i].valid)
         {
            output[i].valid = false;
            continue;
         }

         process(input[i].value,
                 output[i].value);

         output[i].valid = true;
      }

      benchmark::DoNotOptimize(output);
   }
}

// --- 2. Dedicated RAII Class ---
struct DedicatedGuard
{
   Data& item;
   bool committed = false;
   
   explicit DedicatedGuard(Data& i)
      : item(i)
   {
      item.valid = false;
   }
   
   void commit()
   {
      item.valid = true;
      committed = true;
   }
   
   ~DedicatedGuard()
   {
      if (!committed)
      {
         item.valid = false;
      }
   }
};

static void BM_DedicatedRAII(benchmark::State& state)
{
   std::vector<Data> input(state.range(0),
                           {10, true});
   std::vector<Data> output(state.range(0));

   for (auto _ : state)
   {
      for (size_t i = 0; i < input.size(); ++i)
      {
         DedicatedGuard guard(output[i]);

         if (!input[i].valid)
         {
            continue;
         }

         process(input[i].value,
                 output[i].value);

         guard.commit();
      }

      benchmark::DoNotOptimize(output);
   }
}

// --- 3. Generic Lambda Guard ---
template <typename F>
struct GenericGuard
{
   F cleanup;
   bool active = true;
   
   ~GenericGuard()
   {
      if (active)
      {
         cleanup();
      }
   }

   void dismiss()
   {
      active = false;
   }
};

static void BM_GenericLambda(benchmark::State& state)
{
   std::vector<Data> input(state.range(0),
                           {10, true});
   std::vector<Data> output(state.range(0));

   for (auto _ : state)
   {
      for (size_t i = 0; i < input.size(); ++i)
      {
         auto guard = GenericGuard<std::function<void()>>{[&] { output[i].valid = false; }};

         if (!input[i].valid)
         {
            continue;
         }

         process(input[i].value,
                 output[i].value);

         output[i].valid = true;
         guard.dismiss();
      }

      benchmark::DoNotOptimize(output);
   }
}

BENCHMARK(BM_Manual)->Arg(1000);
BENCHMARK(BM_DedicatedRAII)->Arg(1000);
BENCHMARK(BM_GenericLambda)->Arg(1000);
BENCHMARK_MAIN();
