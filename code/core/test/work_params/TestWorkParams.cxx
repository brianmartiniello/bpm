

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

      T* ptr()
      {
         return &data_;
      };

   private :

      T data_ = {};
};

struct ModParamsA
{
   std::string name_ = "ModA";
   std::size_t value_ = 0;
};

class WorkBase
{
   public :

      WorkBase()
      {
         BPM_TRACE_COUT("");
      };

      virtual ~WorkBase()
      {
         BPM_TRACE_COUT("");
      };

      virtual std::string name() = 0;

      virtual void execute() = 0;
};

class ModWork : public WorkBase
{
   public :

      ModWork()
      {
         BPM_TRACE_COUT("");
      };

      ~ModWork() override
      {
         BPM_TRACE_COUT("");
      };

      void execute() override
      {
         BPM_TRACE_COUT("Work name (" << name()
                        << "): param name (" << params_.ptr()->name_
                        << "), param value (" << params_.ptr()->value_
                        << ")");
      };

   private:

      Data<ModParamsA> params_ = {};
};

class ModWorkA : public ModWork
{
   public :

      ModWorkA()
      {
         BPM_TRACE_COUT("");
      };

      ~ModWorkA()
      {
         BPM_TRACE_COUT("");
      };

      std::string name() override
      {
         return "ModWorkA";
      };
};

///////////////////////////////////////////////
///////////////////////////////////////////////

class TestWorkParams : public ::testing::Test
{
   public:

      TestWorkParams() = default;

      void test()
      {
         ModWorkA modWorkA;
         modWorkA.execute();
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
