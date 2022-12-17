#ifndef THZ_COMMON_MATH_INRANGE_H
#define THZ_COMMON_MATH_INRANGE_H

#include <type_traits>

namespace Terrahertz {

/// @brief Checks if the given check is in the given range.
///
/// @tparam TType The type of the min, max and check.
/// @param check The value to check.
/// @param min The value smaller or equal to check.
/// @param max The value greater or equal to check.
/// @return True if check is within the range, false otherwise.
template <typename TType>
constexpr bool inRange(TType check, TType min, TType max) noexcept
{
    return min <= check && check <= max;
}

/// @brief Checks if the given check is in the given range.
///
/// @tparam TRangeType The type of the min and max.
/// @tparam TCheckType The type of the check.
/// @param check The value to check.
/// @param min The value smaller or equal to check.
/// @param max The value greater or equal to check.
/// @return True if check is within the range, false otherwise.
template <typename TRangeType, typename TCheckType>
constexpr bool inRange(TCheckType check, TRangeType min, TRangeType max) noexcept
{
    static_assert(std::is_pointer_v<TRangeType> == std::is_pointer_v<TCheckType>,
                  "inRange function called using pointer and non pointer type.");
    return min <= check && check <= max;
}

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_INRANGE_H
