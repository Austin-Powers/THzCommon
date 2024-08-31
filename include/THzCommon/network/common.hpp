#ifndef THZ_COMMON_NETWORK_COMMON_HPP
#define THZ_COMMON_NETWORK_COMMON_HPP

#include <array>
#include <cstdint>

namespace Terrahertz {

/// @brief Enumerates the version of the IP protocol.
enum class IPVersion
{
    V4 = 4U,
    V6 = 6U
};

namespace Detail {

template <IPVersion TIPV>
struct IPAddress;

template <>
struct IPAddress<IPVersion::V4> final
{
    using type = std::array<std::uint8_t, 4U>;
};
using IPV4Address = IPAddress<IPVersion::V4>::type;

template <>
struct IPAddress<IPVersion::V6> final
{
    using type = std::array<std::uint16_t, 8U>;
};
using IPV6Address = IPAddress<IPVersion::V6>::type;

#ifdef _WIN32
using SocketType = std::uint64_t;
#else
using SocketType = int;
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

} // namespace Detail
} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_COMMON_HPP
