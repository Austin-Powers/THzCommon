#ifndef THZ_COMMON_NETWORK_PRIVATECOMMON_HPP
#define THZ_COMMON_NETWORK_PRIVATECOMMON_HPP

#include "THzCommon/network/common.hpp"

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#else

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#endif

namespace Terrahertz {
namespace Detail {

inline IPV4Address convertIPAddress(in_addr const &addr) noexcept
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
#define W s6_addr16
#endif

inline IPV6Address convertIPAddress(in6_addr const &addr) noexcept
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

} // namespace Detail
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_PRIVATECOMMON_HPP
