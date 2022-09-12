#ifndef THZ_COMMON_CONVERTER_HUFFMANCOMMONS_H
#define THZ_COMMON_CONVERTER_HUFFMANCOMMONS_H

#include "THzCommon/utility/bitbuffer.h"

#include <array>
#include <cstdint>
#include <gsl/gsl>

namespace Terrahertz::Huffman {

/// @brief Name provider for the Huffman project.
struct HuffmanProject
{
    static constexpr char const *name() noexcept { return "THzCommon.Converter.Huffman"; }
};

/// @brief The signature byte of the HuffmanCode block.
constexpr std::uint8_t CodeSignature{0xC3U};

#pragma pack(1) // otherwise the structs would turn out too large

/// @brief The header of a table of Huffman codes.
struct CodeHeader
{
    /// @brief The signature of the table 0xC3.
    std::uint8_t signature{};

    /// @brief Padding bytes for future use.
    std::uint8_t reserved[7]{};

    /// @brief The size of the compressed data [bytes].
    size_t compressedDataSize{};

    /// @brief The size of the uncompressed data [bytes].
    size_t uncompressedDataSize{};
};
static_assert(sizeof(CodeHeader) == 24U, "Huffman::CodeHeader wrong size");
#pragma pack()

/// @brief The type for a symbol distribution.
using SymbolDistribution = std::array<size_t, 0x100U>;

/// @brief Encapsulates the code table for the Huffman-Coding.
class CodeTable
{
public:
    /// @brief Default initializes a new CodeTable.
    CodeTable() noexcept;

    /// @brief Creates the table from the given symbol distribution.
    ///
    /// @param symbolDistribution The distribution of symbols the create the table from.
    void create(SymbolDistribution const &symbolDistribution) noexcept;

    /// @brief Encodes the given symbol and hands the result to the given buffer.
    ///
    /// @param buffer The buffer to add the encoding result to.
    /// @param symbol The symbol to encode.
    /// @return -1 if symbol was rejected, otherwise the number of bits that could not be fitted into the buffer.
    [[nodiscard]] std::int16_t encode(BitBufferWriter &buffer, std::uint8_t symbol) const noexcept;

    /// @brief Encodes the given symbol and hands the result to the given buffer.
    /// @param buffer The buffer to add the encoding result to.
    /// @param symbol The symbol to encode.
    /// @param lastBits The number of bits at the end of the symbol to write (leftover from a previous encoding).
    /// @return -1 if symbol was rejected, otherwise the number of bits that could not be fitted into the buffer.
    [[nodiscard]] std::int16_t encode(BitBufferWriter &buffer, std::uint8_t symbol, size_t lastBits) const noexcept;

    /// @brief Decodes the next byte from the given BitBufferReader.
    ///
    /// @param buffer The buffer to read the bits from.
    /// @return The decoded byte.
    [[nodiscard]] std::uint8_t decode(BitBufferReader &buffer) const noexcept;

    /// @brief Reset the table to identity encoding.
    void reset() noexcept;

    /// @brief Writes the table to the given buffer.
    ///
    /// @param buffer The buffer to write the table to.
    /// @return The remainder of the given buffer or the original buffer if it is too small.
    [[nodiscard]] gsl::span<std::uint8_t> write(gsl::span<std::uint8_t> buffer) const noexcept;

    /// @brief Reads the table from the given buffer.
    ///
    /// @param buffer The buffer to read the table from.
    /// @return The remaining buffer, or the original buffer if reading failed.
    [[nodiscard]] gsl::span<std::uint8_t const> read(gsl::span<std::uint8_t const> buffer) noexcept;

    /// @brief Calculates the bytes needed for the table and the compressed data based on the symbol distribution.
    ///
    /// @param symbolDistribution The distribution of symbols to calculate the expectation from.
    /// @return The bytes needed for the table and the compressed data based on the symbol distribution.
    [[nodiscard]] size_t calculateExpectation(SymbolDistribution const &symbolDistribution) const noexcept;

private:
    /// @brief A Huffman code for one symbol.
    struct Code
    {
        /// @brief Flag signalling if the symbol is present in the distribution the code is based on.
        bool present{true};

        /// @brief The length of the code [bits] - 1.
        std::uint8_t length{};

        /// @brief The array containing the code.
        std::array<std::uint8_t, 32U> array{};

        /// @brief The span of the array, to hand into the BitBuffer.
        gsl::span<std::uint8_t> span{array};
    };

    /// @brief A node in the Huffman-Tree.
    struct Node
    {
        /// @brief The amount of the symbols collected under this node.
        size_t amount{};

        /// @brief The id of the parent.
        std::uint16_t parent{};

        /// @brief The children of this node.
        std::array<std::uint16_t, 2U> children{};
    };

    /// @brief Calculates the length of the table [bytes] and writes the table to the given buffer, if not empty.
    ///
    /// @param buffer The buffer to write the table to.
    /// @param length Output: The length of the table [bytes].
    /// @return The remaining space of the given buffer, or the buffer if too small.
    gsl::span<std::uint8_t> write(gsl::span<std::uint8_t> buffer, size_t &length) const noexcept;

    /// @brief Resets the Huffman-Tree.
    void resetTree() noexcept;

    /// @brief Creates the code table from the Huffman-Tree.
    void createTableFromTree() noexcept;

    /// @brief Creates the Huffman-Tree from the code table.
    ///
    /// @return True if the creation was succesful, false otherwise.
    bool createTreeFromTable() noexcept;

    /// @brief The codes for encoding symbols.
    std::array<Code, 0x100U> _codes{};

    /// @brief The Huffman-Tree.
    std::array<Node, 0x1FFU> _tree{};

    /// @brief The index of the root element of the tree.
    std::uint16_t _rootIndex{};
};

} // namespace Terrahertz::Huffman

#endif // !THZ_COMMON_CONVERTER_HUFFMANCOMMONS_H
