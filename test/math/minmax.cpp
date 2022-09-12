#include "THzCommon/math/minmax.h"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_MinMax : public testing::Test
{};

TEST_F(Math_MinMax, Minimum)
{
    EXPECT_EQ(minimum(1.0, 10.0), 1.0);
    EXPECT_EQ(minimum(1, -10.0), -10);
    EXPECT_EQ(minimum(1.0, -4.0, -3, 7), -4.0);
}

TEST_F(Math_MinMax, Maximum)
{
    EXPECT_EQ(maximum(1.0, 10.0), 10.0);
    EXPECT_EQ(maximum(3, -10.0), 3);
    EXPECT_EQ(maximum(1.0, -4.0, -3, 7), 7.0);
}

TEST_F(Math_MinMax, Clamp)
{
    EXPECT_EQ(clamp(-0.1, 0, 10), 0.0);
    EXPECT_EQ(clamp(0, 0, 10), 0);
    EXPECT_EQ(clamp(1, 0, 10), 1);
    EXPECT_EQ(clamp(5, 2, 8), 5);
    EXPECT_EQ(clamp(-5.0, -8.0, -2.0), -5.0);
    EXPECT_EQ(clamp(-2, -8, -2), -2);
    EXPECT_EQ(clamp(10.1, 1, 10), 10.0);
    EXPECT_EQ(clamp(0, -10, -1), -1);
}

TEST_F(Math_MinMax, ClampReturnsMaxIfMinIsGreaterThanMax) { EXPECT_EQ(clamp(0, 10, 5), 5); }

} // namespace Terrahertz::UnitTests
