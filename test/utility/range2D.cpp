#include "THzCommon/utility/range2D.hpp"

#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <iterator>

namespace Terrahertz::UnitTests {

struct Utility_Range2D : public testing::Test
{
    // check that Range2D::Iterator meets the requirements for a forward iterator
    static_assert(std::forward_iterator<Range2D::Iterator>, "Range2D::Iterator is not a forward_iterator");
};

TEST_F(Utility_Range2D, PositionToIndexConversionCorrect)
{
    Range2D::Position sut{};

    std::array<std::uint16_t, 8U> data{0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U};
    for (auto i = 0U; i < data.size(); ++i)
    {
        sut.index = i;
        EXPECT_EQ(data[sut], data[sut.index]);
    }
}

TEST_F(Utility_Range2D, IteratorDefaultValuesCorrect)
{
    Range2D::Iterator sut{};
    EXPECT_EQ((*sut).index, size_t{});
    EXPECT_EQ((*sut).x, std::uint32_t{});
    EXPECT_EQ((*sut).y, std::uint32_t{});
}

TEST_F(Utility_Range2D, IteratorConstructorCorrect)
{
    std::uint32_t const width{24U};
    {
        size_t const expectedIndex{50U};

        Range2D::Iterator sut{width, expectedIndex};
        EXPECT_EQ((*sut).index, expectedIndex);
        EXPECT_EQ((*sut).x, expectedIndex % width);
        EXPECT_EQ((*sut).y, expectedIndex / width);
    }
    {
        Range2D::Iterator sut{width};
        EXPECT_EQ((*sut).index, 0U);
        EXPECT_EQ((*sut).x, 0U);
        EXPECT_EQ((*sut).y, 0U);
    }
}

TEST_F(Utility_Range2D, EqualityOperatorCorrect)
{
    std::uint32_t const width{24U};
    size_t const        index{32U};
    Range2D::Iterator   iter0{width, index};
    Range2D::Iterator   iter1{width};
    Range2D::Iterator   iter2{2U};
    Range2D::Iterator   iter3{2U, index};

    EXPECT_NE(iter0, iter1);
    EXPECT_NE(iter0, iter2);
    EXPECT_EQ(iter0, iter3);
    EXPECT_EQ(iter1, iter2);
    EXPECT_NE(iter1, iter3);
    EXPECT_NE(iter2, iter3);
}

TEST_F(Utility_Range2D, IncrementationCorrect)
{
    std::uint32_t const width{3U};
    Range2D::Iterator   sut{width};
    Range2D::Iterator   sut2{width};

    auto const checkIterator = [&](Range2D::Iterator const &iter, size_t const index) noexcept {
        EXPECT_EQ((*iter).index, index);
        EXPECT_EQ((*iter).x, index % width);
        EXPECT_EQ((*iter).y, index / width);
    };

    for (auto i = 1U; i < 16U; ++i)
    {
        EXPECT_EQ(sut, sut2++);
        checkIterator(sut2, i);
        checkIterator(++sut, i);
    }
}

TEST_F(Utility_Range2D, BeginAndEndReturnCorrectIterators)
{
    std::uint32_t     width{16U};
    std::uint32_t     height{9U};
    Range2D           sut{width, height};
    Range2D::Iterator expectedBegin{width};
    Range2D::Iterator expectedEnd{width, width * height};
    EXPECT_EQ(sut.begin(), expectedBegin);
    EXPECT_EQ(sut.end(), expectedEnd);
}

TEST_F(Utility_Range2D, UsableInForEachLoop)
{
    std::uint32_t width{16U};
    std::uint32_t height{9U};

    auto counter = 0U;
    for (auto index : Range2D{width, height})
    {
        EXPECT_EQ(index.index, counter);
        EXPECT_EQ(index.x, counter % width);
        EXPECT_EQ(index.y, counter / width);
        ++counter;
    }
}

TEST_F(Utility_Range2D, ConstructionWithWidthZeroDoesNotThrow) { Range2D::Iterator sut{0U, 0U}; }

} // namespace Terrahertz::UnitTests
