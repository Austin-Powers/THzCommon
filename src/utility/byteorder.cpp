#include "THzCommon/utility/byteorder.hpp"

#include <bit>

namespace Terrahertz {

std::uint32_t flipByteOrder(std::uint32_t input) noexcept
{
    std::uint32_t output{};

    auto inArr = std::bit_cast<std::uint8_t *>(&input);
    auto ouArr = std::bit_cast<std::uint8_t *>(&output);

    ouArr[0] = inArr[3];
    ouArr[1] = inArr[2];
    ouArr[2] = inArr[1];
    ouArr[3] = inArr[0];
    return output;
}

} // namespace Terrahertz
