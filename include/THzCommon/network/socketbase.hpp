#ifndef THZ_COMMON_NETWORK_SOCKETBASE_HPP
#define THZ_COMMON_NETWORK_SOCKETBASE_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/common.hpp"

namespace Terrahertz {
namespace Internal {

/// @brief Base containing shared functionality of all sockets.
///
/// @tparam TVersion The version of the internet protocol.
template <IPVersion TVersion>
class SocketBase
{
public:
    /// @brief Initializes a new socket.
    SocketBase() noexcept;

    /// @brief No copy construction allowed.
    SocketBase(SocketBase const &) = delete;

    /// @brief Initializes a socket by moving from another one, invalidating it in the process.
    ///
    /// @param other The socket to move from.
    SocketBase(SocketBase &&other) noexcept;

    /// @brief No copy assignment allowed.
    SocketBase &operator=(SocketBase const &) = delete;

    /// @brief Assigns the connection of another socket to this one by swapping the connections.
    ///
    /// @param other The other socket to swap connections with.
    /// @return This socket.
    SocketBase &operator=(SocketBase &&other) noexcept;

    /// @brief Deinitializes the socket, closing it.
    virtual ~SocketBase() noexcept;

    /// @brief Binds the socket to the given IPV6 address.
    ///
    /// @param to The address to bind the socket to.
    void bind(Address<TVersion> const &to) noexcept;

    /// @brief Closes the socket.
    void close() noexcept;

    /// @brief Checks if the socket is usable.
    ///
    /// @return True if the socket can be used, false otherwise.
    bool good() noexcept;

private:
    /// @brief The native handle of the managed socket.
    Detail::SocketType _handle{};
};

extern template class SocketBase<IPVersion::V4>;
extern template class SocketBase<IPVersion::V6>;

} // namespace Internal
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_SOCKETBASE_HPP
