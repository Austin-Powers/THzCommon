#include "THzCommon/network/udpsocket.hpp"

namespace Terrahertz {

template <IPVersion TVersion>
void UDPSocket<TVersion>::receiveFrom(Address<TVersion> const &address) noexcept
{}

template <IPVersion TVersion>
void UDPSocket<TVersion>::sendTo(Address<TVersion> const &address) noexcept
{}

template class UDPSocket<IPVersion::V4>;
template class UDPSocket<IPVersion::V6>;

} // namespace Terrahertz
