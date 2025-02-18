#include "THzCommon/network/tcpsocket.hpp"

#include "privatecommon.hpp"

namespace Terrahertz {

using SockTraits = Internal::SocketTraits;

template <IPVersion TVersion>
bool TCPSocket<TVersion>::listen(std::uint32_t const backlog) noexcept
{
    return ::listen(this->_handle, static_cast<int>(backlog & std::numeric_limits<int>::max())) != -1;
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::acceptIsNonblocking() const noexcept
{
    return false;
}

template <IPVersion TVersion>
TCPSocket<TVersion> TCPSocket<TVersion>::accept(Address<TVersion> *address) noexcept
{
    Internal::SockAddr<TVersion> addr{};

    auto addrLength = Internal::SockAddrLength<TVersion>;

    auto const result = ::accept(this->_handle, reinterpret_cast<sockaddr *>(&addr), &addrLength);
    return TCPSocket(result);
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::connect(Address<TVersion> const &address) noexcept
{
    auto const addr = Internal::convertSocketAddress(address);
    auto const result =
        ::connect(this->_handle, reinterpret_cast<sockaddr const *>(&addr), Internal::SockAddrLength<TVersion>);
    return result != -1;
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::shutdown(int what) noexcept
{
#ifdef _WIN32
    auto const how = SD_BOTH;
#else
    auto const how = SHUT_RDWR;
#endif
    return ::shutdown(this->_handle, how) != -1;
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::receiveIsNonblocking() const noexcept
{
    return false;
}

template <IPVersion TVersion>
Result<std::span<std::byte>> TCPSocket<TVersion>::receive(std::span<std::byte> buffer) noexcept
{
    auto const result = ::recv(this->_handle,
                               reinterpret_cast<SockTraits::RecvBufferType>(buffer.data()),
                               static_cast<SockTraits::BufferLength>(buffer.size_bytes()),
                               0);
    if (result == -1)
    {
        return Result<std::span<std::byte>>::error();
    }
    return buffer.first(result);
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::sendIsNonblocking() const noexcept
{
    return false;
}

template <IPVersion TVersion>
Result<std::size_t> TCPSocket<TVersion>::send(std::span<std::byte const> buffer) noexcept
{
    auto const result = ::send(this->_handle,
                               reinterpret_cast<SockTraits::SendBufferType>(buffer.data()),
                               static_cast<SockTraits::BufferLength>(buffer.size_bytes()),
                               0);
    if (result == -1)
    {
        return Result<std::size_t>::error();
    }
    return static_cast<std::size_t>(result);
}

template class TCPSocket<IPVersion::V4>;
template class TCPSocket<IPVersion::V6>;

} // namespace Terrahertz
