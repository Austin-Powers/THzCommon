#ifndef THZ_COMMON_UTILITY_BYTEORDER_HPP
#define THZ_COMMON_UTILITY_BYTEORDER_HPP

#include <cstdint>

namespace Terrahertz {

/// @brief Changes the byteorder of the given 32-bit unsigned integer.
///
/// @param input The integer to change.
/// @return The integer in the new byte order.
std::uint32_t flipByteOrder(std::uint32_t input) noexcept;

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_BYTEORDER_HPP
