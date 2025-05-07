#include "THzCommon/network/tcpconnection.hpp"

namespace Terrahertz {

/// @brief Checks if the given address is allowed for the given role.
///
/// @tparam TVersion The version of the internet protocol.
/// @param address The address to check.
/// @param server True if the connection assumes the role of a server, false for client.
/// @return True if the address can be used, false otherwise.
template <IPVersion TVersion>
static bool addressAllowed(Address<TVersion> const &address, bool const server) noexcept
{
    bool result = true;
    if (server)
    {
        bool allZeros = true;
        for (auto const i : address.ipAddress)
        {
            if (i != 0U)
            {
                allZeros = false;
            }
        }
        if (allZeros)
        {
            result = false;
        }
    }
    if (address.port < 1024U)
    {
        // first 1024 ports are reserved for the system.
        result = false;
    }
    return result;
}

template <IPVersion TVersion>
TCPConnection<TVersion>::TCPConnection(Address<TVersion> const &address, bool const server) noexcept
    : _server{server}, _address{address}
{
    if (!addressAllowed(_address, _server))
    {
        if (_server)
        {
            auto success = _serverSocket.bind(_address);
        }
    }
}


template <IPVersion TVersion>
bool TCPConnection<TVersion>::establish() noexcept
{
    return false;
}

template <IPVersion TVersion>
Result<size_t> TCPConnection<TVersion>::send(std::span<std::uint8_t const> const buffer) noexcept
{
    if (!addressAllowed(_address, _server))
    {
        return Result<size_t>::error(ENXIO);
    }
    size_t result{};
    return result;
}

template <IPVersion TVersion>
Result<std::span<std::uint8_t>> TCPConnection<TVersion>::receive(std::span<std::uint8_t> buffer) noexcept
{
    if (!addressAllowed(_address, _server))
    {
        return Result<std::span<std::uint8_t>>::error(ENXIO);
    }
    return std::span<std::uint8_t>{};
}

template class TCPConnection<IPVersion::V4>;
template class TCPConnection<IPVersion::V6>;

} // namespace Terrahertz
