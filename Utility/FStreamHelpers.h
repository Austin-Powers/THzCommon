#ifndef THZ_COMMON_UTILITY_FSTREAMHELPERS_H
#define THZ_COMMON_UTILITY_FSTREAMHELPERS_H

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

/// @brief Function wrapping the read method of the ifstream to make it more easy to use.
///
/// @tparam TType The type of data to write.
/// @param stream The stream to read the data from.
/// @param data The buffer for the read.
template <typename TType>
void readFromStream(std::ifstream &stream, TType &data) noexcept
{
    stream.read(std::bit_cast<char *>(&data), sizeof(TType));
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_FSTREAMHELPERS_H
