#include "THzCommon/network/socketbase.hpp"

#include "privatecommon.hpp"

#include <utility>

namespace Terrahertz {
namespace Internal {

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol>::SocketBase(Internal::SocketApi const &) noexcept
    : _handle{socket(AddressFamily<TVersion>, SocketType<TProtocol>, ProtocolType<TProtocol>)}
{}

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol>::SocketBase(SocketBase<TVersion, TProtocol> &&other) noexcept : _handle{other._handle}
{
    other._handle = SocketTraits::InvalidValue;
}

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol> &
SocketBase<TVersion, TProtocol>::operator=(SocketBase<TVersion, TProtocol> &&other) noexcept
{
    std::swap(_handle, other._handle);
    return *this;
}

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol>::~SocketBase() noexcept
{
    close();
}

template <IPVersion TVersion, Protocol TProtocol>
bool SocketBase<TVersion, TProtocol>::bind(Address<TVersion> const &to) noexcept
{
    auto const address = convertToSockaddrIn(to);
    auto const result =
        ::bind(_handle, reinterpret_cast<sockaddr const *>(&address), SocketTraits::SockLengthType{sizeof(address)});
    return result != -1;
}

template <IPVersion TVersion, Protocol TProtocol>
void SocketBase<TVersion, TProtocol>::close() noexcept
{
    if (_handle != SocketTraits::InvalidValue)
    {
#if defined(_WIN32)
        ::closesocket(_handle);
#else
        ::close(_handle);
#endif
    }
    _handle = SocketTraits::InvalidValue;
}

template <IPVersion TVersion, Protocol TProtocol>
bool SocketBase<TVersion, TProtocol>::good() noexcept
{
    return _handle != SocketTraits::InvalidValue;
}

template class SocketBase<IPVersion::V4, Protocol::UDP>;
template class SocketBase<IPVersion::V4, Protocol::TCP>;
template class SocketBase<IPVersion::V6, Protocol::UDP>;
template class SocketBase<IPVersion::V6, Protocol::TCP>;

} // namespace Internal
} // namespace Terrahertz
