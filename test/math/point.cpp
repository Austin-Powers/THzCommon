#include "THzCommon/math/point.hpp"

#include <cmath>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct MathPoint : public testing::Test
{};

TEST_F(MathPoint, Equals)
{
    Point const a{0, 0};
    Point const b{1, 1};
    Point const c{1, 1};
    EXPECT_EQ(a, a);
    EXPECT_EQ(b, b);
    EXPECT_NE(a, b);
    EXPECT_EQ(b, c);
}

TEST_F(MathPoint, Addition)
{
    Point const a{0, 0};
    Point       b{-2, 2};

    auto const c = a + b;
    EXPECT_EQ(b, c);

    b += c;
    Point const d{-4, 4};
    EXPECT_EQ(b, d);
}

TEST_F(MathPoint, Subtraction)
{
    Point const a{0, 0};
    Point const b{-2, 2};

    {
        Point expected{2, -2};

        auto const c = a - b;
        EXPECT_EQ(expected, c);
    }
    {
        Point c{4, -8};
        c -= b;
        Point const expected{6, -10};
        EXPECT_EQ(expected, c);
    }
}

TEST_F(MathPoint, Multiplication)
{
    Point a{-2, 2};

    auto const  b = a * 2;
    auto const  c = a * -2;
    Point const expectedB{-4, 4};
    Point const expectedC{4, -4};
    EXPECT_EQ(expectedB, b);
    EXPECT_EQ(expectedC, c);

    a *= 3;
    Point const expectedA0{-6, 6};
    EXPECT_EQ(expectedA0, a);
    Point const expectedA1{6, -6};
    a *= -1;
    EXPECT_EQ(expectedA1, a);
}

TEST_F(MathPoint, Division)
{
    Point a{16, -16};

    auto const  b = a / 2;
    auto const  c = a / -2;
    Point const expectedB{8, -8};
    Point const expectedC{-8, 8};
    EXPECT_EQ(expectedB, b);
    EXPECT_EQ(expectedC, c);

    a /= 4;
    Point const expectedA0{4, -4};
    EXPECT_EQ(expectedA0, a);
    Point const expectedA1{-4, 4};
    a /= -1;
    EXPECT_EQ(expectedA1, a);
}

TEST_F(MathPoint, Distance)
{
    auto const epsilon{1e-16};

    Point const a{1, 1};
    Point const b{-2, -2};
    EXPECT_NEAR(a.distance(b), std::sqrt(18), epsilon);
}

TEST_F(MathPoint, Direction)
{
    Point const center{0, 0};
    for (auto x = -1; x < 2; ++x)
    {
        for (auto y = -1; y < 2; ++y)
        {
            Point const other{x, y};
            EXPECT_EQ(center.direction(other), std::atan2(other.y - center.y, other.x - center.x));
        }
    }
}

TEST_F(MathPoint, AngularShift)
{
    Point center{1, 1};
    for (auto x = -5; x < 6; ++x)
    {
        for (auto y = -5; y < 6; ++y)
        {
            Point const other{x, y};
            auto const  direction = center.direction(other);
            auto const  distance  = center.distance(other);
            auto const  shifted   = center.angularShift(direction, distance);
            EXPECT_EQ(shifted.x, other.x) << "X: " << x << " Y: " << y;
            EXPECT_EQ(shifted.y, other.y) << "X: " << x << " Y: " << y;
        }
    }
}

} // namespace Terrahertz::UnitTests
