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
                              std::uint32_t const bufferWidth,
                              std::uint32_t const bufferHeight,
                              std::uint32_t const zoneWidth,
                              std::uint32_t const zoneHeight,
                              std::uint32_t const shiftX,
                              std::uint32_t const shiftY) noexcept
    {
        std::uint32_t actualBufferWidth  = 0U;
        std::uint32_t actualBufferHeight = 0U;
        std::uint32_t actualZoneWidth    = 0U;
        std::uint32_t actualZoneHeight   = 0U;
        std::uint32_t actualShiftX       = 0U;
        std::uint32_t actualShiftY       = 0U;

        Rectangle prevDims{};
        auto      counter = 0U;
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
            if (counter > 0U)
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
            prevDims = dims;

            ++counter;
            EXPECT_LE(counter, 320U);
            if (counter > 320U)
            {
                // Avoid endless loop in case of an error
                break;
            }
        }

        actualBufferWidth += actualZoneWidth;
        actualBufferHeight += actualZoneHeight;

        EXPECT_EQ(zoneWidth, actualZoneWidth);
        EXPECT_EQ(zoneHeight, actualZoneHeight);

        auto const expectedBufferWidth  = bufferWidth - ((bufferWidth - zoneWidth) % shiftX);
        auto const expectedBufferHeight = bufferHeight - ((bufferHeight - zoneHeight) % shiftY);
        EXPECT_EQ(expectedBufferWidth, actualBufferWidth);
        if (expectedBufferHeight != actualBufferHeight)
        {
            EXPECT_EQ(expectedBufferHeight, actualBufferHeight);
        }
        if (actualShiftX != 0U)
        {
            // if shiftX is zero we could not measure it
            EXPECT_EQ(shiftX, actualShiftX);
        }
        if (actualShiftY != 0U)
        {
            // if shiftY is zero we could not measure it
            EXPECT_EQ(shiftY, actualShiftY);
        }
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
    // shift == 0
    SutClass sut4{16U, 16U, 4U, 4U, 0U, 0U};
    // zone... == 0
    SutClass sut5{16U, 16U, 0U, 0U, 1U, 1U};

    EXPECT_EQ(sut0, sut1);
    EXPECT_EQ(sut1, sut2);
    EXPECT_EQ(sut2, sut3);
    EXPECT_EQ(sut3, sut4);
    EXPECT_EQ(sut4, sut5);
}

TEST_F(UtilityRange2DFolding, OuterIteratorBeginEnd)
{
    using SutClass = Range2DFolding::OuterIterator;
    SutClass sutDefault{};
    SutClass sut0{16U, 16U, 4U, 4U, 1U, 1U};
    EXPECT_NE(sutDefault, sut0);
    checkOuterIterator(sut0, 0U, 64U);
    auto const dims0 = getZoneDimensions(sut0);
    EXPECT_EQ(dims0.width, 4U);
    EXPECT_EQ(dims0.height, 4U);
    auto const tPos = sut0.targetPosition();
    EXPECT_EQ(tPos.index, 0U);
    EXPECT_EQ(tPos.x, 0U);
    EXPECT_EQ(tPos.y, 0U);

    SutClass sut1{12U, 16U, 8U, 3U, 2U, 3U};
    EXPECT_NE(sutDefault, sut1);
    checkOuterIterator(sut1, 0U, 36U);
    auto const dims1 = getZoneDimensions(sut1);
    EXPECT_EQ(dims1.width, 8U);
    EXPECT_EQ(dims1.height, 3U);

    SutClass sut2{4U, 4U, 4U, 4U, 1U, 1U};
    EXPECT_NE(sutDefault, sut2);
    checkOuterIterator(sut2, 0U, 16U);
    auto const dims2 = getZoneDimensions(sut2);
    EXPECT_EQ(dims2.width, 4U);
    EXPECT_EQ(dims2.height, 4U);
}

TEST_F(UtilityRange2DFolding, OuterIteratorIncrementing)
{
    using SutClass = Range2DFolding::OuterIterator;
    SutClass sut0{16U, 16U, 4U, 4U, 1U, 1U};
    sut0++;
    checkOuterIterator(sut0, 1U, 65U);
    auto targetPosition = sut0.targetPosition();
    EXPECT_EQ(targetPosition.index, 1U);
    EXPECT_EQ(targetPosition.x, 1U);
    EXPECT_EQ(targetPosition.y, 0U);
    for (auto i = 0U; i < 12U; ++i)
    {
        sut0++;
    }
    checkOuterIterator(sut0, 16U, 80U);
    targetPosition = sut0.targetPosition();
    EXPECT_EQ(targetPosition.index, 13U);
    EXPECT_EQ(targetPosition.x, 0U);
    EXPECT_EQ(targetPosition.y, 1U);

    SutClass sut1{12U, 16U, 8U, 3U, 2U, 3U};
    sut1++;
    targetPosition = sut1.targetPosition();
    EXPECT_EQ(targetPosition.index, 1U);
    EXPECT_EQ(targetPosition.x, 1U);
    EXPECT_EQ(targetPosition.y, 0U);
    checkOuterIterator(sut1, 2U, 38U);
    sut1++;
    targetPosition = sut1.targetPosition();
    EXPECT_EQ(targetPosition.index, 2U);
    EXPECT_EQ(targetPosition.x, 2U);
    EXPECT_EQ(targetPosition.y, 0U);
    checkOuterIterator(sut1, 4U, 40U);
    sut1++;
    targetPosition = sut1.targetPosition();
    EXPECT_EQ(targetPosition.index, 3U);
    EXPECT_EQ(targetPosition.x, 0U);
    EXPECT_EQ(targetPosition.y, 1U);
    checkOuterIterator(sut1, 36U, 72U);

    SutClass sut2{4U, 4U, 4U, 4U, 1U, 1U};
    sut2++;
    targetPosition = sut2.targetPosition();
    EXPECT_EQ(targetPosition.index, 1U);
    EXPECT_EQ(targetPosition.x, 0U);
    EXPECT_EQ(targetPosition.y, 1U);
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
    for (auto buffer = 1U; buffer < 17U; ++buffer)
    {
        for (auto zone = 1U; zone <= buffer; ++zone)
        {
            for (auto shift = 1U; shift < 17U; ++shift)
            {
                for (auto wobble = 0U; wobble < 2U; ++wobble)
                {
                    auto const bufferWidth  = buffer;
                    auto const bufferHeight = buffer + wobble;
                    auto const zoneWidth    = zone;
                    auto const zoneHeight   = zone + wobble;
                    auto const shiftX       = shift;
                    auto const shiftY       = shift + wobble;

                    Range2DFolding sut{bufferWidth, bufferHeight, zoneWidth, zoneHeight, shiftX, shiftY};
                    checkRangeParameters(sut, bufferWidth, bufferHeight, zoneWidth, zoneHeight, shiftX, shiftY);
                }
            }
        }
    }
}

TEST_F(UtilityRange2DFolding, EndIsReachedCorrectlyDespiteStrangeShiftValues)
{
    Range2DFolding sut{1U, 2U, 1U, 2U, 2U, 3U};

    auto iter = sut.begin();
    ++iter;
    EXPECT_EQ(iter, sut.end());
}

TEST_F(UtilityRange2DFolding, TargetDimensions)
{
    for (auto buffer = 0U; buffer < 17U; ++buffer)
    {
        for (auto zone = 0U; zone < 17U; ++zone)
        {
            for (auto shift = 0U; shift < 17U; ++shift)
            {
                Range2DFolding sut{buffer, buffer + 1U, zone, zone + 1U, shift, shift + 1U};

                auto width   = 0U;
                auto height  = 0U;
                auto limiter = 0U;
                for (auto &iter : sut)
                {
                    auto const pos = iter.targetPosition();
                    if (width <= pos.x)
                    {
                        width = pos.x + 1U;
                    }
                    if (height <= pos.y)
                    {
                        height = pos.y + 1U;
                    }
                    ++limiter;
                    if (limiter == 200U)
                    {
                        EXPECT_TRUE(false);
                        break;
                    }
                }
                auto dimensions = sut.targetDimensions();
                EXPECT_EQ(width, dimensions.first);
                EXPECT_EQ(height, dimensions.second);
            }
        }
    }
}

} // namespace Terrahertz::UnitTests
