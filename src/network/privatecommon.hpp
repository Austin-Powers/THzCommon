#ifndef THZ_COMMON_NETWORK_PRIVATECOMMON_HPP
#define THZ_COMMON_NETWORK_PRIVATECOMMON_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/common.hpp"

#include <cstddef>
#include <limits>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#else

#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif

namespace Terrahertz {
namespace Internal {

#ifdef _WIN32
struct SocketTraits final
{
    using SockLengthType = int;
    using SendBufferType = char const *;
    using RecvBufferType = char *;
    using BufferLength   = int;

    static SocketHandleType inline InvalidValue = std::numeric_limits<SocketHandleType>::max();
};
#else
struct SocketTraits final
{
    using SockLengthType = std::uint32_t;
    using SendBufferType = void const *;
    using RecvBufferType = void *;
    using BufferLength   = std::size_t;

    static SocketHandleType inline InvalidValue = -1;
};
#endif

template <IPVersion TVersion>
inline auto constexpr AddressFamily = AF_INET;

template <>
inline auto constexpr AddressFamily<IPVersion::V6> = AF_INET6;

template <IPVersion TVersion>
using SockAddr = std::conditional_t<TVersion == IPVersion::V6, sockaddr_in6, sockaddr_in>;

template <IPVersion TVersion>
inline auto SockAddrLength = static_cast<SocketTraits::SockLengthType>(sizeof(SockAddr<TVersion>));

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

inline IPV4Address convertIPAddress(in_addr const &address) noexcept
{
    std::uint32_t const full{ntohl(address.s_addr)};

    return {
        static_cast<std::uint8_t>(full >> 24U),
        static_cast<std::uint8_t>(full >> 16U),
        static_cast<std::uint8_t>(full >> 8U),
        static_cast<std::uint8_t>(full),
    };
}

#ifdef _WIN32
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

inline sockaddr_in convertSocketAddress(Address<IPVersion::V4> const &address) noexcept
{
    sockaddr_in result{};
    result.sin_family = AF_INET;
    result.sin_addr   = convertIPAddress(address.ipAddress);
    result.sin_port   = htons(address.port);
    return result;
}

inline sockaddr_in6 convertSocketAddress(Address<IPVersion::V6> const &address) noexcept
{
    sockaddr_in6 result{};
    result.sin6_family = AF_INET6;
    result.sin6_addr   = convertIPAddress(address.ipAddress);
    result.sin6_port   = htons(address.port);
    return result;
}

inline Address<IPVersion::V4> convertSocketAddress(sockaddr_in const &address) noexcept
{
    return {convertIPAddress(address.sin_addr), ntohs(address.sin_port)};
}

inline Address<IPVersion::V6> convertSocketAddress(sockaddr_in6 const &address) noexcept
{
    return {convertIPAddress(address.sin6_addr), ntohs(address.sin6_port)};
}

/// @brief Performs a poll operation to see if there is data to read on the socket.
///
/// @param socket The socket to poll.
/// @return True if data can be read from the socket without blocking, false otherwise.
inline bool pollRead(SocketHandleType socket) noexcept
{
    std::array<pollfd, 1U> fds{};
    fds[0].fd     = socket;
    fds[0].events = POLLIN;
#ifdef _WIN32
    auto const result = WSAPoll(fds.data(), 1U, 0);
#else
    auto const result = poll(fds.data(), 1U, 0);
#endif
    return (result > 0) && ((fds[0].revents & POLLIN) != 0);
}

/// @brief Performs a poll operation to see if data can be written to the socket. without blocking.
///
/// @param socket The  socket to poll.
/// @return True if data can be written to the socket without blocking, false otherwise.
inline bool pollWrite(SocketHandleType socket) noexcept
{
    std::array<pollfd, 1U> fds{};
    fds[0].fd     = socket;
    fds[0].events = POLLOUT;
#ifdef _WIN32
    auto const result = WSAPoll(fds.data(), 1U, 0);
#else
    auto const result = poll(fds.data(), 1U, 0);
#endif
    return (result > 0) && ((fds[0].revents & POLLOUT) != 0);
}

} // namespace Internal
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_PRIVATECOMMON_HPP
