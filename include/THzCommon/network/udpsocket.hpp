#ifndef THZ_COMMON_NETWORK_UDPSOCKET_HPP
#define THZ_COMMON_NETWORK_UDPSOCKET_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/socketbase.hpp"

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

    void receiveFrom(Address<TVersion> const &address) noexcept;

    void sendTo(Address<TVersion> const &address) noexcept;
};

extern template class UDPSocket<IPVersion::V4>;
extern template class UDPSocket<IPVersion::V6>;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_UDPSOCKET_HPP
