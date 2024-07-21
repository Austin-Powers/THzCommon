#include "THzCommon/network/address.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#else

#include <netdb>
#include <sys/socket.h>
#include <sys/types.h>

#endif

namespace {

Terrahertz::Detail::IPV4Address convertIPAddress(in_addr const &addr) noexcept
{
    std::uint32_t const full{ntohl(addr.s_addr)};

    return {
        static_cast<std::uint8_t>(full >> 24U),
        static_cast<std::uint8_t>(full >> 16U),
        static_cast<std::uint8_t>(full >> 8U),
        static_cast<std::uint8_t>(full),
    };
}

#if defined(_WIN32)
#define W u.Word
#else
#define W __u6_addr.__u6_addr16
#endif

Terrahertz::Detail::IPV6Address convertIPAddress(in6_addr const &addr) noexcept
{
    return {
        ntohs(addr.W[0]),
        ntohs(addr.W[1]),
        ntohs(addr.W[2]),
        ntohs(addr.W[3]),
        ntohs(addr.W[4]),
        ntohs(addr.W[5]),
        ntohs(addr.W[6]),
        ntohs(addr.W[7]),
    };
}

#undef W

} // namespace

namespace Terrahertz {

std::optional<std::vector<Detail::IPV_Address>> resolveIPAddresses(std::string_view address,
                                                                   Native::SocketApi const &) noexcept
{
    addrinfo hints{};
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *infoPtr = nullptr;

    auto const error = getaddrinfo(address.data(), nullptr, &hints, &infoPtr);
    if (error != 0)
    {
        return {};
    }

    std::vector<Detail::IPV_Address> result{};
    for (auto currentInfo = infoPtr; currentInfo != nullptr; currentInfo = currentInfo->ai_next)
    {
        if (currentInfo->ai_family == AF_INET)
        {
            auto const address = reinterpret_cast<sockaddr_in const *>(currentInfo->ai_addr);
            result.emplace_back(::convertIPAddress(address->sin_addr));
        }
        else if (currentInfo->ai_family == AF_INET6)
        {
            auto const address = reinterpret_cast<sockaddr_in6 const *>(currentInfo->ai_addr);
            result.emplace_back(::convertIPAddress(address->sin6_addr));
        }
    }
    freeaddrinfo(infoPtr);
    return result;
}

} // namespace Terrahertz
