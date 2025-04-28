#include "THzCommon/network/tcpconnection.hpp"

namespace Terrahertz {

/// @brief Checks if the given address is allowed for the given role.
///
/// @tparam TVersion The version of the internet protocol.
/// @param role The role ofthe connection side.
/// @param address The address to check.
/// @return True if the address can be used, false otherwise.
template <IPVersion TVersion>
static bool addressAllowed(typename TCPConnection<TVersion>::Role const role, Address<TVersion> const &address) noexcept
{
    bool result = true;
    if (role != TCPConnection<TVersion>::Role::Server)
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
TCPConnection<TVersion>::TCPConnection(Role const role, Address<TVersion> const &address) noexcept
    : _role{role}, _address{address}
{
    if (!addressAllowed(_role, _address))
    {
        if (_role == Role::Server)
        {}
    }
    establish();
}

template <IPVersion TVersion>
Result<size_t> TCPConnection<TVersion>::send(std::span<std::uint8_t const> const buffer) noexcept
{
    if (!addressAllowed(_role, _address))
    {
        return Result<size_t>::error(ENXIO);
    }
    return 0ULL;
}

template <IPVersion TVersion>
Result<std::span<std::uint8_t>> TCPConnection<TVersion>::receive(std::span<std::uint8_t> buffer) noexcept
{
    if (!addressAllowed(_role, _address))
    {
        return Result<std::span<std::uint8_t>>::error(ENXIO);
    }
    return std::span<std::uint8_t>{};
}

template <IPVersion TVersion>
void TCPConnection<TVersion>::establish() noexcept
{}

template class TCPConnection<IPVersion::V4>;
template class TCPConnection<IPVersion::V6>;

} // namespace Terrahertz
