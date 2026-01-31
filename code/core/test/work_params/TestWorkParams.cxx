

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

template<typename ParamsT>
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

      Data<ParamsT> params_ = {};
};

struct ModParamsA
{
   std::string name_ = "ModA";
   std::size_t value_ = 0;
};

class ModWorkA : public ModWork<ModParamsA>
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

struct ModParamsB
{
   std::string name_ = "ModB";
   std::size_t value_ = 100;
};

class ModWorkB : public ModWork<ModParamsB>
{
   public :

      ModWorkB()
      {
         BPM_TRACE_COUT("");
      };

      ~ModWorkB()
      {
         BPM_TRACE_COUT("");
      };

      std::string name() override
      {
         return "ModWorkB";
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
         ModWorkB modWorkB;
         modWorkB.execute();
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
