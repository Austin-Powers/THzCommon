#include "THzCommon/utility/range2DFolding.hpp"

#include "THzCommon/math/rectangle.hpp"

#include <gtest/gtest.h>
#include <limits>

namespace Terrahertz::UnitTests {

struct UtilityRange2DFolding : public testing::Test
{
    void checkOuterIterator(Range2DFolding::OuterIterator iter, size_t const beginIndex, size_t const endIndex) noexcept
    {
        EXPECT_EQ((*iter.begin()).index, beginIndex);
        EXPECT_EQ((*iter.end()).index, endIndex);
    };

    Rectangle getZoneDimensions(Range2DFolding::OuterIterator &iterator) noexcept
    {
        Rectangle  result{};
        auto const beginPos     = (*iterator.begin());
        result.upperLeftPoint.x = beginPos.x;
        result.upperLeftPoint.y = beginPos.y;
        for (auto pos : iterator)
        {
            auto const width = (pos.x - result.upperLeftPoint.x);
            if (result.width < width)
            {
                result.width = width;
            }
            auto const height = (pos.y - result.upperLeftPoint.y);
            if (result.height < height)
            {
                result.height = height;
            }
        }
        ++result.width;
        ++result.height;
        return result;
    }

    void checkRangeParameters(Range2DFolding     &toCheck,
                              std::uint32_t const expectedBufferWidth,
                              std::uint32_t const expectedBufferHeight,
                              std::uint32_t const expectedZoneWidth,
                              std::uint32_t const expectedZoneHeight,
                              std::uint32_t const expectedShiftX,
                              std::uint32_t const expectedShiftY) noexcept
    {
        std::uint32_t actualBufferWidth  = 0U;
        std::uint32_t actualBufferHeight = 0U;
        std::uint32_t actualZoneWidth    = 0U;
        std::uint32_t actualZoneHeight   = 0U;
        std::uint32_t actualShiftX       = 0U;
        std::uint32_t actualShiftY       = 0U;

        bool      firstRun = true;
        Rectangle prevDims{};
        for (auto outer : toCheck)
        {
            auto const dims = getZoneDimensions(outer);
            if (actualBufferWidth < static_cast<std::uint32_t>(dims.upperLeftPoint.x))
            {
                actualBufferWidth = dims.upperLeftPoint.x;
            }
            if (actualBufferHeight < static_cast<std::uint32_t>(dims.upperLeftPoint.y))
            {
                actualBufferHeight = dims.upperLeftPoint.y;
            }
            actualZoneWidth  = dims.width;
            actualZoneHeight = dims.height;
            if (!firstRun)
            {
                auto shiftX = dims.upperLeftPoint.x - prevDims.upperLeftPoint.x;
                if (shiftX < 0)
                {
                    shiftX = 0;
                }
                if (actualShiftX < static_cast<std::uint32_t>(shiftX))
                {
                    actualShiftX = shiftX;
                }
                auto shiftY = dims.upperLeftPoint.y - prevDims.upperLeftPoint.y;
                if (shiftY < 0)
                {
                    shiftY = 0;
                }
                if (actualShiftY < static_cast<std::uint32_t>(shiftY))
                {
                    actualShiftY = shiftY;
                }
            }
            firstRun = false;
            prevDims = dims;
        }

        actualBufferWidth += actualZoneWidth;
        actualBufferHeight += actualZoneHeight;

        EXPECT_EQ(expectedBufferWidth, actualBufferWidth);
        EXPECT_EQ(expectedBufferHeight, actualBufferHeight);
        EXPECT_EQ(expectedZoneWidth, actualZoneWidth);
        EXPECT_EQ(expectedZoneHeight, actualZoneHeight);
        EXPECT_EQ(expectedShiftX, actualShiftX);
        EXPECT_EQ(expectedShiftY, actualShiftY);
    }
};

TEST_F(UtilityRange2DFolding, PositionSetupWidthZero)
{
    Range2DFolding::Position sut{};
    sut.setup(0U, 0U);
    EXPECT_EQ(sut.index, std::numeric_limits<size_t>::max());
}

TEST_F(UtilityRange2DFolding, PositionSetupCorrect)
{
    Range2DFolding::Position sut{};
    size_t const             index = 43U;
    std::uint32_t const      width = 14U;

    sut.setup(index, width);
    EXPECT_EQ(sut.x, (index % width));
    EXPECT_EQ(sut.y, (index / width));
}

TEST_F(UtilityRange2DFolding, InnerIteratorInvalidParameters)
{
    using SutClass = Range2DFolding::InnerIterator;
    // Default construction
    SutClass sut0{};
    // zoneOffset + zoneWidth > bufferWidth
    SutClass sut1{4U, 2U, 3U, 4U};
    // startIndex points before zone
    SutClass sut2{8U, 2U, 4U, 1U};
    SutClass sut3{8U, 2U, 4U, 9U};
    // startIndex points after zone
    SutClass sut4{8U, 2U, 4U, 6U};
    SutClass sut5{8U, 2U, 4U, 14U};
    // zeros
    SutClass sut6{0U, 0U, 0U, 0U};

    EXPECT_EQ((*sut0).index, std::numeric_limits<size_t>::max());
    EXPECT_EQ(sut0, sut1);
    EXPECT_EQ(sut1, sut2);
    EXPECT_EQ(sut2, sut3);
    EXPECT_EQ(sut3, sut4);
    EXPECT_EQ(sut4, sut5);
    EXPECT_EQ(sut5, sut6);
}

TEST_F(UtilityRange2DFolding, InnerIteratorConstruction)
{
    using SutClass = Range2DFolding::InnerIterator;
    SutClass sutDefault{};
    SutClass sut0{8U, 0U, 4U, 0U};

    EXPECT_NE(sutDefault, sut0);
    auto const pos0 = (*sut0);
    EXPECT_EQ(pos0.index, 0U);
    EXPECT_EQ(pos0.x, 0U);
    EXPECT_EQ(pos0.y, 0U);

    SutClass sut1{8U, 2U, 4U, 2U};
    EXPECT_NE(sutDefault, sut1);
    auto const pos1 = (*sut1);
    EXPECT_EQ(pos1.index, 2U);
    EXPECT_EQ(pos1.x, 2U);
    EXPECT_EQ(pos1.y, 0U);

    SutClass sut2{8U, 2U, 4U, 10U};
    EXPECT_NE(sutDefault, sut2);
    auto const pos2 = (*sut2);
    EXPECT_EQ(pos2.index, 10U);
    EXPECT_EQ(pos2.x, 2U);
    EXPECT_EQ(pos2.y, 1U);
}

TEST_F(UtilityRange2DFolding, InnerIteratorIncrementing)
{
    Range2DFolding::InnerIterator sut{4U, 1U, 2U, 1U};
    for (auto y = 0U; y < 3U; ++y)
    {
        for (auto x = 1U; x < 3U; ++x)
        {
            auto const pos = (*sut);
            EXPECT_EQ(pos.index, (x + (y * 4U)));
            EXPECT_EQ(pos.x, x);
            EXPECT_EQ(pos.y, y);
            sut++;
        }
    }
}

TEST_F(UtilityRange2DFolding, OuterIteratorInvalidParameters)
{
    using SutClass = Range2DFolding::OuterIterator;
    SutClass sut0{};
    // zoneWidth > bufferWidth
    SutClass sut1{16U, 16U, 17U, 16U, 1U, 1U};
    // zoneHeight > bufferHeight
    SutClass sut2{16U, 16U, 16U, 17U, 1U, 1U};
    // startIndexOutOfBounds
    SutClass sut3{16U, 16U, 8U, 8U, 8U, 8U, 9U};

    EXPECT_EQ(sut0, sut1);
    EXPECT_EQ(sut1, sut2);
    EXPECT_EQ(sut2, sut3);
}

TEST_F(UtilityRange2DFolding, OuterIteratorBeginEnd)
{
    using SutClass = Range2DFolding::OuterIterator;
    SutClass sutDefault{};
    SutClass sut0{16U, 16U, 4U, 4U, 1U, 1U};
    EXPECT_NE(sutDefault, sut0);
    checkOuterIterator(sut0, 0U, 64U);
    auto const dims = getZoneDimensions(sut0);
    EXPECT_EQ(dims.width, 4U);
    EXPECT_EQ(dims.height, 4U);

    SutClass sut1{12U, 16U, 8U, 3U, 2U, 3U};
    EXPECT_NE(sutDefault, sut1);
    checkOuterIterator(sut1, 0U, 36U);

    SutClass sut2{4U, 4U, 4U, 4U, 1U, 1U};
    EXPECT_NE(sutDefault, sut2);
    checkOuterIterator(sut2, 0U, 16U);
}

TEST_F(UtilityRange2DFolding, OuterIteratorIncrementing)
{
    using SutClass = Range2DFolding::OuterIterator;
    SutClass sut0{16U, 16U, 4U, 4U, 1U, 1U};
    sut0++;
    checkOuterIterator(sut0, 1U, 65U);
    for (auto i = 0U; i < 12U; ++i)
    {
        sut0++;
    }
    checkOuterIterator(sut0, 16U, 80U);

    SutClass sut1{12U, 16U, 8U, 3U, 2U, 3U};
    sut1++;
    checkOuterIterator(sut1, 2U, 38U);
    sut1++;
    checkOuterIterator(sut1, 4U, 40U);
    sut1++;
    checkOuterIterator(sut1, 36U, 72U);

    SutClass sut2{4U, 4U, 4U, 4U, 1U, 1U};
    sut2++;
    checkOuterIterator(sut2, 4U, 20U);
}

TEST_F(UtilityRange2DFolding, BasicFunctionality)
{
    Range2DFolding sut{16U, 16U, 4U, 4U, 4U, 4U};

    Range2DFolding::InnerIterator const defaultInner{};
    EXPECT_NE(sut.begin().begin(), defaultInner);
    EXPECT_NE(sut.begin().end(), defaultInner);
    EXPECT_NE(sut.end().begin(), defaultInner);
    EXPECT_NE(sut.end().end(), defaultInner);

    auto outerRuns = 0U;
    for (auto zone : sut)
    {
        ++outerRuns;
        auto innerRuns = 0U;
        for (auto index : zone)
        {
            ++innerRuns;
        }
        EXPECT_EQ(innerRuns, 16U);

        //  prevent infinite loop
        if (outerRuns > 32U)
        {
            break;
        }
    }

    EXPECT_EQ(outerRuns, 16U);
    checkRangeParameters(sut, 16U, 16U, 4U, 4U, 4U, 4U);
}

TEST_F(UtilityRange2DFolding, CheckBehavior)
{
    Range2DFolding sut0{16U, 12U, 1U, 1U, 1U, 1U};
    checkRangeParameters(sut0, 16U, 12U, 1U, 1U, 1U, 1U);
    Range2DFolding sut1{16U, 16U, 2U, 2U, 4U, 4U};
    // 14 because last 2 rows/colums are skipped
    checkRangeParameters(sut1, 14U, 14U, 2U, 2U, 4U, 4U);
    Range2DFolding sut2{9U, 12U, 4U, 4U, 2U, 4U};
    checkRangeParameters(sut2, 8U, 12U, 4U, 4U, 2U, 4U);
}

} // namespace Terrahertz::UnitTests
