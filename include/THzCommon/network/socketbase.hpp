#ifndef THZ_COMMON_NETWORK_SOCKETBASE_HPP
#define THZ_COMMON_NETWORK_SOCKETBASE_HPP

#include "THzCommon/network/address.hpp"
#include "THzCommon/network/common.hpp"
#include "THzCommon/utility/result.hpp"

namespace Terrahertz {
namespace Internal {

/// @brief Base containing shared functionality of all sockets.
///
/// @tparam TVersion The version of the internet protocol.
/// @tparam TProtocol The protocol on top of the internet protocol (UDP/TCP).
template <IPVersion TVersion, Protocol TProtocol>
class SocketBase
{
public:
    /// @brief Initializes a new socket.
    SocketBase(SocketApi const & = SocketApi::instance()) noexcept;

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

    /// @brief Returns the native handle of the socket.
    ///
    /// @return The native handle of the socket.
    [[nodiscard]] SocketHandleType handle() const noexcept;

    /// @brief Binds the socket to the given IP address.
    ///
    /// @param to The address to bind the socket to.
    /// @return True if socket was bound, false otherwise.
    bool bind(Address<TVersion> const &to) noexcept;

    /// @brief Sets the reuse_addr option of this socket.
    ///
    /// @param reuse The value reuse_addr shall be set to.
    /// @return True if the operation was successfull, false otherwise.
    bool setReuseAddr(bool reuse) noexcept;

    /// @brief Returns the current state of the reuse_addr option of this socket.
    ///
    /// @return The current state of the reuse_addr option of this socket.
    Result<bool> getReuseAddr() noexcept;

    /// @brief Closes the socket.
    void close() noexcept;

    /// @brief Checks if the socket is usable.
    ///
    /// @return True if the socket can be used, false otherwise.
    bool good() noexcept;

protected:
    /// @brief Initializes a new SocketBase from an already existing handle.
    ///
    /// @param handle The handle to initialize the socket with.
    inline SocketBase(SocketHandleType handle) noexcept : _handle{handle} {}

    /// @brief The native handle of the managed socket.
    SocketHandleType _handle;
};

extern template class SocketBase<IPVersion::V4, Protocol::UDP>;
extern template class SocketBase<IPVersion::V4, Protocol::TCP>;
extern template class SocketBase<IPVersion::V6, Protocol::UDP>;
extern template class SocketBase<IPVersion::V6, Protocol::TCP>;

} // namespace Internal
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_SOCKETBASE_HPP
