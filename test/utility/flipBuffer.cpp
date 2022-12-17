#include "THzCommon/utility/flipBuffer.hpp"

#include "THzCommon/math/rectangle.hpp"
#include "THzCommon/utility/spanhelpers.hpp"

#include <array>
#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Utility_FlipBuffer : public testing::Test
{};

TEST_F(Utility_FlipBuffer, BufferDimensionsMismatch)
{
    std::array<std::int32_t, 10U> buffer{};
    Rectangle const               dimensions{5U, 5U};
    EXPECT_FALSE(flipBufferHorizontally(toSpan<std::int32_t>(buffer), dimensions));
}

TEST_F(Utility_FlipBuffer, FlipHorizontallyWithEvenNumberOfRows)
{
    // 01 02 03 04    13 14 15 16
    // 05 06 07 08 -> 09 10 11 12
    // 09 10 11 12    05 06 07 08
    // 13 14 15 16    01 02 03 04

    std::array<std::int32_t, 16U> buffer{};

    std::int32_t i = 0;
    for (auto &entry : buffer)
    {
        entry = ++i;
    }
    Rectangle const dimensions{4U, 4U};
    EXPECT_TRUE(flipBufferHorizontally(toSpan<std::int32_t>(buffer), dimensions));
    i      = 13;
    auto x = 0;
    for (auto const &entry : buffer)
    {
        ASSERT_EQ(entry, i);
        ++i;
        ++x;
        if (x == 4)
        {
            x = 0;
            i -= 8;
        }
    }
}

TEST_F(Utility_FlipBuffer, FlipHorizontallyWithOddNumberOfRows)
{
    // 01 02 03 04 05    21 22 23 24 25
    // 06 07 08 09 10    16 17 18 19 20
    // 11 12 13 14 15 -> 11 12 13 14 15
    // 16 17 18 19 20    06 07 08 09 10
    // 21 22 23 24 25    01 02 03 04 05

    std::array<std::int32_t, 25U> buffer{};

    std::int32_t i = 0;
    for (auto &entry : buffer)
    {
        entry = ++i;
    }
    Rectangle const dimensions{5U, 5U};
    EXPECT_TRUE(flipBufferHorizontally(toSpan<std::int32_t>(buffer), dimensions));
    i      = 21;
    auto x = 0;
    for (auto const &entry : buffer)
    {
        ASSERT_EQ(entry, i);
        ++i;
        ++x;
        if (x == 5)
        {
            x = 0;
            i -= 10;
        }
    }
}

} // namespace Terrahertz::UnitTests
