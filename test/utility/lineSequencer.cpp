#include "THzCommon/utility/lineSequencer.hpp"

#include "THzCommon/utility/spanhelpers.hpp"

#include <array>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Utility_LineSequencer : public testing::Test
{
    std::array<double, 40U> arr{};
    gsl::span<double>       span{toSpan<double>(arr)};
};

TEST_F(Utility_LineSequencer, CreationFailsIfBufferSizeIsNotAMultipleOfWidth)
{
    auto sut = LineSequencer<double>::create(span, 3U);
    EXPECT_FALSE(sut);
    sut = LineSequencer<double>::create(span, 80U);
    EXPECT_FALSE(sut);
    sut = LineSequencer<double>::create(span, 0U);
    EXPECT_FALSE(sut);
}

TEST_F(Utility_LineSequencer, SequencingBufferBackwards)
{
    auto const lineWidth = 10U;

    auto sut = LineSequencer<double>::create(span, lineWidth);
    ASSERT_TRUE(sut);

    auto lineCount = 0U;
    auto line      = sut->nextLine();
    do
    {
        ++lineCount;
        EXPECT_EQ(line.size(), lineWidth);
        EXPECT_EQ(line.data(), (span.data() + span.size() - (lineCount * lineWidth)));
        line = sut->nextLine();
    } while (!line.empty());

    EXPECT_EQ(lineCount, span.size() / lineWidth);
}

TEST_F(Utility_LineSequencer, SequencingBufferForwards)
{
    auto const lineWidth = 10U;

    auto sut = LineSequencer<double>::create(span, lineWidth, false);
    ASSERT_TRUE(sut);

    auto lineCount = 0U;
    auto line      = sut->nextLine();
    do
    {
        EXPECT_EQ(line.size(), lineWidth);
        EXPECT_EQ(line.data(), (span.data() + (lineCount * lineWidth)));
        ++lineCount;
        line = sut->nextLine();
    } while (!line.empty());

    EXPECT_EQ(lineCount, span.size() / lineWidth);
}

} // namespace Terrahertz::UnitTests
