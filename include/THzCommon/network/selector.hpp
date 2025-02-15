#ifndef THZ_COMMON_NETWORK_HPP
#define THZ_COMMON_NETWORK_HPP

#include "THzCommon/network/common.hpp"
#include "THzCommon/network/socketbase.hpp"
#include "THzCommon/utility/result.hpp"
#include "THzCommon/utility/staticPImpl.hpp"

#include <chrono>

namespace Terrahertz {

/// @brief Encapsulates the select function, making it easier to handle.
class Selector
{
public:
    /// @brief Initializes a new selector instance.
    Selector(Internal::SocketApi const & = Internal::SocketApi::instance()) noexcept;

    /// @brief Deinitalizes the selector instance.
    ~Selector() noexcept;

    /// @brief Adds a socket to this selector.
    ///
    /// @tparam TVersion The IP version of the socket.
    /// @tparam TProtocol The protocol used by the socket.
    /// @param socket The socket to add.
    template <IPVersion TVersion, Protocol TProtocol>
    void add(Internal::SocketBase<TVersion, TProtocol> const &socket) noexcept
    {
        add(socket.handle());
    }

    /// @brief Performs the select operation.
    ///
    /// @param timeout The timeout of the operation.
    /// @return The number of file descriptors contained in the result set.
    Result<std::size_t> select(std::chrono::microseconds timeout = std::chrono::microseconds{0}) noexcept;

    /// @brief Removes all sockets from this selector.
    void clear() noexcept;

    /// @brief Checks if data can be read from the given socket.
    ///
    /// @tparam TVersion The IP version of the socket.
    /// @tparam TProtocol The protocol used by the socket.
    /// @param socket The socket to check.
    /// @return True if there is data to read on the socket, false otherwise.
    template <IPVersion TVersion, Protocol TProtocol>
    [[nodiscard]] bool isRead(Internal::SocketBase<TVersion, TProtocol> const &socket) const noexcept
    {
        return isRead(socket.handle());
    }

    /// @brief Checks if data can be written to the given socket.
    ///
    /// @tparam TVersion The IP version of the socket.
    /// @tparam TProtocol The protocol used by the socket.
    /// @param socket The socket to check.
    /// @return True if data can be written to the socket, false otherwise.
    template <IPVersion TVersion, Protocol TProtocol>
    [[nodiscard]] bool isWrite(Internal::SocketBase<TVersion, TProtocol> const &socket) const noexcept
    {
        return isWrite(socket.handle());
    }

    /// @brief Checks if there are exceptional conditions on the given socket.
    ///
    /// @tparam TVersion The IP version of the socket.
    /// @tparam TProtocol The protocol used by the socket.
    /// @param socket The socket to check.
    /// @return True if there are exceptional conditions on the socket, false otherwise.
    template <IPVersion TVersion, Protocol TProtocol>
    [[nodiscard]] bool isExcept(Internal::SocketBase<TVersion, TProtocol> const &socket) const noexcept
    {
        return isExcept(socket.handle());
    }

private:
    /// @brief Adds a socket handle to the selector.
    ///
    /// @param handle The handle to add.
    void add(Internal::SocketHandleType const handle) noexcept;

    /// @brief Checks if data can be read from the given socket handle.
    ///
    /// @param handle The handle of the socket to check.
    /// @return True if data can be read, false otherwise.
    bool isRead(Internal::SocketHandleType const handle) const noexcept;

    /// @brief Checks if data can be written to the given socket handle.
    ///
    /// @param handle The handle of the socket to check.
    /// @return True if data can be written, false otherwise.
    bool isWrite(Internal::SocketHandleType const handle) const noexcept;

    /// @brief Checks if there exceptional conditions on the given socket handle.
    ///
    /// @param handle The handle of the socket to check.
    /// @return True if there are exceptional conditions on the given socket handle, false otherwise.
    bool isExcept(Internal::SocketHandleType const handle) const noexcept;

    /// @brief Declaration of the data implementation.
    struct DataImpl;

    /// @brief Pointer to the data implementation.
    StaticPImpl<DataImpl, 2048U> _data{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_HPP
