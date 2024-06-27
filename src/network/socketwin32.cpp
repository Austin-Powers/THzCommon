#ifdef _WIN32

#include "THzCommon/network/socketwin32.hpp"

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <system_error>

namespace Terrahertz {

SocketWin32::SocketWin32() noexcept(false)
{
    WSADATA    wsaData;
    auto const result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        throw std::system_error({result, std::iostream_category()}, "WSAStartup failed.");
    }
}

SocketWin32::~SocketWin32() noexcept { WSACleanup(); }

} // namespace Terrahertz

#endif // !_WIN32
