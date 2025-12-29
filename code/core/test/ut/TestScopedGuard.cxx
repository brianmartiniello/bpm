

#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <bpm/core/Logger.hxx>
#include <bpm/core/ScopedGuard.hxx>


class TestScopedGuard : public ::testing::Test
{
   public:

      TestScopedGuard() = default;

      void test()
      {
         {
            BPM_SCOPED_TRACE_COUT("Active");

            auto count = 0U;

            {
               bpm::core::ScopeGuard scopeGuard([&](){ ++count; });
               (void) scopeGuard;
            }

            EXPECT_EQ(count, 1);
         }
         
         {
            BPM_SCOPED_TRACE_COUT("Dismiss");

            auto count = 0U;

            {
               bpm::core::ScopeGuard scopeGuard([&](){ ++count; });
               scopeGuard.dismiss();
            }

            EXPECT_EQ(count, 0);
         }
         
         {
            BPM_SCOPED_TRACE_COUT("makeScopedGuard");

            auto count = 0U;

            auto cleanup = [&](){ ++count; };

            {
               auto scopedGuard = bpm::core::makeScopedGuard(cleanup);
               (void) scopedGuard;
            }

            EXPECT_EQ(count, 1);
         }
      }

   private:

};


TEST_F(TestScopedGuard, test)
{
   test();
}


int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
