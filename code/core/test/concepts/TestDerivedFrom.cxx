

#include <concepts>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>

struct Base {};

struct Derived : Base {};

struct Unrelated {};

template<typename D>
concept DerivedFromBase = std::derived_from<D, Base>;

// Using the concept shorthand
template<DerivedFromBase T>
void analyze(T obj)
{
   std::cout << "Success: Type is derived from Base.\n";
}

class TestDerivedFrom : public ::testing::Test
{
   public:

      TestDerivedFrom() = default;

      void test()
      {
         BPM_SCOPED_TRACE_COUT("test");

         Derived d;
         Unrelated u;

         analyze(d); // Compiles perfectly

         // analyze(u);
         // ^ ERROR: constraints not satisfied.
         // The compiler will explicitly tell you that 'Unrelated'
         // does not satisfy 'DerivedFromBase'.
      }

   private:

};


TEST_F(TestDerivedFrom, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
