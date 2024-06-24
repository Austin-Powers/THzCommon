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
size_t decodedSize(size_t byteCount) noexcept;

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

} // namespace Terrahertz::Base64

#endif // !THZ_COMMON_CONVERTER_BASE64_HPP
