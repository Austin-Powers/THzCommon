#include "THzCommon/math/bilinearInterpolation.hpp"

#include <THzCommon/math/rectangle.hpp>
#include <THzCommon/utility/spanhelpers.hpp>
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

TEST_F(Math_BilinearInterpolation, InterpolationCorrect)
{
    double result{};

    auto const lowerIndex = [](double const v, std::uint32_t const dimension) noexcept -> std::uint32_t {
        auto const dim = dimension - 1U;
        auto const v0  = static_cast<std::uint32_t>(v * dim);
        return (v0 == dim) ? v0 - 1U : v0;
    };
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

            auto const x0Int = lowerIndex(x, dimensions.width);
            auto const y0Int = lowerIndex(y, dimensions.height);
            auto const x1Int = x0Int + 1;
            auto const y1Int = y0Int + 1;

            // we need to subtract one because we are addressing the areas between the points not the points themselves
            auto const x0 = static_cast<double>(x0Int) / (dimensions.width - 1U);
            auto const x1 = static_cast<double>(x1Int) / (dimensions.width - 1U);
            auto const y0 = static_cast<double>(y0Int) / (dimensions.height - 1U);
            auto const y1 = static_cast<double>(y1Int) / (dimensions.height - 1U);

            auto const q00 = q(x0Int, y0Int);
            auto const q10 = q(x1Int, y0Int);
            auto const q01 = q(x0Int, y1Int);
            auto const q11 = q(x1Int, y1Int);

            auto const xDiff = x1 - x0;
            auto const yDiff = y1 - y0;

            auto const qx0 = (x1 - x) / xDiff;
            auto const qx1 = (x - x0) / xDiff;
            auto const qy0 = (y1 - y) / yDiff;
            auto const qy1 = (y - y0) / yDiff;

            auto const fy0 = (qx0 * q00) + (qx1 * q10);
            auto const fy1 = (qx0 * q01) + (qx1 * q11);
            auto const f   = (qy0 * fy0) + (qy1 * fy1);

            EXPECT_NEAR(result, f, epsilon);
        }
    }
}

} // namespace Terrahertz::UnitTests
