#include "THzCommon/utility/bitbuffer.hpp"

#include <array>
#include <cstdint>
#include <gsl/gsl>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityBitBufferReader : public testing::Test
{
    std::array<std::uint8_t, 0x4U> array{0b1101'1011U, 0b1110'0101U, 0b1101'1011U, 0b1110'0101U};

    gsl::span<std::uint8_t const> span{array};

    BitBufferReader reader{span};
};

TEST_F(UtilityBitBufferReader, DefaultConstructedReaderCorrect)
{
    BitBufferReader defReader{};
    EXPECT_EQ(defReader.bitsLeft(), 0U);
    EXPECT_FALSE(defReader.next());
    EXPECT_EQ(defReader.bitsLeft(), 0U);
}

TEST_F(UtilityBitBufferReader, BitsLeftCorrect)
{
    EXPECT_EQ(reader.bitsLeft(), 32U);
    auto const      span3 = span.subspan(1);
    BitBufferReader reader3{span3};
    EXPECT_EQ(reader3.bitsLeft(), 24U);
    auto const      span2 = span.subspan(2);
    BitBufferReader reader2{span2};
    EXPECT_EQ(reader2.bitsLeft(), 16U);
}

TEST_F(UtilityBitBufferReader, NextCorrect)
{
    auto bits = 32U;

    auto const checkBitsLeft = [&]() {
        EXPECT_EQ(reader.bitsLeft(), bits);
        --bits;
    };

    checkBitsLeft();
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_FALSE(reader.next());
    checkBitsLeft();
    for (auto i = 0U; i < 4U; ++i)
    {
        reader.next();
        checkBitsLeft();
    }
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_TRUE(reader.next());
    checkBitsLeft();
    EXPECT_FALSE(reader.next());
    checkBitsLeft();
}

TEST_F(UtilityBitBufferReader, NextReturnsFalseWhenBufferRunsOut)
{
    auto const      span1 = span.subspan(3);
    BitBufferReader reader1{span1};
    for (auto i = 0U; i < 7; ++i)
    {
        reader1.next();
    }
    EXPECT_EQ(reader1.bitsLeft(), 1U);
    EXPECT_TRUE(reader1.next());
    EXPECT_EQ(reader1.bitsLeft(), 0U);
    EXPECT_FALSE(reader1.next());
    EXPECT_EQ(reader1.bitsLeft(), 0U);
    EXPECT_FALSE(reader1.next());
}

struct UtilityBitBufferWriter : public testing::Test
{
    std::array<std::uint8_t, 0x4U> array{};

    gsl::span<std::uint8_t> span{array};

    BitBufferWriter writer{span};

    size_t bits = (8U * span.size());

    void checkWriting(bool bit)
    {
        EXPECT_TRUE(writer.write(bit));
        --bits;
        EXPECT_EQ(writer.bitsLeft(), bits);
    }
};

TEST_F(UtilityBitBufferWriter, DefaultConstructedWriterCorrect)
{
    BitBufferWriter defWriter{};
    EXPECT_FALSE(defWriter.write(true));
    EXPECT_EQ(defWriter.bitsLeft(), 0U);
}

TEST_F(UtilityBitBufferWriter, NewBufferHasAllBitsLeft) { EXPECT_EQ(writer.bitsLeft(), 8U * span.size()); }

TEST_F(UtilityBitBufferWriter, WritingSingleBits)
{
    array[0U] = 0x00U;
    checkWriting(true);
    checkWriting(false);
    checkWriting(true);
    EXPECT_EQ(array[0U], 0b1010'0000U);
}

TEST_F(UtilityBitBufferWriter, WritingZerosCorrectly)
{
    array[0U] = 0xFFU;
    checkWriting(false);
    checkWriting(true);
    checkWriting(false);
    EXPECT_EQ(array[0U], 0b0101'1111U);
}

TEST_F(UtilityBitBufferWriter, WritingMoreThanEightBits)
{
    for (auto i = 0U; i < 24U; ++i)
    {
        checkWriting(true);
    }
    EXPECT_EQ(array[0U], 0xFFU);
    EXPECT_EQ(array[1U], 0xFFU);
    EXPECT_EQ(array[2U], 0xFFU);
    EXPECT_EQ(array[3U], 0x00U);
}

TEST_F(UtilityBitBufferWriter, WritingMultipleBitsDoesNotWriteIfGivenBufferTooSmall)
{
    std::array<std::uint8_t, 1U> bitArray{0b1100'0011U};
    gsl::span<std::uint8_t>      bitSpan{bitArray};
    EXPECT_EQ(writer.write(bitSpan, 9U), 0U);
}

TEST_F(UtilityBitBufferWriter, WritingMuliplteBits)
{
    std::array<std::uint8_t, 2U> bitArray{0b1100'0011U, 0b1111'1111U};
    gsl::span<std::uint8_t>      bitSpan{bitArray};
    EXPECT_EQ(writer.write(bitSpan, 3U), 3U);
    EXPECT_EQ(writer.write(bitSpan, 2U), 2U);
    EXPECT_EQ(writer.write(bitSpan, 9U), 9U);
    EXPECT_EQ(array[0U], 0b1101'1110U);
    EXPECT_EQ(array[1U], 0b0001'1100U);
}

TEST_F(UtilityBitBufferWriter, WritingMoreBitsThenTheBufferCanHold)
{
    std::array<std::uint8_t, 2U> bitArray{0b1100'0011U, 0b1111'1111U};
    gsl::span<std::uint8_t>      bitSpan{bitArray};
    EXPECT_EQ(writer.write(bitSpan, 16U), 16U);
    EXPECT_EQ(writer.write(bitSpan, 14U), 14U);
    EXPECT_EQ(writer.write(bitSpan, 8U), 2U);
    EXPECT_EQ(writer.write(bitSpan, 8U), 0U);
    EXPECT_FALSE(writer.write(true));
}

TEST_F(UtilityBitBufferWriter, OffsetIsUsed)
{
    std::array<std::uint8_t, 2U> bitArray{0b1100'0011U, 0b1111'1111U};
    gsl::span<std::uint8_t>      bitSpan{bitArray};
    EXPECT_EQ(writer.write(bitSpan, 4U, 6U), 4U);
    EXPECT_EQ(array[0U], 0b1111'0000U);
}

} // namespace Terrahertz::UnitTests
