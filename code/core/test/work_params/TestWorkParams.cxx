

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

// ** Member variable check **
// MemberPtr: The pointer to the member variable of the type (e.g., &User::name)
// ExpectedType: What the member should be (e.g., std::string)
template <typename ParamsT,
          auto MemberPtr,
          typename ExpectedType>
concept ParamsHasdMember = 
    std::is_member_object_pointer_v<decltype(MemberPtr)> && 
    std::convertible_to<std::invoke_result_t<decltype(MemberPtr), ParamsT>, ExpectedType>;

template <typename ParamsT>
concept ValidParamsVars = 
    ParamsHasdMember<ParamsT, &ParamsT::name_, std::string> &&
    ParamsHasdMember<ParamsT, &ParamsT::value_, std::size_t>;

// ** Params member method check **
// Check for:
// class MyParams
// {
//    ReturnType methodName(const& std::string& workName);
// }
#define DEFINE_PARAMS_HAS_METHOD(methodName) \
template <typename ParamsT, \
          typename ReturnType> \
concept ParamsHasMethod_##methodName = requires(ParamsT params, const std::string& workName) \
{ \
   { params.methodName(workName) } -> std::same_as<ReturnType>; \
}

DEFINE_PARAMS_HAS_METHOD(name);
DEFINE_PARAMS_HAS_METHOD(value);
template <typename ParamsT>
concept ValidParams =
   ParamsHasMethod_name<ParamsT, std::string> &&
   ParamsHasMethod_value<ParamsT, std::size_t>;

template<ValidParams ParamsT>
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
                        << "): param name (" << params_.ptr()->name(name())
                        << "), param value (" << params_.ptr()->value(name())
                        << ")");
      };

   private:

      Params<ParamsT> params_ = {};
};

struct ParamsA
{
   std::string name(const std::string& /* s */) { return "A"; };
   std::size_t value(const std::string& /* s */) { return 1; };
};

class WorkA final : public Work<ParamsA>
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
   std::string name(const std::string& /* s */) { return "B"; };
   std::size_t value(const std::string& /* s */) { return 100; };
   std::size_t unused(const std::string& /* s */) { return 321; };
};

class WorkB final : public Work<ParamsB>
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

struct ParamsC
{
   std::string name(const std::string& /* s */) { return "C"; };
   std::size_t value(const std::string& /* s */) { return 1000; };
   std::string unused(const std::string& /* s */) { return "654"; };
};

#define CREATE_WORK(WorkName, ParamsNam) \
class WorkName final : public Work<ParamsNam> \
{ \
   public : \
      WorkName() { BPM_TRACE_COUT(""); }; \
      ~WorkName() { BPM_TRACE_COUT(""); }; \
      std::string name() override { return #WorkName; }; \
};

CREATE_WORK(WorkC, ParamsC)

///////////////////////////////////////////////
///////////////////////////////////////////////

class TestWorkParams : public ::testing::Test
{
   public:

      TestWorkParams() = default;

      void test()
      {
         WorkA workA;
         workA.execute();
         WorkB workB;
         workB.execute();
         WorkC workC;
         workC.execute();
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
