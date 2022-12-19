#ifndef THZ_COMMON_UTILITY_CONCEPTS_HPP
#define THZ_COMMON_UTILITY_CONCEPTS_HPP

#include <concepts>

namespace Terrahertz {

// clang-format off

/// @brief Concept of a type that can be multiplied by another type.
template <typename TFactorA, typename TFactorB>
concept MultiplyableBy = requires(TFactorA a, TFactorB b)
{
    {a * b};
};

/// @brief Deduces the return type of the multiplication of the two given types.
/// 
/// @tparam TFactorA The type of factor A.
/// @tparam TFactorB The type of factor A.
template<typename TFactorA, typename TFactorB>
struct MultiplicationResult
{
    static_assert(MultiplyableBy<TFactorA, TFactorB>, "Factors cannot be multiplied");

    using type = decltype(std::declval<TFactorA>() * std::declval<TFactorB>());
};

/// @brief Shortcut to MultiplicationResult::type
template<typename TFactorA, typename TFactorB>
using MultiplicationResult_t = typename MultiplicationResult<TFactorA, TFactorB>::type;

/// @brief Concept of a container a span can be created from.
template <typename TContainerType>
concept Container = requires(TContainerType container)
{
    typename TContainerType::pointer;
    {container.data()} -> std::same_as<typename TContainerType::pointer>;
    {container.size()} -> std::integral;
};
// clang-format on

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_CONCEPTS_HPP
