#include "THzCommon/network/selector.hpp"

#include "privatecommon.hpp"

#include <algorithm>

namespace Terrahertz {

struct Selector::DataImpl
{
    int    nfds{};
    fd_set readfd{};
    fd_set writefd{};
    fd_set exceptfd{};
};

Selector::Selector(Internal::SocketApi const &) noexcept { _data.init(); }

Selector::~Selector() noexcept = default;

void Selector::add(Internal::SocketHandleType const handle) noexcept
{
    FD_SET(handle, &_data->readfd);
    FD_SET(handle, &_data->writefd);
    FD_SET(handle, &_data->exceptfd);
    _data->nfds = std::max(_data->nfds, static_cast<int>(handle));
}

Result<std::size_t> Selector::select(std::chrono::microseconds timeout) noexcept
{
    timeval tv{};
    tv.tv_sec  = static_cast<decltype(tv.tv_sec)>(timeout.count() / 1'000'000);
    tv.tv_usec = static_cast<decltype(tv.tv_usec)>(timeout.count() % 1'000'000);

    auto const result = ::select(_data->nfds, &_data->readfd, &_data->writefd, &_data->exceptfd, &tv);
    if (result == -1)
    {
        return Result<std::size_t>::error();
    }
    return static_cast<std::size_t>(result);
}

void Selector::clear() noexcept
{
    _data->nfds = 0;
    FD_ZERO(&_data->readfd);
    FD_ZERO(&_data->writefd);
    FD_ZERO(&_data->exceptfd);
}

bool Selector::isRead(Internal::SocketHandleType const handle) const noexcept
{
    return FD_ISSET(handle, &_data->readfd);
}

bool Selector::isWrite(Internal::SocketHandleType const handle) const noexcept
{
    return FD_ISSET(handle, &_data->writefd);
}

bool Selector::isExcept(Internal::SocketHandleType const handle) const noexcept
{
    return FD_ISSET(handle, &_data->exceptfd);
}

} // namespace Terrahertz
