#include "THzCommon/math/inrange.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct MathInRange : public testing::Test
{};

TEST_F(MathInRange, ValidRangeChecksOneType)
{
    EXPECT_FALSE(inRange(-11, -10, 10));
    EXPECT_TRUE(inRange(-10, -10, 10));
    EXPECT_TRUE(inRange(0, -10, 10));
    EXPECT_TRUE(inRange(10, -10, 10));
    EXPECT_FALSE(inRange(11, -10, 10));
}

TEST_F(MathInRange, ValidRangeChecksMultipleTypes)
{
    EXPECT_FALSE(inRange(-11.0, -10, 10));
    EXPECT_TRUE(inRange(-10.0, -10, 10));
    EXPECT_TRUE(inRange(0.0, -10, 10));
    EXPECT_TRUE(inRange(10.0, -10, 10));
    EXPECT_FALSE(inRange(11.0, -10, 10));
}

TEST_F(MathInRange, SwappingMinGreaterMaxReturnsFalse)
{
    EXPECT_FALSE(inRange(-11.0, 10, -10));
    EXPECT_FALSE(inRange(0.0, 10, -10));
    EXPECT_FALSE(inRange(11.0, 10, -10));
    EXPECT_FALSE(inRange(-11, 10, -10));
    EXPECT_FALSE(inRange(0, 10, -10));
    EXPECT_FALSE(inRange(11, 10, -10));
}

} // namespace Terrahertz::UnitTests
