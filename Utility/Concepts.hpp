#include <concepts>

// clang-format off
/// @brief Concept of a container a span can be created from.
template <typename TContainerType>
concept Container = requires(TContainerType container)
{
    {typename TContainerType::value_type};
    {container.data()} -> std::same_as<typename TContainerType::value_type *>;
    {container.size()} -> std::integral;
};
// clang-format on
