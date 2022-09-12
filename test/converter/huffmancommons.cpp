#include "Converter/HuffmanCommons.h"

#include "Structures/Stack.h"
#include "Utility/BitBuffer.h"

#include <array>
#include <gtest/gtest.h>
#include <memory>

namespace Terrahertz::UnitTests {

struct Converter_HuffmanCommons : public testing::Test
{
    Huffman::SymbolDistribution symbolDistribution{};

    using CodeArray = std::array<std::uint8_t, 16U>;

    std::array<CodeArray, 0x100U> codes{};

    void createCodes() noexcept
    {
        struct Node
        {
            std::uint16_t parentIndex{0xFFFFU};
            std::uint16_t child0Index{0xFFFFU};
            std::uint16_t child1Index{0xFFFFU};
            size_t        count{};
        };

        auto const tree = std::make_unique<std::array<Node, 0x1FFU>>();
        for (auto i = 0U; i < 0x100U; ++i)
        {
            (*tree)[i].count = symbolDistribution[i];
        }
        for (auto i = 0x100U; i < 0x1FFU; ++i)
        {
            auto &node = (*tree)[i];

            auto const countOfNode = [&](std::uint16_t const index) noexcept -> size_t { return (*tree)[index].count; };
            auto const shiftIndex  = [&](std::uint16_t const index) noexcept {
                if (node.child0Index == 0xFFFFU)
                {
                    node.child0Index = node.child1Index;
                }
                else if (countOfNode(node.child0Index) > countOfNode(node.child1Index))
                {
                    node.child0Index = node.child1Index;
                }
                node.child1Index = index;
            };
            auto const checkIndex = [&](std::uint16_t const index) noexcept -> bool {
                auto const &otherNode = (*tree)[index];
                if (otherNode.count == 0U)
                {
                    // ignore empty nodes
                    return false;
                }
                if (otherNode.parentIndex != 0xFFFFU)
                {
                    // otherNode already taken
                    return false;
                }
                if (node.child0Index == 0xFFFFU || node.child1Index == 0xFFFFU)
                {
                    // node missing children
                    return true;
                }
                auto const child0Count = countOfNode(node.child0Index);
                auto const child1Count = countOfNode(node.child1Index);
                auto const otherCount  = otherNode.count;
                return (child0Count > otherCount) || (child1Count > otherCount);
            };

            for (auto j = 0U; j < i; ++j)
            {
                if (checkIndex(j))
                {
                    shiftIndex(j);
                }
            }

            auto &child0 = (*tree)[node.child0Index];
            auto &child1 = (*tree)[node.child1Index];
            node.count   = child0.count + child1.count;

            child0.parentIndex = i;
            child1.parentIndex = i;
        }

        for (auto i = 0U; i < 0x100U; ++i)
        {
            Stack<uint8_t, 256U> stack{};
            for (auto index = i; (*tree)[index].parentIndex != 0xFFFFU; index = (*tree)[index].parentIndex)
            {
                auto const &node   = (*tree)[index];
                auto const &parent = (*tree)[node.parentIndex];
                if (parent.child1Index == index)
                {
                    stack.push(0x1U);
                }
                else
                {
                    stack.push(0x0U);
                }
            }
            BitBufferWriter bb{codes.at(i)};
            while (stack.filled() != 0U)
            {
                bb.write(stack.pop() == 0x1U);
            }
        }
    }
};

TEST_F(Converter_HuffmanCommons, EncodingReplicatesCodesIfNoDistributionWasGiven)
{
    auto const table = std::make_unique<Huffman::CodeTable>();

    std::array<std::uint8_t, 1U> array{};
    gsl::span<std::uint8_t>      span{array};
    for (auto i = 0U; i < 256U; ++i)
    {
        BitBufferWriter buffer{span};
        EXPECT_EQ(table->encode(buffer, static_cast<std::uint8_t>(i)), 0);
        EXPECT_EQ(array[0], i);
    }
}

TEST_F(Converter_HuffmanCommons, EncodingReplicatesCodesOnEvenDistribution)
{
    for (auto &symbol : symbolDistribution)
    {
        symbol = 1U;
    }
    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    std::array<std::uint8_t, 1U> array{};
    gsl::span<std::uint8_t>      span{array};
    for (auto i = 0U; i < 256U; ++i)
    {
        BitBufferWriter buffer{span};
        EXPECT_EQ(table->encode(buffer, static_cast<uint8_t>(i)), 0);
        EXPECT_EQ(array[0], i);
    }
}

TEST_F(Converter_HuffmanCommons, EncodingRejectsSymbolsThatHadZeroInTheDistribution)
{
    for (auto &symbol : symbolDistribution)
    {
        symbol = 1U;
    }
    symbolDistribution[12U]  = 0U;
    symbolDistribution[16U]  = 0U;
    symbolDistribution[222U] = 0U;

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    std::array<std::uint8_t, 1U> array{};
    gsl::span<std::uint8_t>      span{array};
    BitBufferWriter              buffer{span};
    EXPECT_EQ(table->encode(buffer, 12U), -1);
    EXPECT_EQ(array[0], 0U);
    EXPECT_EQ(table->encode(buffer, 16U), -1);
    EXPECT_EQ(array[0], 0U);
    EXPECT_EQ(table->encode(buffer, 222U), -1);
    EXPECT_EQ(array[0], 0U);
}

TEST_F(Converter_HuffmanCommons, EncodingResultsInCorrectCodes)
{
    for (auto i = 0ULL; i < symbolDistribution.size(); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    CodeArray codeArray{};
    createCodes();
    for (auto i = 0U; i < 0x100U; ++i)
    {
        std::memset(codeArray.data(), 0x0U, codeArray.size());
        BitBufferWriter writer{codeArray};
        EXPECT_EQ(table->encode(writer, i), 0);
        for (auto j = 0U; j < codeArray.size(); ++j)
        {
            EXPECT_EQ(codeArray[j], codes[i][j]);
        }
    }
}

TEST_F(Converter_HuffmanCommons, EncodingReturnsLeftoverBitsIfBufferIsTooSmall)
{
    for (auto i = 0ULL; i < symbolDistribution.size(); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    CodeArray codeArray{};
    createCodes();
    BitBufferWriter writer{codeArray};

    auto bits = writer.bitsLeft();
    EXPECT_EQ(table->encode(writer, static_cast<std::uint8_t>(1U)), 0);
    auto const bitsPerSymbol = bits - writer.bitsLeft();
    bits -= bitsPerSymbol;
    while (bits > bitsPerSymbol)
    {
        EXPECT_EQ(table->encode(writer, static_cast<std::uint8_t>(1U)), 0);
        bits -= bitsPerSymbol;
        EXPECT_EQ(writer.bitsLeft(), bits);
    }
    EXPECT_EQ(table->encode(writer, static_cast<std::uint8_t>(1U)), bitsPerSymbol - bits);
}

TEST_F(Converter_HuffmanCommons, EncodingOffsetWorksAsIntended)
{
    for (auto i = 0ULL; i < symbolDistribution.size(); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    createCodes();

    auto const symbol = static_cast<std::uint8_t>(1U);
    CodeArray  codeArrayExp{};
    CodeArray  codeArrayAct{};

    BitBufferWriter writerExp{codeArrayExp};
    EXPECT_EQ(table->encode(writerExp, symbol), 0);

    auto actualSpan0 = gsl::span<std::uint8_t>{codeArrayAct};
    auto actualSpan1 = actualSpan0.subspan(1);
    actualSpan0      = actualSpan0.subspan(0, 1);
    BitBufferWriter writerAct0{actualSpan0};
    auto const      offset = table->encode(writerAct0, symbol);
    BitBufferWriter writerAct1{actualSpan1};
    EXPECT_EQ(table->encode(writerAct1, symbol, offset), 0);
    for (auto i = 0U; i < codeArrayAct.size(); ++i)
    {
        EXPECT_EQ(codeArrayExp[i], codeArrayAct[i]);
    }
}

TEST_F(Converter_HuffmanCommons, DecodingResultsInTheOriginalSymbols)
{
    for (auto i = 0ULL; i < symbolDistribution.size(); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    std::array<std::uint8_t, 5U> originalData{};
    std::memcpy(originalData.data(), "hallo", originalData.size());

    CodeArray       codedData{};
    BitBufferWriter bbw{codedData};
    for (auto const symbol : originalData)
    {
        ASSERT_EQ(table->encode(bbw, symbol), 0U);
    }

    BitBufferReader reader{codedData};
    for (auto const symbol : originalData)
    {
        EXPECT_EQ(symbol, table->decode(reader));
    }
}

TEST_F(Converter_HuffmanCommons, DecodingResultsInTheOriginalSymbolsWithUnusedSymbols)
{
    for (auto i = 4ULL; i < (symbolDistribution.size() - 4U); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);

    std::array<std::uint8_t, 5U> originalData{};
    std::memcpy(originalData.data(), "hallo", originalData.size());

    CodeArray       codedData{};
    BitBufferWriter bbw{codedData};
    for (auto const symbol : originalData)
    {
        ASSERT_EQ(table->encode(bbw, symbol), 0U);
    }

    BitBufferReader reader{codedData};
    for (auto const symbol : originalData)
    {
        EXPECT_EQ(symbol, table->decode(reader));
    }
}

TEST_F(Converter_HuffmanCommons, TableWritingRejectTooSmallBuffer)
{
    for (auto i = 4ULL; i < (symbolDistribution.size() - 4U); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);
    CodeArray               buffer{};
    gsl::span<std::uint8_t> span{buffer};
    EXPECT_EQ(span, table->write(span));
    for (auto const byte : buffer)
    {
        ASSERT_EQ(byte, 0x00U);
    }
}

TEST_F(Converter_HuffmanCommons, TableWritingResultAsExpeced)
{
    for (auto i = 4ULL; i < (symbolDistribution.size() - 4U); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);
    std::array<std::uint8_t, 0x7FFU> buffer{};
    gsl::span<std::uint8_t>          span{buffer};

    auto const remainingSpan = table->write(span);
    auto const size          = (static_cast<std::uint16_t>(buffer[1U]) << 8U) | buffer[0U];
    EXPECT_EQ(remainingSpan.size(), span.size() - (size & 0xFFF));
}

TEST_F(Converter_HuffmanCommons, TableReadingRejectsCorruptData)
{
    for (auto i = 4ULL; i < (symbolDistribution.size() - 4U); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);
    std::array<std::uint8_t, 0x7FFU> buffer{};
    gsl::span<std::uint8_t>          span{buffer};

    EXPECT_NE(table->write(span), span);
    buffer[200U] = 0U;

    auto const table2 = std::make_unique<Huffman::CodeTable>();
    EXPECT_EQ(table2->read(span), gsl::span<std::uint8_t const>{span});
}

TEST_F(Converter_HuffmanCommons, TableReadingRecreatesStateOfTheOfTheWritingTable)
{
    for (auto i = 4ULL; i < (symbolDistribution.size() - 4U); ++i)
    {
        symbolDistribution[i] = i + 1U;
    }

    auto const table = std::make_unique<Huffman::CodeTable>();
    table->create(symbolDistribution);
    std::array<std::uint8_t, 0x7FFU> buffer{};
    gsl::span<std::uint8_t>          span{buffer};

    EXPECT_NE(table->write(span), span);
    auto const table2 = std::make_unique<Huffman::CodeTable>();
    EXPECT_NE(table2->read(span), gsl::span<std::uint8_t const>{span});

    CodeArray tableResult{};
    CodeArray table2Result{};

    gsl::span<std::uint8_t> resultSpan{tableResult};
    gsl::span<std::uint8_t> result2Span{table2Result};
    for (auto symbol = 0U; symbol < 0x100U; ++symbol)
    {
        auto encodeResult = 0;
        if ((symbol < 4U) || (symbol >= (symbolDistribution.size() - 4U)))
        {
            encodeResult = -1;
        }
        std::memset(tableResult.data(), 0x0U, tableResult.size());
        std::memset(table2Result.data(), 0x0U, table2Result.size());
        BitBufferWriter tableWriter{resultSpan};
        BitBufferWriter table2Writer{result2Span};
        ASSERT_EQ(table->encode(tableWriter, symbol), encodeResult);
        ASSERT_EQ(table2->encode(table2Writer, symbol), encodeResult);

        ASSERT_EQ(tableResult, table2Result);
    }
}

} // namespace Terrahertz::UnitTests
