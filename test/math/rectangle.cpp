#include "THzCommon/math/rectangle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_Rectangle : public testing::Test
{
    Rectangle const r0{};
};

TEST_F(Math_Rectangle, ConstructionUsingWidthAndHeight)
{
    Rectangle rwh{1024U, 768U};
    EXPECT_EQ(rwh.upperLeftPoint.x, 0);
    EXPECT_EQ(rwh.upperLeftPoint.y, 0);
    EXPECT_EQ(rwh.width, 1024U);
    EXPECT_EQ(rwh.height, 768U);

    Rectangle rpwh{Point{-18, 22}, 13U, 37U};
    EXPECT_EQ(rpwh.upperLeftPoint.x, -18);
    EXPECT_EQ(rpwh.upperLeftPoint.y, 22);
    EXPECT_EQ(rpwh.width, 13U);
    EXPECT_EQ(rpwh.height, 37U);
}

TEST_F(Math_Rectangle, Comparison)
{
    {
        Rectangle a{20U, 16U};
        Rectangle b{20U, 16U};
        Rectangle c{21U, 16U};
        Rectangle d{20U, 15U};

        EXPECT_EQ(a, a);
        EXPECT_EQ(a, b);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
    }
    {
        Point pA{1, 2};
        Point pB{-3, 4};

        Rectangle a{pA, 20U, 12U};
        Rectangle b{pA, 20U, 12U};
        Rectangle c{pB, 20U, 12U};
        Rectangle d{pB, 21U, 12U};

        EXPECT_EQ(a, a);
        EXPECT_EQ(a, b);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
    }
}

TEST_F(Math_Rectangle, ConstructionUsingPoints)
{
    // p[5] p[6] p[7]
    // p[4]  a   p[0]
    // p[3] p[2] p[1]
    Point a{-2, -3};

    std::array<Point, 8U> p{a, a, a, a, a, a, a, a};
    p[7].x = p[0].x = p[1].x = a.x + 3;
    p[1].y = p[2].y = p[3].y = a.y - 3;
    p[3].x = p[4].x = p[5].x = a.x - 3;
    p[5].y = p[6].y = p[7].y = a.y + 3;

    auto const createAndCheckRectangle = [](Point const &a, Point const &b) noexcept {
        Rectangle  r{a, b};
        auto const expectedX      = std::min(a.x, b.x);
        auto const expectedY      = std::min(a.y, b.y);
        auto const expectedWidth  = std::max(a.x, b.x) - expectedX;
        auto const expectedHeight = std::max(a.y, b.y) - expectedY;
        EXPECT_EQ(r.upperLeftPoint.x, expectedX);
        EXPECT_EQ(r.upperLeftPoint.y, expectedY);
        EXPECT_EQ(r.width, expectedWidth);
        EXPECT_EQ(r.height, expectedHeight);
    };

    createAndCheckRectangle(a, a);
    for (auto const &point : p)
    {
        createAndCheckRectangle(a, point);
    }
}

TEST_F(Math_Rectangle, Area)
{
    auto const checkArea = [](Rectangle const &r) noexcept { EXPECT_EQ(r.area(), r.width * r.height); };
    checkArea(Rectangle{0U, 0U});
    checkArea(Rectangle{128U, 0U});
    checkArea(Rectangle{0U, 128U});
    checkArea(Rectangle{128U, 128U});
}

TEST_F(Math_Rectangle, LowerRightPoint)
{
    EXPECT_EQ(r0.upperLeftPoint, r0.lowerRightPoint());
    Rectangle const r1{4U, 4U};
    EXPECT_EQ(r1.upperLeftPoint.x + r1.width, r1.lowerRightPoint().x);
    EXPECT_EQ(r1.upperLeftPoint.y + r1.height, r1.lowerRightPoint().y);
}

TEST_F(Math_Rectangle, Center)
{
    EXPECT_EQ(r0.center(), Point{});
    Rectangle const r1{3U, 3U};
    EXPECT_EQ(r1.center(), Point(1, 1));
    Rectangle const r2{4U, 4U};
    EXPECT_EQ(r2.center(), Point(2, 2));
}

TEST_F(Math_Rectangle, Intersection)
{
    // intersection with self returns self
    EXPECT_EQ(r0.intersection(r0), r0);
    Rectangle const r1{12U, 12U};
    EXPECT_EQ(r1.intersection(r1), r1);

    // intersection transitive
    EXPECT_EQ(r0.intersection(r1), r0);
    EXPECT_EQ(r1.intersection(r0), r0);

    // no intersection
    Rectangle r2{-11, -11, 11U, 11U};
    EXPECT_EQ(r2.intersection(r1), r0);

    // regular intersection
    ++r2.width;
    ++r2.height;
    Rectangle expected{1U, 1U};
    EXPECT_EQ(r2.intersection(r1), expected);
    Rectangle const r3{-2, -2, 17U, 17U};
    expected.upperLeftPoint.x = -2;
    expected.upperLeftPoint.y = -2;
    expected.width            = 3U;
    expected.height           = 3U;
    EXPECT_EQ(r3.intersection(r2), expected);

    // fully enclosed
    EXPECT_EQ(r3.intersection(r1), r1);
    EXPECT_EQ(r1.intersection(r3), r1);
}

TEST_F(Math_Rectangle, Encloses)
{

    EXPECT_TRUE(r0.encloses(r0));
    Rectangle const r1{2U, 2U};
    Rectangle const r2{-1, -1, 4U, 4U};
    EXPECT_TRUE(r2.encloses(r1));
    EXPECT_FALSE(r1.encloses(r2));
}

TEST_F(Math_Rectangle, ShiftEdge)
{
    Rectangle const base{10, 10, 20U, 20U};

    auto const checkShift = [&base](Direction const     direction,
                                    std::int32_t const  value,
                                    std::int32_t const  expectedX,
                                    std::int32_t const  expectedY,
                                    std::uint32_t const expectedWidth,
                                    std::uint32_t const expectedHeight) noexcept {
        Rectangle sut{base};
        sut.shiftEdge(direction, value);
        EXPECT_EQ(sut.upperLeftPoint.x, expectedX);
        EXPECT_EQ(sut.upperLeftPoint.y, expectedY);
        EXPECT_EQ(sut.width, expectedWidth);
        EXPECT_EQ(sut.height, expectedHeight);
    };

    // Expand
    checkShift(Direction::Up, 32, 10, 42, 20U, 52U);
    checkShift(Direction::Right, 32, 10, 10, 52U, 20U);
    checkShift(Direction::Down, 32, 10, 10, 20U, 52U);
    checkShift(Direction::Left, 32, -22, 10, 52U, 20U);

    // Shrink
    checkShift(Direction::Up, -5, 10, 5, 20U, 15U);
    checkShift(Direction::Right, -5, 10, 10, 15U, 20U);
    checkShift(Direction::Down, -5, 10, 10, 20U, 15U);
    checkShift(Direction::Left, -5, 15, 10, 15U, 20U);

    // Shrink too much
    checkShift(Direction::Up, -25, 10, -10, 20U, 0U);
    checkShift(Direction::Right, -25, 10, 10, 0U, 20U);
    checkShift(Direction::Down, -25, 10, 10, 20U, 0U);
    checkShift(Direction::Left, -25, 30, 10, 0U, 20U);
}

TEST_F(Math_Rectangle, RangeCreatedCorrectly)
{
    Rectangle const sut{10, 10, 20U, 20U};
    EXPECT_EQ((*sut.range().end()).index, sut.area());
}

} // namespace Terrahertz::UnitTests
