#ifdef _WIN32

#include "THzCommon/network/socketwin32.hpp"

#include "THzCommon/logging/logging.hpp"

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <system_error>

namespace Terrahertz {

/// @brief Name provider for the socket project.
struct SocketProject
{
    static constexpr char const *name() noexcept { return "THzCommon.Network.Socket"; }
};

SocketWin32::SocketWin32() noexcept(false) : _socket{INVALID_SOCKET}
{
    WSADATA    wsaData;
    auto const result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        logMessage<LogLevel::Error, SocketProject>("Failed to startup WSA");
        throw std::system_error({result, std::iostream_category()}, "WSAStartup failed.");
    }
}

SocketWin32::~SocketWin32() noexcept
{
    disconnect();
    WSACleanup();
}

bool SocketWin32::connectTo(std::string_view const address, std::uint16_t const port) noexcept
{
    if (_socket != INVALID_SOCKET)
    {
        logMessage<LogLevel::Warning, SocketProject>("Trying to call connectTo on an already connected socket");
        return false;
    }
    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET)
    {
        logMessage<LogLevel::Error, SocketProject>("Creating socket failed");
        return false;
    }
    sockaddr_in socket_address{};
    socket_address.sin_family = AF_INET;
    // TODO fein dann nehmen wir halt inet_pton hier
    socket_address.sin_addr.s_addr = inet_addr(address.data());
    socket_address.sin_port        = htons(port);
    if (connect(_socket, reinterpret_cast<sockaddr *>(&socket_address), sizeof(socket_address)) == SOCKET_ERROR)
    {
        std::array<char, 64> text{};
        snprintf(text.data(), text.size(), "Connecting socket failed with error: %d", WSAGetLastError());
        logMessage<LogLevel::Error, SocketProject>(text.data());
        if (closesocket(_socket) == SOCKET_ERROR)
        {
            logMessage<LogLevel::Error, SocketProject>("Closing socket failed as well");
        }
        _socket = INVALID_SOCKET;
        return false;
    }
    return true;
}

void SocketWin32::disconnect() noexcept
{
    if (_socket != INVALID_SOCKET)
    {
        if (closesocket(_socket) == SOCKET_ERROR)
        {
            logMessage<LogLevel::Error, SocketProject>("Closing socket failed");
        }
        _socket = INVALID_SOCKET;
    }
}

bool SocketWin32::isConnected() const noexcept { return _socket != INVALID_SOCKET; }

} // namespace Terrahertz

#endif // !_WIN32
