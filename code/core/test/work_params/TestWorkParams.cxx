

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>

template<typename T>
class Data
{
   public :
      Data() = default;
      ~Data() = default;
      T* ptr() { return &data_; };
   private :
      T data_ = {};
};

struct ModParams
{
   std::size_t scaleFactor_;
   std::size_t offset_;
};

class WorkBase
{
   public :
      WorkBase() = default;
      virtual ~WorkBase() = default;
      virtual std::string name() = 0;
      virtual void initialize() = 0;
      virtual void execute() = 0;
};

class ModWork : public WorkBase
{
   public :
      ModWork() { BPM_TRACE_COUT(""); };
      ~ModWork() override { BPM_TRACE_COUT(""); };
      std::string name() override { return "ModWork"; };
      void initialize() override { BPM_TRACE_COUT(""); };
      void execute() override { BPM_TRACE_COUT(""); };
   private:
      Data<ModParams> params_ = {};
};

///////////////////////////////////////////////
///////////////////////////////////////////////

class TestWorkParams : public ::testing::Test
{
   public:

      TestWorkParams() = default;

      void test()
      {
         ModWork modWork;
         modWork.initialize();
         modWork.execute();
      }

   private:

};


TEST_F(TestWorkParams, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
