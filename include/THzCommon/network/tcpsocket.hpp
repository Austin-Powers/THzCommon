#ifndef THZ_COMMON_NETWORK_TCPSOCKET_HPP
#define THZ_COMMON_NETWORK_TCPSOCKET_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/socketbase.hpp"
#include "THzCommon/utility/result.hpp"

#include <cstddef>
#include <span>

namespace Terrahertz {

/// @brief Encapsulates necesarry code to send and receive data via Transmission Control Protocol (TCP).
///
/// @tparam TVersion The version of the internet protocol.
template <IPVersion TVersion>
class TCPSocket : public Internal::SocketBase<TVersion, Protocol::TCP>
{
    using base_t = Internal::SocketBase<TVersion, Protocol::TCP>;

public:
    using base_t::base_t;
    using base_t::bind;
    using base_t::close;
    using base_t::good;

    /// @brief Sets socket up to listen for connections.
    ///
    /// @param backlog The maximum number of queued connections.
    /// @return True if operation was successfull, false otherwise.
    bool listen(std::uint32_t backlog) noexcept;

    /// @brief Accept an incoming connection.
    ///
    /// @param address The address of the connecting client, nullptr if irrelevant.
    /// @return The socket for the new connection, socket will not be good if calls fails.
    TCPSocket accept(Address<TVersion> *address) noexcept;

    /// @brief Connects to a server.
    ///
    /// @param address The address of the server to connect to.
    /// @return True if connecting was successful, false otherwise.
    bool connect(Address<TVersion> const &address) noexcept;

    /// @brief Shuts down the connection, partially or entirely.
    ///
    /// @param what What part of the connection to shut down.
    /// @return True if operation was successful, false otherwise.
    bool shutdown(int what) noexcept;

    /// @brief Receives data from the connected peer.
    ///
    /// @param buffer The buffer for the received data.
    /// @return The part of the buffer that was filled with received data.
    Result<std::span<std::byte>> receive(std::span<std::byte> buffer) noexcept;

    /// @brief Sends data to the connected peer.
    ///
    /// @param buffer The buffer containing the data to send.
    /// @return The number of transmitted bytes.
    Result<std::size_t> send(std::span<std::byte const> buffer) noexcept;
};

extern template class TCPSocket<IPVersion::V4>;
extern template class TCPSocket<IPVersion::V6>;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_TCPSOCKET_HPP
