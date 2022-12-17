#include "THzCommon/math/bilinearInterpolation.h"

#include <THzCommon/math/rectangle.h>
#include <THzCommon/utility/spanhelpers.h>
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

struct Math_BilinearInterpolation : public testing::Test
{
    std::array<double, 16U> const grid{
        1.0, 0.75, 0.5, 0.25, 0.5, 0.25, 1.0, 0.75, 0.0, 0.25, 0.25, 0.25, 1.0, 0.75, 0.5, 0.25};

    Rectangle const dimensions{4U, 4U};

    BilinearInterpolation<double> sut{toSpan<double const>(grid), dimensions};
};

TEST_F(Math_BilinearInterpolation, InterpolateRejectsIllegalCoordinates)
{
    double const           expectedResult{1.337};
    double                 result{expectedResult};
    std::array<double, 6U> coordinates{-0.1, 0.0, 0.5, 1.0, 1.1, std::numeric_limits<double>::quiet_NaN()};
    for (auto const y : coordinates)
    {
        for (auto const x : coordinates)
        {
            if (0.0 <= y && y <= 1.0 && 0.0 <= x && x <= 1.0)
            {
                EXPECT_TRUE(sut.interpolate(x, y, result));
                result = expectedResult;
            }
            else
            {
                EXPECT_FALSE(sut.interpolate(x, y, result));
                EXPECT_EQ(result, expectedResult);
            }
        }
    }
}

/* TEST_F(Math_BilinearInterpolation, InterpolationCorrect)
{
    double result{};

    auto const q = [this](std::int32_t x, std::int32_t y) noexcept -> double {
        return grid[static_cast<std::size_t>(y * dimensions.width) + x];
    };
    auto const size    = 42U;
    auto const epsilon = 1.0e-8;
    for (auto yPos = 0U; yPos < size; ++yPos)
    {
        for (auto xPos = 0U; xPos < size; ++xPos)
        {
            auto const x = static_cast<double>(xPos) / size;
            auto const y = static_cast<double>(yPos) / size;
            EXPECT_TRUE(sut.interpolate(x, y, result));

            auto const x0Int = static_cast<std::int32_t>(x * 4);
            auto const y0Int = static_cast<std::int32_t>(y * 4);
            auto const x1Int = x0Int + 1;
            auto const y1Int = y0Int + 1;
            ASSERT_LT(x1Int, dimensions.width);
            ASSERT_LT(y1Int, dimensions.height);
            auto const x0 = static_cast<double>(x0Int) / dimensions.width;
            auto const x1 = static_cast<double>(x1Int) / dimensions.width;
            auto const y0 = static_cast<double>(y0Int) / dimensions.height;
            auto const y1 = static_cast<double>(y1Int) / dimensions.height;

            auto const xDiff = x1 - x0;
            auto const yDiff = y1 - y0;
            auto const qx0   = (x1 - x) / xDiff;
            auto const qx1   = (x - x0) / xDiff;
            auto const qy0   = (y1 - y) / yDiff;
            auto const qy1   = (y - y0) / yDiff;

            auto const fy0 = (qx0 * q(x0Int, y0Int)) + (qx1 * q(x1Int, y0Int));
            auto const fy1 = (qx0 * q(x0Int, y1Int)) + (qx1 * q(x1Int, y1Int));
            auto const f   = (qy0 * fy0) + (qy1 * fy1);

            ASSERT_NEAR(result, f, epsilon);
        }
    }
} /**/

} // namespace Terrahertz::UnitTests
