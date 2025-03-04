#ifndef THZ_COMMON_CONVERTER_BASE64_HPP
#define THZ_COMMON_CONVERTER_BASE64_HPP

#include <cstdint>
#include <gsl/span>

namespace Terrahertz::Base64 {

/// @brief Calculates the encoded size for the given byte count.
///
/// @param byteCount The bytes to encode.
/// @return The number of symbols needed to encode the bytes.
size_t encodedSize(size_t byteCount) noexcept;

/// @brief Calculates maximum the decoded size for the given symbol count.
///
/// @param symbolCount The symbols to decode.
/// @return The maximum number of bytes needed to decode the symbols.
size_t decodedSize(size_t symbolCount) noexcept;

/// @brief Encodes the given input buffer into base64 and stores it in the output buffer.
///
/// @param input Span containing the data to encode.
/// @param output Span for the encoded data.
/// @return A subspan of output containing the encoded data, empty span in case of an error.
gsl::span<char> encode(gsl::span<std::uint8_t const> const input, gsl::span<char> output) noexcept;

/// @brief Decodes the given input buffer from base64 and stores the result in output buffer.
///
/// @param input Span containing the base 64 encoded data.
/// @param output Span for the decoded data.
/// @return A subspan of output containing the decoded data, or empty span if decoding failed.
gsl::span<std::uint8_t> decode(gsl::span<char const> const input, gsl::span<std::uint8_t> output) noexcept;

/// @brief Helps encode data into base64.
class Encoder final
{
public:
    /// @brief Initializes a new encoder.
    ///
    /// @param outputBuffer The output buffer to use.
    Encoder(gsl::span<char> outputBuffer) noexcept;

    /// @brief Writes the given data to the buffer.
    ///
    /// @tparam T The type of data to write.
    /// @param data The data to write.
    /// @return True if the data could be written, false otherwise.
    template <typename T>
    bool write(T const &data) noexcept
    {
        auto const buffer = reinterpret_cast<std::uint8_t const *>(&data);
        for (auto i = 0U; i < sizeof(T); ++i)
        {
            if (!writeByte(buffer[i]))
            {
                return false;
            }
        }
        return true;
    }

    /// @brief Writes the next byte to the buffer.
    ///
    /// @param byte The byte to add to the buffer.
    /// @return True if the byte could be written, false otherwise.
    bool writeByte(std::uint8_t const byte) noexcept;

private:
    /// @brief The buffer to store the bytes to encode next.
    std::array<std::uint8_t, 3U> _accumulator{};

    /// @brief The span of the _accumulator.
    gsl::span<std::uint8_t> _accSpan{};

    /// @brief The number of bytes accumulated.
    std::uint8_t _bytesAccumulated{};

    /// @brief The buffer to output the encoded data to.
    gsl::span<char> _outputBuffer{};
};

/// @brief Helps decode data from base64.
class Decoder final
{
public:
    /// @brief Initializes a new decoder.
    ///
    /// @param inputBuffer The input buffer to use.
    Decoder(gsl::span<char const> inputBuffer) noexcept;

    template <typename T>
    bool read(T &data) noexcept
    {
        auto const buffer = reinterpret_cast<std::uint8_t *>(&data);
        for (auto i = 0U; i < sizeof(T); ++i)
        {
            if (!dataAvailable())
            {
                return false;
            }
            buffer[i] = readByte();
        }
        return true;
    }

    /// @brief Checks if the buffer still has data to read.
    ///
    /// @return True if there is data left ot read.
    bool dataAvailable() const noexcept;

    /// @brief Reads the next byte from the buffer.
    ///
    /// @return The byte read.
    std::uint8_t readByte() noexcept;

private:
    /// @brief The buffer to store the decoded bytes.
    std::array<std::uint8_t, 3U> _accumulator{};

    /// @brief The span of the _accumulator.
    gsl::span<std::uint8_t> _accSpan{};

    /// @brief The index of the next byte to read from the accumulator.
    std::uint8_t _readIndex{3U};

    /// @brief The buffer to get the input data from.
    gsl::span<char const> _inputBuffer{};
};

} // namespace Terrahertz::Base64

#endif // !THZ_COMMON_CONVERTER_BASE64_HPP
