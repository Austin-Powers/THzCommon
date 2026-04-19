#ifndef THZ_COMMON_MEMORY_ADDRESSHELPER_HPP
#define THZ_COMMON_MEMORY_ADDRESSHELPER_HPP

namespace Terrahertz {

/// @brief Checks if the two given addresses are the same.
///
/// @tparam TA The type of the first address object.
/// @tparam TB The type of the sceond address object.
/// @param addressA The first address.
/// @param addressB The second address.
/// @return True if the addresses are the same, false otherwise.
template <typename TA, typename TB>
[[nodiscard]] inline bool sameAddress(TA const *const addressA, TB const *const addressB) noexcept
{
    auto a = reinterpret_cast<unsigned char const *const>(addressA);
    auto b = reinterpret_cast<unsigned char const *const>(addressB);
    return a == b;
}

} // namespace Terrahertz

#endif // !THZ_COMMON_MEMORY_ADDRESSHELPER_HPP
