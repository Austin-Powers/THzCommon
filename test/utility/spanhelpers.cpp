#include "THzCommon/utility/spanhelpers.h"

#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

namespace Terrahertz::UnitTests {

struct Utility_SpanHelpers : public testing::Test
{
    struct TestData
    {
        uint8_t  v0{};
        uint8_t  v1{};
        uint16_t v2{};
        uint16_t v3{};
        uint32_t v4{};
    };

    std::array<std::uint16_t, 32U> arr{};

    std::vector<std::uint16_t> vec{};

    std::array<std::uint8_t, 32U> buffer{};
};

TEST_F(Utility_SpanHelpers, ToSpanCreatesSpanFromPointerAndSize)
{
    auto const uint16span = toSpan<std::uint16_t>(arr.data(), arr.size());
    EXPECT_EQ(uint16span.data(), arr.data());
    EXPECT_EQ(uint16span.size(), arr.size());

    auto const int16span = toSpan<std::int16_t>(arr.data(), arr.size());
    EXPECT_EQ(int16span.data(), std::bit_cast<std::int16_t *>(arr.data()));
    EXPECT_EQ(int16span.size(), arr.size());
}

TEST_F(Utility_SpanHelpers, ToSpanCreatesSpanFromContainer)
{
    {
        auto const uint16span = toSpan<std::uint16_t>(arr);
        EXPECT_EQ(uint16span.data(), arr.data());
        EXPECT_EQ(uint16span.size(), arr.size());

        auto const int16span = toSpan<std::int16_t>(arr);
        EXPECT_EQ(int16span.data(), std::bit_cast<std::int16_t *>(arr.data()));
        EXPECT_EQ(int16span.size(), arr.size());
    }
    {
        auto const uint16span = toSpan<std::uint16_t>(vec);
        EXPECT_EQ(uint16span.data(), vec.data());
        EXPECT_EQ(uint16span.size(), vec.size());

        auto const int16span = toSpan<std::int16_t>(vec);
        EXPECT_EQ(int16span.data(), std::bit_cast<std::int16_t *>(vec.data()));
        EXPECT_EQ(int16span.size(), vec.size());
    }
    {
        vec.resize(16U);
        auto const uint16span = toSpan<std::uint16_t>(vec);
        EXPECT_EQ(uint16span.data(), vec.data());
        EXPECT_EQ(uint16span.size(), vec.size());

        auto const int16span = toSpan<std::int16_t>(vec);
        EXPECT_EQ(int16span.data(), std::bit_cast<std::int16_t *>(vec.data()));
        EXPECT_EQ(int16span.size(), vec.size());
    }
}

TEST_F(Utility_SpanHelpers, WriteToSpanHandlesEmptySpan)
{
    TestData data{};

    gsl::span<std::uint8_t> span{};

    auto const result = writeToSpan(span, data);
    EXPECT_TRUE(result.empty());
}

TEST_F(Utility_SpanHelpers, WriteToSpanHandlesTooSmallSpan)
{
    TestData   data{};
    auto       span   = gsl::span<std::uint8_t>{buffer}.subspan(0U, sizeof(TestData) - 1);
    auto const result = writeToSpan(span, data);
    EXPECT_EQ(span, result);
}

TEST_F(Utility_SpanHelpers, WriteToSpan)
{
    TestData data{};
    data.v0 = 0x47U;
    data.v1 = 0x11U;
    data.v2 = 0x8150U;
    data.v3 = 0x1337U;
    data.v4 = 0x19911124U;

    auto       span   = gsl::span<std::uint8_t>{buffer};
    auto const result = writeToSpan(span, data);
    EXPECT_EQ(result.size(), span.size() - sizeof(TestData));
    auto ptr = std::bit_cast<std::uint8_t *>(&data);
    for (auto i = 0U; i < sizeof(TestData); ++i)
    {
        ASSERT_EQ(buffer[i], ptr[i]);
    }
    EXPECT_EQ(buffer[sizeof(TestData)], 0U);
}

TEST_F(Utility_SpanHelpers, ReadFromTooSmallSpan)
{
    TestData data{};
    data.v0 = 0x47U;
    data.v1 = 0x11U;
    data.v2 = 0x8150U;
    data.v3 = 0x1337U;
    data.v4 = 0x19911124U;

    {
        gsl::span<std::uint8_t> span{};

        auto const result = readFromSpan(span, data);
        EXPECT_TRUE(result.empty());
        EXPECT_EQ(data.v0, 0x47U);
        EXPECT_EQ(data.v1, 0x11U);
        EXPECT_EQ(data.v2, 0x8150U);
        EXPECT_EQ(data.v3, 0x1337U);
        EXPECT_EQ(data.v4, 0x19911124U);
    }
    {
        auto const span   = gsl::span<std::uint8_t>{buffer}.subspan(0U, sizeof(TestData) - 1U);
        auto const result = readFromSpan(span, data);
        EXPECT_EQ(span, result);
        EXPECT_EQ(data.v0, 0x47U);
        EXPECT_EQ(data.v1, 0x11U);
        EXPECT_EQ(data.v2, 0x8150U);
        EXPECT_EQ(data.v3, 0x1337U);
        EXPECT_EQ(data.v4, 0x19911124U);
    }
}

TEST_F(Utility_SpanHelpers, ReadFromSpan)
{
    TestData data{};
    data.v0 = 0x47U;
    data.v1 = 0x11U;
    data.v2 = 0x8150U;
    data.v3 = 0x1337U;
    data.v4 = 0x19911124U;

    auto span = gsl::span<std::uint8_t>{buffer};
    span      = writeToSpan(span, data);
    span      = gsl::span<std::uint8_t>{buffer};

    TestData   read{};
    auto const result = readFromSpan(span, read);
    EXPECT_EQ(result.size(), span.size() - sizeof(TestData));
    EXPECT_EQ(data.v0, read.v0);
    EXPECT_EQ(data.v1, read.v1);
    EXPECT_EQ(data.v2, read.v2);
    EXPECT_EQ(data.v3, read.v3);
    EXPECT_EQ(data.v4, read.v4);
}

TEST_F(Utility_SpanHelpers, TrySubspanReturnsEmptySpan)
{
    auto const span   = gsl::span<std::uint8_t>{buffer};
    auto const result = trySubspan(span, buffer.size());
    EXPECT_TRUE(result.empty());
}

TEST_F(Utility_SpanHelpers, TrySubspanReturnsCorrectSubspan)
{
    auto const span     = gsl::span<std::uint8_t>{buffer};
    auto const length   = buffer.size() - 1U;
    auto const expected = span.subspan(length);
    auto const actual   = trySubspan(span, length);
    EXPECT_EQ(actual, expected);
}

} // namespace Terrahertz::UnitTests
