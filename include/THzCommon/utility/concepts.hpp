#ifndef THZ_COMMON_UTILITY_CONCEPTS_HPPPP
#define THZ_COMMON_UTILITY_CONCEPTS_HPPPP

#include <concepts>

namespace Terrahertz {

// clang-format off

/// @brief Concept of a type that can be multiplied by another type and returns a result in its type.
template <typename TFactorA, typename TFactorB>
concept MultiplyableBy = requires(TFactorA a, TFactorB b)
{
    {a * b} -> std::same_as<TFactorA>;
};

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

#endif // !THZ_COMMON_UTILITY_CONCEPTS_HPPPP
