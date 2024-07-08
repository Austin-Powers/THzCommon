#ifndef THZ_COMMON_NETWORK_SOCKETWIN32_HPP
#define THZ_COMMON_NETWORK_SOCKETWIN32_HPP

#ifdef _WIN32

#include <cstdint>
#include <string_view>

namespace Terrahertz {

/// @brief A network socket for Windows using TCP.
class SocketWin32
{
public:
    /// @brief The socket type on Win32.
    using SocketType = unsigned long long;

    /// @brief Initializes a new Socket.
    ///
    /// @exception system_error In case WSAStartup fails.
    SocketWin32() noexcept(false);

    /// @brief Finalizes this socket.
    ~SocketWin32() noexcept;

    /// @brief Connects to the given address and port.
    ///
    /// @param address The address of the machine to connect to.
    /// @param port The port on the machine to connect to.
    /// @return True if connection was established successfully, false otherwise.
    bool connectTo(std::string_view const address, std::uint16_t const port) noexcept;

    /// @brief Closes the current connection, if connected.
    void disconnect() noexcept;

    /// @brief Checks if this socket is connected.
    ///
    /// @return True if the socket is connected, false otherwise.
    bool isConnected() const noexcept;

private:
    /// @brief The wrapped socket.
    SocketType _socket;
};

} // namespace Terrahertz

#endif // !_WIN32
#endif // !THZ_COMMON_NETWORK_SOCKETWIN32_HPP
