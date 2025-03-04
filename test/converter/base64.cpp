#include "THzCommon/converter/base64.hpp"

#include "THzCommon/utility/spanhelpers.hpp"

#include <array>
#include <cstring>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct ConverterBase64 : public testing::Test
{
    static constexpr char TestData[] = "You're only unproductive by the standards of the world we lived in two months "
                                       "ago and that world is gone now\n";

    // encoded using echo "string" | base64
    // decode using echo encoded | base64 -d
    static constexpr char TestDataEncoded[] =
        "WW91J3JlIG9ubHkgdW5wcm9kdWN0aXZlIGJ5IHRoZSBzdGFuZGFyZHMgb2YgdGhlIHdvcmxkIHdlIGxpdmVkIGluIHR3byBtb250aHMgYWdvIG"
        "FuZCB0aGF0IHdvcmxkIGlzIGdvbmUgbm93Cg==";

    std::array<std::uint8_t, 128U> rawData{};
    std::array<char, 192U>         encodedData{};

    gsl::span<std::uint8_t> rawDataSpan{rawData};
    gsl::span<char>         encodedDataSpan{encodedData};

    void initRawData() noexcept
    {
        auto const length = std::strlen(TestData);
        std::memcpy(rawData.data(), TestData, length);
        rawDataSpan = rawDataSpan.subspan(0, length);
    }

    void initEncodedData() noexcept
    {
        auto const length = std::strlen(TestDataEncoded);
        std::memcpy(encodedData.data(), TestDataEncoded, length);
        encodedDataSpan = encodedDataSpan.subspan(0, length);
    }
};

TEST_F(ConverterBase64, EncodedSize)
{
    for (auto i = 0U; i < 16U; ++i)
    {
        EXPECT_EQ(Base64::encodedSize(i), ((i + 2U) / 3U) * 4U);
    }
}

TEST_F(ConverterBase64, DecodedSize)
{
    for (auto i = 0U; i < 16U; ++i)
    {
        EXPECT_EQ(Base64::decodedSize(i), (i / 4U) * 3U);
    }
}

TEST_F(ConverterBase64, EncodingReturnsEmptySpanIfNoDataWasGiven)
{
    gsl::span<std::uint8_t> emptySpan{};

    auto const result = Base64::encode(emptySpan, encodedDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, EncodingReturnsEmptySpanIfTooSmall)
{
    initRawData();
    auto const smallEncodedDataSpan = encodedDataSpan.subspan(0, Base64::encodedSize(rawDataSpan.size()) - 1U);
    auto const result               = Base64::encode(rawDataSpan, smallEncodedDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, EncodingReturnsCorrectResult)
{
    initRawData();
    auto const buffer                 = Base64::encode(rawDataSpan, encodedDataSpan);
    auto const encodedSizeExpectation = Base64::encodedSize(rawDataSpan.size());
    ASSERT_EQ(buffer.size(), encodedSizeExpectation);
    for (auto i = 0U; i < encodedSizeExpectation; ++i)
    {
        ASSERT_EQ(buffer[i], TestDataEncoded[i]);
    }
}

TEST_F(ConverterBase64, DecodingReturnsEmptySpanIfNoDataWasGiven)
{
    gsl::span<char> emptySpan{};

    auto const result = Base64::decode(emptySpan, rawDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, DecodingReturnsEmptySpanIfOutputIsTooSmall)
{
    initEncodedData();
    auto const smallDecodedDataSpan = rawDataSpan.subspan(0, Base64::decodedSize(encodedDataSpan.size()) - 1U);
    auto const result               = Base64::decode(encodedDataSpan, smallDecodedDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, DecodingReturnsEmptySpanIfInputHasIllegalSymbols)
{
    initEncodedData();
    encodedData[32U]  = '#';
    auto const result = Base64::decode(encodedDataSpan, rawDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, DecodingReturnsEmptySpanIfInputHasEqualSignInTheMiddle)
{
    initEncodedData();
    encodedData[32U]  = '=';
    auto const result = Base64::decode(encodedDataSpan, rawDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, DecodingReturnsEmptySpanIfInputLengthIsNotAMultipleOfFour)
{
    initEncodedData();
    auto const illSizedSpan = encodedDataSpan.subspan(0U, encodedDataSpan.size() - 1U);
    auto const result       = Base64::decode(illSizedSpan, rawDataSpan);
    EXPECT_TRUE(result.empty());
}

TEST_F(ConverterBase64, DecodingReturnsCorrectResult)
{
    initEncodedData();
    auto const buffer                 = Base64::decode(encodedDataSpan, rawDataSpan);
    auto const decodedSizeExpectation = std::strlen(TestData);
    ASSERT_EQ(buffer.size(), decodedSizeExpectation);
    for (auto i = 0U; i < decodedSizeExpectation; ++i)
    {
        ASSERT_EQ(buffer[i], TestData[i]);
    }
}

TEST_F(ConverterBase64, DecodingRecreatesEncodedDataInDifferentLengths)
{
    auto const checkRecreation = [&](char const *const data) noexcept {
        auto const dataLength = std::strlen(data);
        auto const dataSpan   = gsl::span<std::uint8_t const>{std::bit_cast<std::uint8_t const *>(data), dataLength};
        encodedDataSpan       = toSpan<char>(encodedData);
        auto const encoded    = Base64::encode(dataSpan, encodedDataSpan);
        rawDataSpan           = toSpan<std::uint8_t>(rawData);
        auto const decoded    = Base64::decode(encoded, rawDataSpan);

        // compare
        EXPECT_EQ(decoded.size(), dataLength);
    };
    checkRecreation("Test");
    checkRecreation("Test1");
    checkRecreation("Test12");
    checkRecreation("Test123");
    checkRecreation("Test1234");
    checkRecreation("Test12345");
}

TEST_F(ConverterBase64, EncoderWritingBytes)
{
    Base64::Encoder sut{encodedDataSpan};
    EXPECT_TRUE(sut.writeByte(2U));
    EXPECT_EQ(encodedDataSpan[0U], 0U);
    EXPECT_TRUE(sut.writeByte(4U));
    EXPECT_EQ(encodedDataSpan[0U], 0U);
    EXPECT_TRUE(sut.writeByte(8U));
    EXPECT_EQ(encodedDataSpan[0U], 'A');
}

TEST_F(ConverterBase64, EncoderRunningOutOfBuffer)
{
    auto const buffer = encodedDataSpan.subspan(0U, 3U);

    Base64::Encoder sut{buffer};
    EXPECT_FALSE(sut.writeByte(2U));
}

TEST_F(ConverterBase64, DecoderReadingBytes)
{
    Base64::Encoder encoder{encodedDataSpan};
    EXPECT_TRUE(encoder.writeByte(32U));
    EXPECT_TRUE(encoder.writeByte(16U));
    EXPECT_TRUE(encoder.writeByte(8U));

    Base64::Decoder sut{encodedDataSpan};
    EXPECT_TRUE(sut.dataAvailable());
    EXPECT_EQ(sut.readByte(), 32U);
    EXPECT_TRUE(sut.dataAvailable());
    EXPECT_EQ(sut.readByte(), 16U);
    EXPECT_TRUE(sut.dataAvailable());
    EXPECT_EQ(sut.readByte(), 8U);
    EXPECT_TRUE(sut.dataAvailable());
}

TEST_F(ConverterBase64, DecoderReadingMultibyteType)
{
    std::uint32_t const expectedValue{4711U};
    std::uint32_t       actualValue{};
    Base64::Encoder     encoder{encodedDataSpan};

    EXPECT_TRUE(encoder.write(expectedValue));
    // add two bytes to actually write things
    EXPECT_TRUE(encoder.writeByte(12U));
    EXPECT_TRUE(encoder.writeByte(24U));

    Base64::Decoder sut{encodedDataSpan};
    EXPECT_TRUE(sut.read(actualValue));
    EXPECT_EQ(expectedValue, actualValue);
}

} // namespace Terrahertz::UnitTests
