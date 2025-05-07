#ifndef THZ_COMMON_NETWORK_CONNECTION_HPP
#define THZ_COMMON_NETWORK_CONNECTION_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/tcpsocket.hpp"
#include "THzCommon/utility/result.hpp"

#include <span>

namespace Terrahertz {

/// @brief Wrapper for handling a TCP connection.
///
/// @tparam TVersion The version of the internet protocol.
template <IPVersion TVersion>
class TCPConnection
{
public:
    /// @brief Initializes a new connection using the given IPv4 address.
    ///
    /// @param address The address to connect/bind to.
    /// @param server True if the connection assumes the role of a server, false for client.
    TCPConnection(Address<TVersion> const &address, bool const server = false) noexcept;

    /// @brief Establishes the connection, if not already established.
    ///
    /// @return True if the connection is established, false otherwise.
    bool establish() noexcept;

    /// @brief Sends the content of the given buffer via the connection, if established.
    ///
    /// @param buffer The buffer containing the data to send.
    /// @return The amount of bytes send, if sending was successful.
    Result<size_t> send(std::span<std::uint8_t const> const buffer) noexcept;

    /// @brief Receives data through the connection, if established, and stores it in the given buffer.
    ///
    /// @param buffer The buffer to store the received data in.
    /// @return The part of the buffer that was filled with the received data, if successful.
    Result<std::span<std::uint8_t>> receive(std::span<std::uint8_t> buffer) noexcept;

private:
    /// @brief True if this connection acts as a server, false for client.
    bool _server{};

    /// @brief The address to connect/bind to.
    Address<TVersion> _address{};

    /// @brief The server socket to establish a connection.
    TCPSocket<TVersion> _serverSocket{};

    /// @brief The socket used by the connection.
    TCPSocket<TVersion> _socket{};
};

extern template class TCPConnection<IPVersion::V4>;
extern template class TCPConnection<IPVersion::V6>;

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_CONNECTION_HPP
