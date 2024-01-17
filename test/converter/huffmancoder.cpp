#include "THzCommon/converter/huffmancoder.hpp"

#include "THzCommon/utility/fstreamhelpers.hpp"
#include "THzCommon/utility/spanhelpers.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <fstream>
#include <gsl/gsl>
#include <gtest/gtest.h>
#include <memory>

#ifndef HUFFMANTESTFILEPATH
#error "HUFFMANTESTFILEPATH must be defined"
#endif // !HUFFMANTESTFILEPATH

namespace Terrahertz::UnitTests {

struct ConverterHuffmanCoder : public testing::Test
{
    using StaticBuffer = std::array<std::uint8_t, 4U * 1024U>;
    using Buffer       = std::unique_ptr<StaticBuffer>;

    Buffer uncompressedBuffer{};
    Buffer compressedBuffer{};
    Buffer decompressedBuffer{};

    gsl::span<std::uint8_t> uncompressedSpan{};
    gsl::span<std::uint8_t> compressedSpan{};
    gsl::span<std::uint8_t> decompressedSpan{};

    Huffman::Encoder encoder{};
    Huffman::Decoder decoder{};

    void SetUp() override
    {
        uncompressedBuffer = std::make_unique<StaticBuffer>();
        compressedBuffer   = std::make_unique<StaticBuffer>();
        decompressedBuffer = std::make_unique<StaticBuffer>();
        uncompressedSpan   = gsl::span<std::uint8_t>{*uncompressedBuffer};
        compressedSpan     = gsl::span<std::uint8_t>{*compressedBuffer};
        decompressedSpan   = gsl::span<std::uint8_t>{*decompressedBuffer};
    }

    void loadInput() noexcept
    {
        std::string const filepath{HUFFMANTESTFILEPATH};
        std::ifstream     file{filepath};
        EXPECT_TRUE(readFromStream(file, *uncompressedBuffer));
    }
};

TEST_F(ConverterHuffmanCoder, EmptyEncoderBehavesCorrectly)
{
    EXPECT_EQ(encoder.expectedSize(), 0U);
    EXPECT_EQ(encoder.collectCompressedData(compressedSpan), 0U);
}

TEST_F(ConverterHuffmanCoder, EncodingRejectNewBufferIfOldBufferIsNotFullyProcessed)
{
    EXPECT_TRUE(encoder.compress(uncompressedSpan));
    EXPECT_FALSE(encoder.compress(uncompressedSpan));
}

TEST_F(ConverterHuffmanCoder, EncodingRejectsTooSmallBuffers)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const smallSpan = toSpan<std::uint8_t>(uncompressedBuffer->data(), 123U);
    EXPECT_EQ(encoder.collectCompressedData(smallSpan), 0U);
}

TEST_F(ConverterHuffmanCoder, EncodingExpectationCorrect)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    while (encodedBytes = encoder.collectCompressedData(compressedSpan))
    {
        compressedSize += encodedBytes;
        compressedSpan = compressedSpan.subspan(encodedBytes);
    }

    EXPECT_EQ(expectedSize, compressedSize);
}

TEST_F(ConverterHuffmanCoder, EncodingExpectationCorrectOnCollectingMultipleChunks)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    auto       miniSpan = compressedSpan.subspan(0U, 640U);

    auto runs = 0U;
    while (encodedBytes = encoder.collectCompressedData(miniSpan))
    {
        compressedSize += encodedBytes;
        ++runs;
    }

    EXPECT_GE(runs, 2U);
    EXPECT_EQ(expectedSize, compressedSize);
}

TEST_F(ConverterHuffmanCoder, EmptyDecoderDoesNotReturnData)
{
    EXPECT_EQ(decoder.bytesLeft(), 0U);
    EXPECT_EQ(decoder.collectNextByte(), 0U);
    EXPECT_EQ(decoder.collectDecompressedData(decompressedSpan), 0U);
}

TEST_F(ConverterHuffmanCoder, DecodingDoesNotAcceptBufferTooSmallForHeader)
{
    EXPECT_FALSE(decoder.decompress(compressedSpan.subspan(0, 5)));
}

TEST_F(ConverterHuffmanCoder, DecodingDoesNotAcceptWrongSignature) { EXPECT_FALSE(decoder.decompress(compressedSpan)); }

TEST_F(ConverterHuffmanCoder, DecodingDoesNotAcceptBufferTooSmallForTable)
{
    Huffman::CodeHeader header{};
    header.signature = Huffman::CodeSignature;
    static_cast<void>(writeToSpan(compressedSpan, header));
    EXPECT_FALSE(decoder.decompress(compressedSpan.subspan(0, 64)));
}

TEST_F(ConverterHuffmanCoder, DecodingDoesNotAcceptBufferTooSmallForCompressedData)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    while (encodedBytes = encoder.collectCompressedData(compressedSpan))
    {
        compressedSize += encodedBytes;
        compressedSpan = compressedSpan.subspan(encodedBytes);
    }
    compressedSpan = {*compressedBuffer};
    EXPECT_FALSE(decoder.decompress(compressedSpan.subspan(0U, compressedSize - 1U)));
}

TEST_F(ConverterHuffmanCoder, DecodingRecreatesEncodedData)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    while (encodedBytes = encoder.collectCompressedData(compressedSpan))
    {
        compressedSize += encodedBytes;
        compressedSpan = compressedSpan.subspan(encodedBytes);
    }
    compressedSpan = {*compressedBuffer};
    EXPECT_TRUE(decoder.decompress(compressedSpan.subspan(0U, compressedSize)));
    EXPECT_EQ(decoder.bytesLeft(), uncompressedSpan.size());
    EXPECT_EQ(decoder.collectDecompressedData(decompressedSpan), decompressedSpan.size());
    for (auto i = 0U; i < uncompressedSpan.size(); ++i)
    {
        ASSERT_EQ(uncompressedSpan[i], decompressedSpan[i]);
    }
    EXPECT_EQ(decoder.collectDecompressedData(decompressedSpan), 0U);
}

TEST_F(ConverterHuffmanCoder, DecodingRecreatesEncodedDataBytewise)
{
    loadInput();

    EXPECT_TRUE(encoder.compress(uncompressedSpan));
    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    while (encodedBytes = encoder.collectCompressedData(compressedSpan))
    {
        compressedSize += encodedBytes;
        compressedSpan = compressedSpan.subspan(encodedBytes);
    }
    compressedSpan = {*compressedBuffer};
    EXPECT_TRUE(decoder.decompress(compressedSpan.subspan(0U, compressedSize)));
    EXPECT_EQ(decoder.bytesLeft(), uncompressedSpan.size());
    for (auto i = 0U; i < 100U; ++i)
    {
        ASSERT_EQ(decoder.bytesLeft(), uncompressedSpan.size() - i);
        ASSERT_EQ(decoder.collectNextByte(), uncompressedSpan[i]);
    }
}

TEST_F(ConverterHuffmanCoder, EncodingEdgeCase)
{
    // create the data
    uncompressedSpan = uncompressedSpan.subspan(0U, 1146U);
    std::fill(uncompressedSpan.begin(), uncompressedSpan.end(), 0xFFU);
    uncompressedSpan[0U] = 0U;
    uncompressedSpan[1U] = 0U;
    uncompressedSpan[2U] = 0U;
    uncompressedSpan[3U] = 117U;
    uncompressedSpan[4U] = 253U;
    uncompressedSpan[5U] = 254U;

    EXPECT_TRUE(encoder.compress(uncompressedSpan));

    auto const expectedSize = encoder.expectedSize();
    size_t     compressedSize{};
    size_t     encodedBytes{};
    while (encodedBytes = encoder.collectCompressedData(compressedSpan))
    {
        compressedSize += encodedBytes;
        compressedSpan = compressedSpan.subspan(encodedBytes);
    }

    EXPECT_EQ(expectedSize, compressedSize);
}

} // namespace Terrahertz::UnitTests
