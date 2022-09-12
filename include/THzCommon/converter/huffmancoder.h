#ifndef THZ_COMMON_CONVERTER_HUFFMANCODER_H
#define THZ_COMMON_CONVERTER_HUFFMANCODER_H

#include "HuffmanCommons.h"
#include "THzCommon/utility/bitbuffer.h"

#include <cstdint>
#include <gsl/gsl>

namespace Terrahertz::Huffman {

/// @brief Encapsulated the code for performing a Huffman-Encoding.
class Encoder
{
public:
    /// @brief Hands in a reference to a buffer of data to compress.
    ///
    /// @param data The data to compress.
    /// @return True if compressions was initialized successfully, false otherwise.
    bool compress(gsl::span<std::uint8_t const> data) noexcept;

    /// @brief Returns the exptected size of the compressed data [bytes].
    ///
    /// @return The exptected size of the compressed data [bytes].
    size_t expectedSize() const noexcept;

    /// @brief Hands in a buffer to collect the compressed data.
    ///
    /// @param buffer The buffer for the compressed data.
    /// @return The number of bytes written to the buffer, 0 if compression finished.
    size_t collectCompressedData(gsl::span<std::uint8_t> buffer) noexcept;

private:
    /// @brief The code table for the encoding.
    CodeTable _table{};

    /// @brief The span of the data left to compress.
    gsl::span<std::uint8_t const> _data{};

    /// @brief The expected size of the compressed data [bytes].
    size_t _expectedSize{};

    /// @brief Flag signalling if the header has been written already.
    bool _headerWritten{};

    /// @brief The bits left over from the last encoded symbol.
    std::int16_t _leftoverBits{};
};

/// @brief Encapsulated the code for performing a Huffman-Decoding.
class Decoder
{
public:
    /// @brief Hands in a reference to a buffer of data to compress.
    ///
    /// @param buffer The data to compress.
    /// @return True if compressions was initialized successfully, false otherwise.
    bool decompress(gsl::span<std::uint8_t const> buffer) noexcept;

    /// @brief Hands in a buffer to collect the decompressed data.
    ///
    /// @param buffer The buffer for the decompressed data.
    /// @return The number of bytes written to the buffer, 0 if decompression finished.
    size_t collectDecompressedData(gsl::span<std::uint8_t> buffer) noexcept;

    /// @brief Returns the next decompressed byte.
    ///
    /// @return The decompressed byte, or 0 if there is no more data left.
    std::uint8_t collectNextByte() noexcept;

    /// @brief Returns the amount of bytes left to decompress.
    ///
    /// @return The amount of bytes left to decompress.
    size_t bytesLeft() const noexcept;

private:
    /// @brief The code table for decoding.
    CodeTable _table{};

    /// @brief The reader for the encoded bits.
    BitBufferReader _reader{};

    /// @brief The bytes left to decompress.
    size_t _bytesLeft{};
};

} // namespace Terrahertz::Huffman

#endif // !THZ_COMMON_CONVERTER_HUFFMANCODER_H
