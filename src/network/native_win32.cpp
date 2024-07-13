#include "THzCommon/network/native.hpp"

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <Windows.h>
#include <stdexcept>

namespace Terrahertz {

SocketApi::SocketApi()
{
    WSADATA    data{};
    auto const result = WSAStartup(MAKEWORD(2, 2), &data);
    if (result != 0)
    {
        throw std::runtime_error("WSAStartup failed");
    }
}

SocketApi::~SocketApi() { WSACleanup(); }

SocketApi const &SocketApi::instance()
{
    static SocketApi api{};
    return api;
}

} // namespace Terrahertz
