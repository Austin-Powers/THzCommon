#ifndef THZ_COMMON_NETWORK_UDPSOCKET_HPP
#define THZ_COMMON_NETWORK_UDPSOCKET_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/socketbase.hpp"
#include "THzCommon/utility/result.hpp"

#include <cstddef>
#include <span>

namespace Terrahertz {

/// @brief Encapsulates necesarry code to send and receive data via User Datagram Protocol (UDP).
///
/// @tparam TVersion The version of the internet protocol.
template <IPVersion TVersion>
class UDPSocket : public Internal::SocketBase<TVersion, Protocol::UDP>
{
    using base_t = Internal::SocketBase<TVersion, Protocol::UDP>;

public:
    using base_t::base_t;
    using base_t::bind;
    using base_t::close;
    using base_t::good;

    /// @brief Receives data from somewhere.
    ///
    /// @param address The address the data was received from, nullptr if address is irrelevant.
    /// @param buffer A span to put the received data into.
    /// @return The part of the buffer that was filled with received data.
    Result<std::span<std::byte>> receiveFrom(Address<TVersion> *address, std::span<std::byte> buffer) noexcept;

    /// @brief Sends data to the given address.
    ///
    /// @param address The address to send the data to.
    /// @param buffer A span to read the data to send from.
    /// @return The number of transmitted bytes.
    Result<std::size_t> sendTo(Address<TVersion> const &address, std::span<std::byte const> buffer) noexcept;
};

extern template class UDPSocket<IPVersion::V4>;
extern template class UDPSocket<IPVersion::V6>;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_UDPSOCKET_HPP
