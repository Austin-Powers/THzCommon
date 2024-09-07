#include "THzCommon/network/tcpsocket.hpp"

#include "privatecommon.hpp"

namespace Terrahertz {

using SockTraits = Internal::SocketTraits;

template <IPVersion TVersion>
bool TCPSocket<TVersion>::listen(std::uint32_t const backlog) noexcept
{
    return false;
}

template <IPVersion TVersion>
TCPSocket<TVersion> TCPSocket<TVersion>::accept(Address<TVersion> *address) noexcept
{
    return TCPSocket();
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::connect(Address<TVersion> const &address) noexcept
{
    return false;
}

template <IPVersion TVersion>
bool TCPSocket<TVersion>::shutdown(int what) noexcept
{
    return false;
}

template <IPVersion TVersion>
Result<std::span<std::byte>> TCPSocket<TVersion>::receive(std::span<std::byte> buffer) noexcept
{
    return {};
}

template <IPVersion TVersion>
Result<std::size_t> TCPSocket<TVersion>::send(std::span<std::byte const> buffer) noexcept
{
    return 0;
}

template class TCPSocket<IPVersion::V4>;
template class TCPSocket<IPVersion::V6>;

} // namespace Terrahertz
