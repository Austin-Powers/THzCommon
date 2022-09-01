#ifndef THZ_COMMON_UTILITY_SPANCAST_H
#define THZ_COMMON_UTILITY_SPANCAST_H

#include <bit>
#include <concepts>
#include <cstring>
#include <gsl/gsl>

namespace Terrahertz {

/// @brief Creates a gsl::span from a given pointer and size, performing all necessary casts.
///
/// @tparam TSpanType The value_type of the span.
/// @tparam TDataType The data type of the pointer to convert.
/// @tparam TSizeType The size type of the buffer.
/// @param pointer The pointer to the buffer.
/// @param size The size of the buffer.
/// @return The span of the buffer.
template <typename TSpanType, typename TDataType, typename TSizeType>
[[nodiscard]] inline gsl::span<TSpanType> toSpan(TDataType *pointer, TSizeType size) noexcept
{
    return gsl::span<TSpanType>{std::bit_cast<TSpanType *>(pointer), static_cast<size_t>(size)};
}

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

/// @brief Explicitly creates a gsl::span from a given container.
///
/// @tparam TSpanType The value_type of the span.
/// @tparam TContainerType The type of container to create the span from.
/// @param container The container to create the span from.
/// @return The span of the container.
template <typename TSpanType, Container TContainerType>
[[nodiscard]] inline gsl::span<TSpanType> toSpan(TContainerType &container) noexcept
{
    return gsl::span<TSpanType>{std::bit_cast<TSpanType *>(container.data()), static_cast<size_t>(container.size())};
}

/// @brief Writes the given instance to the given buffer.
///
/// @tparam TBufferDataType The type of data used by the buffer (has to be one byte wide).
/// @tparam TDataType The type the instance to write to the buffer.
/// @param buffer The buffer to write the data to.
/// @param instance The instance to write to the buffer.
/// @return The remaining buffer, if buffer was too small it remains unchanged.
template <typename TBufferDataType, typename TDataType>
[[nodiscard]] inline gsl::span<TBufferDataType> writeToSpan(gsl::span<TBufferDataType> buffer,
                                                            TDataType const           &instance) noexcept
{
    static_assert(sizeof(TBufferDataType) == 1U, "TBufferDataType has to be one byte wide");
    if (buffer.size() < sizeof(TDataType))
    {
        return buffer;
    }
    std::memcpy(buffer.data(), &instance, sizeof(TDataType));
    return buffer.subspan(sizeof(TDataType));
}

/// @brief Reads into the given instance from the given buffer.
///
/// @tparam TBufferDataType The type of data used by the buffer (has to be one byte wide).
/// @tparam TDataType The type the instance to read from the buffer.
/// @param buffer The buffer to read the data from.
/// @param instance The instance to read into from the buffer.
/// @return The remaining buffer, if buffer was too small it remains unchanged.
template <typename TBufferDataType, typename TDataType>
[[nodiscard]] inline gsl::span<TBufferDataType> readFromSpan(gsl::span<TBufferDataType> buffer,
                                                             TDataType                 &instance) noexcept
{
    static_assert(sizeof(TBufferDataType) == 1U, "TBufferDataType has to be one byte wide");
    if (buffer.size() < sizeof(TDataType))
    {
        return buffer;
    }
    std::memcpy(&instance, buffer.data(), sizeof(TDataType));
    return buffer.subspan(sizeof(TDataType));
}

/// @brief Tries to create a subspan of the given span, returns an empty span instead of crashing.
///
/// @tparam TDataType The data type of the span.
/// @param span The span to create the subspan of.
/// @param n The number of elements to exclude from the given span.
/// @return The subspan or empty span if n > span.size()
template <typename TDataType>
[[nodiscard]] inline gsl::span<TDataType> trySubspan(gsl::span<TDataType> const span, size_t const n) noexcept
{
    if (n > static_cast<size_t>(span.size()))
    {
        return {};
    }
    return span.subspan(n);
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_SPANCAST_H
