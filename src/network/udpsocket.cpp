#include "THzCommon/network/udpsocket.hpp"

#include "privatecommon.hpp"

namespace Terrahertz {

using SockTraits = Internal::SocketTraits;

template <IPVersion TVersion>
Result<std::span<std::byte>> UDPSocket<TVersion>::receiveFrom(Address<TVersion>   *address,
                                                              std::span<std::byte> buffer) noexcept
{
    Internal::SockAddr<TVersion> addr{};

    auto addrLength = Internal::SockAddrLength<TVersion>;

    auto const result = ::recvfrom(this->_handle,
                                   reinterpret_cast<SockTraits::RecvBufferType>(buffer.data()),
                                   static_cast<SockTraits::BufferLength>(buffer.size_bytes()),
                                   0,
                                   reinterpret_cast<sockaddr *>(&addr),
                                   &addrLength);

    if (address != nullptr)
    {
        *address = Internal::convertSocketAddress(addr);
    }
    if (result == -1)
    {
        return Result<std::span<std::byte>>::error(errno);
    }
    return buffer.first(result);
}

template <IPVersion TVersion>
Result<std::size_t> UDPSocket<TVersion>::sendTo(Address<TVersion> const   &address,
                                                std::span<std::byte const> buffer) noexcept
{
    auto const addr   = Internal::convertSocketAddress(address);
    auto const result = ::sendto(this->_handle,
                                 reinterpret_cast<SockTraits::SendBufferType>(buffer.data()),
                                 static_cast<SockTraits::BufferLength>(buffer.size_bytes()),
                                 0,
                                 reinterpret_cast<sockaddr const *>(&addr),
                                 Internal::SockAddrLength<TVersion>);
    return static_cast<std::size_t>(result);
}

template class UDPSocket<IPVersion::V4>;
template class UDPSocket<IPVersion::V6>;

} // namespace Terrahertz
