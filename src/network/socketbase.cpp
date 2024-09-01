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
{}

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol> &SocketBase<TVersion, TProtocol>::operator=(SocketBase<TVersion, TProtocol> &&) noexcept
{
    return *this;
}

template <IPVersion TVersion, Protocol TProtocol>
SocketBase<TVersion, TProtocol>::~SocketBase() noexcept
{}

template <IPVersion TVersion, Protocol TProtocol>
void SocketBase<TVersion, TProtocol>::bind(Address<TVersion> const &to) noexcept
{}

template <IPVersion TVersion, Protocol TProtocol>
void SocketBase<TVersion, TProtocol>::close() noexcept
{}

template <IPVersion TVersion, Protocol TProtocol>
bool SocketBase<TVersion, TProtocol>::good() noexcept
{
    return false;
}

template class SocketBase<IPVersion::V4, Protocol::UDP>;
template class SocketBase<IPVersion::V4, Protocol::TCP>;
template class SocketBase<IPVersion::V6, Protocol::UDP>;
template class SocketBase<IPVersion::V6, Protocol::TCP>;

} // namespace Internal
} // namespace Terrahertz
