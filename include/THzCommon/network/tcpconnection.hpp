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
    /// @brief The role an end of the connection takes.
    enum class Role
    {
        /// @brief This end will connect to a server.
        Client,

        /// @brief This end will accept client connections.
        Server
    };

    /// @brief Initializes a new connection using the given IPv4 address.
    ///
    /// @param role The role of this end of the connection.
    /// @param address The address to connect/bind to.
    TCPConnection(Role const role, Address<TVersion> const &address) noexcept;

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
    /// @brief Establishes the connection.
    void establish() noexcept;

    /// @brief The role of this end of the connection.
    Role _role{};

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
