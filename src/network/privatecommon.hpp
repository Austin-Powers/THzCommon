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
namespace Internal {

template <IPVersion TVersion>
inline auto constexpr AddressFamily = AF_INET;

template <>
inline auto constexpr AddressFamily<IPVersion::V6> = AF_INET6;

template <IPVersion TVersion>
using SockAddr = std::conditional_t<TVersion == IPVersion::V6, sockaddr_in6, sockaddr_in>;

template <Protocol TProtocol>
inline auto constexpr SocketType = SOCK_DGRAM;

template <>
inline auto constexpr SocketType<Protocol::TCP> = SOCK_STREAM;

template <Protocol TProtocol>
inline auto constexpr ProtocolType = IPPROTO_UDP;

template <>
inline auto constexpr ProtocolType<Protocol::TCP> = IPPROTO_TCP;

inline in_addr convertIPAddress(IPV4Address const &address) noexcept
{
    std::uint32_t const full{(static_cast<std::uint32_t>(address[0U]) << 24U) |
                             (static_cast<std::uint32_t>(address[1U]) << 16U) |
                             (static_cast<std::uint32_t>(address[2U]) << 8U) | static_cast<std::uint32_t>(address[3U])};

    in_addr addr{};
    addr.s_addr = htonl(full);
    return addr;
}

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

inline in6_addr convertIPAddress(IPV6Address const &address) noexcept
{
    in6_addr addr{};
    addr.W[0] = htons(address[0U]);
    addr.W[1] = htons(address[1U]);
    addr.W[2] = htons(address[2U]);
    addr.W[3] = htons(address[3U]);
    addr.W[4] = htons(address[4U]);
    addr.W[5] = htons(address[5U]);
    addr.W[6] = htons(address[6U]);
    addr.W[7] = htons(address[7U]);
    return addr;
}

inline IPV6Address convertIPAddress(in6_addr const &address) noexcept
{
    return {
        ntohs(address.W[0]),
        ntohs(address.W[1]),
        ntohs(address.W[2]),
        ntohs(address.W[3]),
        ntohs(address.W[4]),
        ntohs(address.W[5]),
        ntohs(address.W[6]),
        ntohs(address.W[7]),
    };
}

#undef W

} // namespace Internal
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_PRIVATECOMMON_HPP
