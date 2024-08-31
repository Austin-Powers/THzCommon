#include "THzCommon/network/socketbase.hpp"

namespace Terrahertz {
namespace Internal {

template <IPVersion TVersion>
SocketBase<TVersion>::SocketBase() noexcept
{}

template <IPVersion TVersion>
SocketBase<TVersion>::SocketBase(SocketBase<TVersion> &&) noexcept
{}

template <IPVersion TVersion>
SocketBase<TVersion> &SocketBase<TVersion>::operator=(SocketBase<TVersion> &&) noexcept
{
    return *this;
}

template <IPVersion TVersion>
SocketBase<TVersion>::~SocketBase() noexcept
{}

template <IPVersion TVersion>
void SocketBase<TVersion>::bind(Address<TVersion> const &to) noexcept
{}

template <IPVersion TVersion>
void SocketBase<TVersion>::close() noexcept
{}

template <IPVersion TVersion>
bool SocketBase<TVersion>::good() noexcept
{
    return false;
}

template class SocketBase<IPVersion::V4>;
template class SocketBase<IPVersion::V6>;

} // namespace Internal
} // namespace Terrahertz
