#ifndef THZ_COMMON_UTILITY_TIME_HPP
#define THZ_COMMON_UTILITY_TIME_HPP

#include <chrono>
#include <string>

namespace Terrahertz {

/// @brief Shortcut to the system_clock::now function.
#define SystemNow std::chrono::system_clock::now()

/// @brief Shortcut for milliseconds.
using Milliseconds = std::chrono::milliseconds;

/// @brief Shortcut for the system_clock time_point.
using SystemTimePoint = std::chrono::time_point<std::chrono::system_clock>;

/// @brief Retrieves the current time as an iso timestamp.
///
/// @returns String containing the timestamp.
std::string currentTimestampString() noexcept;

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_TIME_HPP
