#ifndef THZ_COMMON_NETWORK_NATIVE_HPP
#define THZ_COMMON_NETWORK_NATIVE_HPP

#include <cstdint>

namespace Terrahertz {

#ifdef _WIN32
struct socket_traits
{};
#else
struct socket_traits
{};
#endif // !_WIN32

/// @brief Encapsulates the native socket API.
class SocketApi final
{
public:
    SocketApi(SocketApi const &) = delete;

    SocketApi &operator=(SocketApi const &) = delete;

    /// @brief Finalizes the SocketApi.
    ~SocketApi() noexcept;

    /// @brief Returns the global instance of the API.
    ///
    /// @return The global intance of the API.
    static SocketApi const &instance();

private:
    /// @brief Default initializes the socket API.
    SocketApi();
};

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_NATIVE_HPP
