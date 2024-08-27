#ífndef THZ_COMMON_NETWORK_SOCKETBASE_HPP
#define THZ_COMMON_NETWORK_SOCKETBASE_HPP

#include "THzCommon/network/address.hpp"

namespace Terrahertz {
namespace Internal {

/// @brief Base containing shared functionality of all sockets.
class SocketBase
{
public:
    SocketBase() noexcept;

    SocketBase(SocketBase const &) = delete;

    SocketBase(SocketBase &&) noexecpt;

    SocketBase &operator=(SocketBase const &) = delete;

    SocketBase &operator=(SocketBase &&) noexecpt;

    ~SocketBase() noexcept;

    void bind(Address const &to) noexcept;

    void close() noexcept;

    bool good() noexcept;
};

} // namespace Internal
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_SOCKETBASE_HPP
