

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>

template<typename T>
class Params
{
   public :

      Params() = default;

      ~Params() = default;

      T* ptr()
      {
         return &params_;
      };

   private :

      T params_ = {};
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
class Work : public WorkBase
{
   public :

      Work()
      {
         BPM_TRACE_COUT("");
      };

      ~Work() override
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

      Params<ParamsT> params_ = {};
};

struct ParamsA
{
   std::string name_ = "A";
   std::size_t value_ = 0;
};

class WorkA : public Work<ParamsA>
{
   public :

      WorkA()
      {
         BPM_TRACE_COUT("");
      };

      ~WorkA()
      {
         BPM_TRACE_COUT("");
      };

      std::string name() override
      {
         return "WorkA";
      };
};

struct ParamsB
{
   std::string name_ = "B";
   std::size_t value_ = 100;
};

class WorkB : public Work<ParamsB>
{
   public :

      WorkB()
      {
         BPM_TRACE_COUT("");
      };

      ~WorkB()
      {
         BPM_TRACE_COUT("");
      };

      std::string name() override
      {
         return "WorkB";
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
         WorkA WorkA;
         WorkA.execute();
         WorkB WorkB;
         WorkB.execute();
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
