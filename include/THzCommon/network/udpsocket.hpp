#ifndef THZ_COMMON_NETWORK_UDPSOCKET_HPP
#define THZ_COMMON_NETWORK_UDPSOCKET_HPP

#include "THzCommon/network_new/socketbase.hpp"

namespace Terrahertz {

/// @brief Encapsulates necesarry code to send and receive data via User Datagram Protocol (UDP).
class UDPSocket : public Internal::SocketBase
{
    using base_t = Internal::SocketBase;

public:
    using base_t::base_t;
    using base_t::bind;
    using base_t::close;
    using base_t::good;

    void receiveFrom(Address const &address) noexcept;

    void sendTo(Address const &address) noexcept;
};

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_UDPSOCKET_HPP
