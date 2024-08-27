#ifndef THZ_COMMON_NETWORK_TCPSOCKET_HPP
#define THZ_COMMON_NETWORK_TCPSOCKET_HPP

#include "THzCommon/network/socketbase.hpp"

namespace Terrahertz {

/// @brief Encapsulates necesarry code to send and receive data via Transmission Control Protocol (TCP).
class TCPSocket : public Internal::SocketBase
{
    using base_t = Internal::SocketBase;

public:
    using base_t::base_t;
    using base_t::bind;
    using base_t::close;
    using base_t::good;

    void listen() noexcept;

    void accept() noexcept;

    void shutdown() noexcept;

    void send() noexcept;

    void receive() noexcept;
};

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_TCPSOCKET_HPP
