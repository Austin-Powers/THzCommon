#include "THzCommon/math/inrange.h"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_InRange : public testing::Test
{};

TEST_F(Math_InRange, ValidRangeChecksOneType)
{
    EXPECT_FALSE(inRange(-11, -10, 10));
    EXPECT_TRUE(inRange(-10, -10, 10));
    EXPECT_TRUE(inRange(0, -10, 10));
    EXPECT_TRUE(inRange(10, -10, 10));
    EXPECT_FALSE(inRange(11, -10, 10));
}

TEST_F(Math_InRange, ValidRangeChecksMultipleTypes)
{
    EXPECT_FALSE(inRange(-11.0, -10, 10));
    EXPECT_TRUE(inRange(-10.0, -10, 10));
    EXPECT_TRUE(inRange(0.0, -10, 10));
    EXPECT_TRUE(inRange(10.0, -10, 10));
    EXPECT_FALSE(inRange(11.0, -10, 10));
}

TEST_F(Math_InRange, SwappingMinGreaterMaxReturnsFalse)
{
    EXPECT_FALSE(inRange(-11.0, 10, -10));
    EXPECT_FALSE(inRange(0.0, 10, -10));
    EXPECT_FALSE(inRange(11.0, 10, -10));
    EXPECT_FALSE(inRange(-11, 10, -10));
    EXPECT_FALSE(inRange(0, 10, -10));
    EXPECT_FALSE(inRange(11, 10, -10));
}

} // namespace Terrahertz::UnitTests
