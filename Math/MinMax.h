#ifndef THZ_COMMON_UTILITY_MINMAX_H
#define THZ_COMMON_UTILITY_MINMAX_H

#include <type_traits>

namespace Terrahertz {

/// @brief Compares the two given values and returns the smallest one.
///
/// @tparam TType The type of the values to compare.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @returns The smallest of the two given values.</returns>
template <typename TType>
constexpr TType minimum(TType a, TType b) noexcept
{
    return a > b ? b : a;
}

/// @brief Compares the two given values and returns the smallest one.
///
/// @tparam TTypeA The type of the first value.
/// @tparam TTypeB The type of the second value.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @returns The smallest of the two given values.
template <typename TTypeA, typename TTypeB>
constexpr TTypeA minimum(TTypeA a, TTypeB b) noexcept
{
    static_assert(std::is_pointer_v<TTypeA> == std::is_pointer_v<TTypeB>,
                  "minimum function called using pointer and non pointer type.");
    auto bCpy = static_cast<TTypeA>(b);
    return a > bCpy ? bCpy : a;
}

/// @brief Compares the given values and returns the smallest one.
///
/// @tparam TTypeA The type of the first value.
/// @tparam TTypeB The type of the second value.
/// @tparam TOtherTypes The type of the other values.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @param o The other values to compare.
/// @returns The smallest of the given values.
template <typename TTypeA, typename TTypeB, typename... TOtherTypes>
constexpr TTypeA minimum(TTypeA a, TTypeB b, TOtherTypes... o) noexcept
{
    return minimum(a, minimum(b, o...));
}

/// @brief Compares the two given values and returns the biggest one.
///
/// @tparam TType The type of the values to compare.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @returns The biggest of the two given values.
template <typename TType>
constexpr TType maximum(TType a, TType b) noexcept
{
    return a < b ? b : a;
}

/// @brief Compares the two given values and returns the biggest one.
///
/// @tparam TTypeA The type of the first value.
/// @tparam TTypeB The type of the second value.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @returns The biggest of the two given values.
template <typename TTypeA, typename TTypeB>
constexpr TTypeA maximum(TTypeA a, TTypeB b) noexcept
{
    static_assert(std::is_pointer_v<TTypeA> == std::is_pointer_v<TTypeB>,
                  "maximum function called using pointer and non pointer type.");
    auto bCpy = static_cast<TTypeA>(b);
    return a < bCpy ? bCpy : a;
}

/// @brief Compares the given values and returns the biggest one.
///
/// @tparam TTypeA The type of the first value.
/// @tparam TTypeB The type of the second value.
/// @tparam TOtherTypes The type of the other values.
/// @param a The first value to compare.
/// @param b The second value to compare.
/// @param o The other values to compare.
/// @returns The biggest of the given values.
template <typename TTypeA, typename TTypeB, typename... TOtherTypes>
constexpr TTypeA maximum(TTypeA a, TTypeB b, TOtherTypes... o) noexcept
{
    return maximum(a, maximum(b, o...));
}

/// @brief Clamps the given value between the min and max value.
///
/// @tparam TType The type of values to compare.
/// @param value The value to clamp.
/// @param min The minimum of the value.
/// @param max The maximum of the value.
/// @return The clamped value.
template <typename TType>
constexpr TType clamp(TType value, TType min, TType max) noexcept
{
    return minimum(maximum(value, min), max);
}

/// @brief Clamps the given value between the min and max value.
///
/// @tparam TValueType The type of the value to clamp.
/// @tparam TMinType The type of the min value.
/// @tparam TMaxType The type of the max value.
/// @param value The value to clamp.
/// @param min The minimum of the value.
/// @param max The maximum of the value.
/// @return The clamped value.
/// @remark In case max is smaller than min, max is returned.
template <typename TValueType, typename TMinType, typename TMaxType>
constexpr TValueType clamp(TValueType value, TMinType min, TMaxType max) noexcept
{
    return minimum(maximum(value, min), max);
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_MINMAX_H
