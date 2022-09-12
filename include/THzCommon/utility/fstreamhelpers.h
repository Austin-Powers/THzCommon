#ifndef THZ_COMMON_UTILITY_FSTREAMHELPERS_H
#define THZ_COMMON_UTILITY_FSTREAMHELPERS_H

#include "concepts.h"

#include <bit>
#include <fstream>

namespace Terrahertz {

/// @brief Function wrapping the write method of the ofstream to make it more easy to use.
///
/// @tparam TType The type of data to write.
/// @param stream The stream to write the data to.
/// @param data The data to write.
template <typename TType>
void writeToStream(std::ofstream &stream, TType const &data) noexcept
{
    stream.write(std::bit_cast<char const *>(&data), sizeof(TType));
}

/// @brief Function wrapping the write method of the ofstream to make it more easy to use.
///
/// @tparam TContainerType The type of container to write.
/// @param stream The stream to write the data to.
/// @param container The container whos content to write.
template <Container TContainerType>
void writeToStream(std::ofstream &stream, TContainerType const &container) noexcept
{
    stream.write(std::bit_cast<char const *>(container.data()), sizeof(TContainerType::value_type) * container.size());
}

/// @brief Function wrapping the read method of the ifstream to make it more easy to use.
///
/// @tparam TType The type of data to write.
/// @param stream The stream to read the data from.
/// @param data The buffer for the read.
/// @return True if reading was successful, false otherwise.
template <typename TType>
[[nodiscard]] bool readFromStream(std::ifstream &stream, TType &data) noexcept
{
    if (!stream.is_open())
    {
        return false;
    }
    return stream.read(std::bit_cast<char *>(&data), sizeof(TType)).gcount() == sizeof(TType);
}

/// @brief Function wrapping the read method of the ifstream to make it more easy to use.
///
/// @tparam TContainerType The type of container to read into.
/// @param stream The stream to read the data from.
/// @param data The buffer for the read.
/// @return The number of complete instance read from the file.
template <Container TContainerType>
[[nodiscard]] size_t readFromStream(std::ifstream &stream, TContainerType &container) noexcept
{
    if (!stream.is_open())
    {
        return 0;
    }
    auto const valueTypeSize = sizeof(TContainerType::value_type);
    auto const bytesToRead   = valueTypeSize * container.size();
    return stream.read(std::bit_cast<char *>(container.data()), bytesToRead).gcount() / valueTypeSize;
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_FSTREAMHELPERS_H
