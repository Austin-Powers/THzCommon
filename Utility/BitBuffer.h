#ifndef THZ_COMMON_UTILITY_BITBUFFER_H
#define THZ_COMMON_UTILITY_BITBUFFER_H

#include <cstdint>
#include <gsl/gsl>

namespace Terrahertz {

/// @brief Encapsulates the code for reading bitwise from a byte buffer.
///
/// @remarks Reads bits in a given buffer first to last Byte and MSB to LSB.
class BitBufferReader final
{
public:
    /// @brief Default initializes a new BitBufferReader.
    BitBufferReader() noexcept = default;

    /// @brief Initializes a new BitBuffer using the given memory.
    ///
    /// @param buffer The memory to work with.
    BitBufferReader(gsl::span<std::uint8_t const> buffer) noexcept;

    /// @brief Returns the number of bits left to read in the buffer.
    ///
    /// @return The number of bits left for reading.
    size_t bitsLeft() const noexcept;

    /// @brief Reads the next bit from the buffer.
    ///
    /// @return The read bit.
    bool next() noexcept;

private:
    /// @brief The buffer to read the bits from.
    gsl::span<std::uint8_t const> _buffer{};

    /// @brief The next byte to read.
    std::uint8_t _nextByte{};

    /// @brief The number of bits left in _nextByte.
    std::uint8_t _bitsLeftInByte{};
};

/// @brief Encapsulates the code for writing bitwise from a byte buffer.
///
/// @remarks Writes bits in a given buffer first to last Byte and MSB to LSB.
class BitBufferWriter
{
public:
    /// @brief Default initializes a new BitBufferWriter.
    BitBufferWriter() noexcept = default;

    /// @brief Initializes a new BitBuffer using the given memory.
    ///
    /// @param buffer The memory to work with.
    BitBufferWriter(gsl::span<uint8_t> buffer) noexcept;

    /// @brief Returns the number of bits left to read/write.
    ///
    /// @return The number of bits left to read/write.
    size_t bitsLeft() const noexcept;

    /// @brief Writes the next bit to the buffer.
    ///
    /// @param bit The bit to write.
    /// @return True if writing was successful, false othwise.
    bool write(bool bit) noexcept;

    /// @brief Writes a number of bits to the buffer.
    ///
    /// @param bytes The span containing the bits to write.
    /// @param count The number of bits to write.
    /// @return The number of bits written to the buffer.
    size_t write(gsl::span<uint8_t const> const bytes, size_t count) noexcept;

    /// @brief Writes a number of bits to the buffer.
    ///
    /// @param bytes The span containing the bits to write.
    /// @param count The number of bits to write.
    /// @param offset The offset in the given buffer.
    /// @return The number of bits written to the buffer.
    size_t write(gsl::span<uint8_t const> const bytes, size_t count, size_t offset) noexcept;

private:
    /// @brief The memory this BitBuffer is working on.
    gsl::span<uint8_t> _buffer{};

    /// @brief The position of the next bit to write in the next byte.
    std::uint8_t _nextBitInByte{};

    /// @brief The mask for writing the next bit.
    std::uint8_t _mask{0b1000'0000};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_BITBUFFER_H
