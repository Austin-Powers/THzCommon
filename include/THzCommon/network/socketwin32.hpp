#ifndef THZ_COMMON_NETWORK_SOCKETWIN32_HPP
#define THZ_COMMON_NETWORK_SOCKETWIN32_HPP

#ifdef _WIN32

namespace Terrahertz {

/// @brief A network socket for Windows using TCP.
class SocketWin32
{
public:
    /// @brief Initializes a new Socket.
    ///
    /// @exception system_error In case WSAStartup fails.
    SocketWin32() noexcept(false);

    /// @brief Finalizes this socket.
    ~SocketWin32() noexcept;
};

} // namespace Terrahertz

#endif // !_WIN32
#endif // !THZ_COMMON_NETWORK_SOCKETWIN32_HPP
