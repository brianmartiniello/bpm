

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>

struct Unrelated {};

///////////////////////////////////////////////
///////////////////////////////////////////////

struct Base {};

struct Derived : Base {};

template<typename D>
concept DerivedFromBase = std::derived_from<D, Base>;

// Using the concept shorthand
template<DerivedFromBase T>
void analyze(T obj)
{
   std::cout << "Success: Type is derived from Base.\n";
}

///////////////////////////////////////////////
///////////////////////////////////////////////

// 1. Your templated base
template<typename T>
struct BaseT {};

struct DerivedT : BaseT<int> {};

// 2. Create a helper namespace to hide the detection logic
namespace detail
{
   template<typename T>
   void is_derived_from_baseT(const BaseT<T>&);
}

// Why it works without a body - When the compiler evaluates a concept, it performs Substitution and Overload Resolution:
// A. Substitution: It looks at detail::is_base_of_templated_base(d).
// B. Deduction: It asks, "Can I find a version of this function where d fits the arguments?"
// C. Validation: If d is a DerivedT (which inherits from BaseT<int>), the compiler realizes it can treat d as a BaseT<int>&. It successfully deduces that T = int.
// D. Discard: Once the compiler proves that a valid function call could be formed, it is satisfied. It never generates a call to that address, so the linker never looks for a function body.

// Key Properties of this Pattern
// A. No Runtime Overhead: Since the function is never called, it adds zero bytes to your final executable.
// B. Compile-Time Only: This exists purely to "trick" the compiler into performing template argument deduction for us.

// 3. Define the concept by checking if a call to that function is valid
template<typename D>
concept DerivedFromTemplatedBaseT = requires(D d)
{
   detail::is_derived_from_baseT(d);
};

// // 2A. A helper trait to check for inheritance from Base<T>
// template <typename T>
// struct is_derived_from_baseT
// {
//    static constexpr bool value = false;
// };

// // 2B. This specialization "unwraps" the Base<U> inheritance
// template <typename U>
// struct is_derived_from_baseT<BaseT<U>>
// {
//    static constexpr bool value = true;
// };

// // 3. Define the concept using a "requires" expression to check 
// //    if the type can be converted to its own Base version
// template<typename D>
// concept DerivedFromTemplatedBaseT = requires(D d)
// {
//    []<typename T>(BaseT<T>&){}(d);
// };

template<DerivedFromTemplatedBaseT T>
void analyzeT(T obj)
{
   // Works for any BaseT<int>, BaseT<double>, etc.
   std::cout << "Success: Type is derived from BaseT.\n";
}

///////////////////////////////////////////////
///////////////////////////////////////////////

class TestDerivedFrom : public ::testing::Test
{
   public:

      TestDerivedFrom() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");

         Unrelated u;
         // analyze(u);
         // ^ ERROR: constraints not satisfied.
         // The compiler will explicitly tell you that 'Unrelated'
         // does not satisfy 'DerivedFromBase'.
         (void) u;

         Base b;
         analyze(b); // Success

         Derived d;
         analyze(d); // Success
      }

      void testT()
      {
         BPM_SCOPED_TRACE_COUT("testT");

         Unrelated u;
         // analyzeT(u);
         // ^ ERROR: constraints not satisfied.
         // The compiler will explicitly tell you that 'Unrelated'
         // does not satisfy 'DerivedFromBase'.
         (void) u;

         BaseT<double> b;
         analyzeT(b); // Success

         DerivedT d; // Inherits Base<int>   
         analyzeT(d); // Success
      }

   private:

};


TEST_F(TestDerivedFrom, test)
{
   test();
}


TEST_F(TestDerivedFrom, testT)
{
   testT();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
