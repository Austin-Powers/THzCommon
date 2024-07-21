#ifndef THZ_COMMON_NETWORK_ADDRESS_HPP
#define THZ_COMMON_NETWORK_ADDRESS_HPP

#include "THzCommon/network/native.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

namespace Terrahertz {

/// @brief Enumerates the version of the IP protocol.
enum class IPVersion
{
    V4 = 4U,
    V6 = 6U
};

namespace Detail {

template <IPVersion TIPV>
struct IPAddress;

template <>
struct IPAddress<IPVersion::V4> final
{
    using type = std::array<std::uint8_t, 4U>;
};
using IPV4Address = IPAddress<IPVersion::V4>::type;

template <>
struct IPAddress<IPVersion::V6> final
{
    using type = std::array<std::uint16_t, 8U>;
};
using IPV6Address = IPAddress<IPVersion::V6>::type;

using IPV_Address = std::variant<IPV4Address, IPV6Address>;

} // namespace Detail

/// @brief Combines IP address and port.
///
/// @tparam TIPV The version of the IP protocol.
template <IPVersion TIPV>
struct Address final
{
    /// @brief The IP address of the machine.
    Detail::IPAddress<TIPV> ipAddress;

    /// @brief The port of the application.
    std::uint16_t port;
};

/// @brief Resolves the given address to a list of suitable IP addresses.
///
/// @param address The address to resolve.
/// @return A vector of ip addresses, if successful.
std::optional<std::vector<Detail::IPV_Address>>
resolveIPAddresses(std::string_view address, Native::SocketApi const & = Native::SocketApi::instance()) noexcept;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_ADDRESS_HPP
