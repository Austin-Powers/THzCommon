#ifndef THZ_COMMON_NETWORK_ADDRESS_HPP
#define THZ_COMMON_NETWORK_ADDRESS_HPP

#include "THzCommon/network/common.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

namespace Terrahertz {

/// @brief Combines IP address and port.
///
/// @tparam TIPV The version of the IP protocol.
template <IPVersion TIPV>
struct Address final
{
    /// @brief The IP address of the machine.
    Internal::IPAddress<TIPV>::type ipAddress{};

    /// @brief The port of the application.
    std::uint16_t port{};
};

using IPV_Addresses = std::vector<std::variant<Internal::IPV4Address, Internal::IPV6Address>>;

/// @brief Resolves the given address to a list of suitable IP addresses.
///
/// @param address The address to resolve.
/// @return A vector of ip addresses, if successful.
std::optional<IPV_Addresses> resolveIPAddresses(std::string_view address,
                                                Internal::SocketApi const & = Internal::SocketApi::instance()) noexcept;

/// @brief Returns the first IPV4Address from the given list of addresses, if possible.
///
/// @param addresses The list of addresses to extract the address from.
/// @return The first IPV4Address from the given list, if there is one.
std::optional<Internal::IPV4Address> getFirstIPV4From(std::optional<IPV_Addresses> const &addresses) noexcept;

/// @brief Returns the first IPV6Address from the given list of addresses, if possible.
///
/// @param addresses The list of addresses to extract the address from.
/// @return The first IPV6Address from the given list, if there is one.
std::optional<Internal::IPV6Address> getFirstIPV6From(std::optional<IPV_Addresses> const &addresses) noexcept;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_ADDRESS_HPP
