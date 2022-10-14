#ifndef THZ_COMMON_UTILITY_CONCEPTS_HPP
#define THZ_COMMON_UTILITY_CONCEPTS_HPP

#include <concepts>

namespace Terrahertz {

// clang-format off
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
