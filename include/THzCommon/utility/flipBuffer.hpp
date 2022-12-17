#ifndef THZ_COMMON_UTILITY_FLIPBUFFER_H
#define THZ_COMMON_UTILITY_FLIPBUFFER_H

#include "THzCommon/math/rectangle.hpp"

#include <gsl/gsl>

namespace Terrahertz {

/// @brief Flips the given buffer along the X-axis.
///
/// @tparam TValueType The value type of the buffer.
/// @param buffer The buffer to flip.
/// @param dimensions The dimensions of the buffer.
/// @return False in case dimensions.area() does not match buffer.size(), true otherwise.
template <typename TValueType>
bool flipBufferHorizontally(gsl::span<TValueType> buffer, Rectangle const &dimensions) noexcept
{
    if (buffer.size() != dimensions.area())
    {
        return false;
    }

    TValueType temp{};
    for (auto y = 0U; y < (dimensions.height / 2U); ++y)
    {
        auto upperRow = buffer.data() + (dimensions.width * y);
        auto lowerRow = buffer.data() + (dimensions.width * (dimensions.height - y - 1U));
        for (auto x = 0U; x < dimensions.width; ++x)
        {
            temp        = (*upperRow);
            (*upperRow) = (*lowerRow);
            (*lowerRow) = temp;
            ++upperRow;
            ++lowerRow;
        }
    }
    return true;
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_FLIPBUFFER_H
