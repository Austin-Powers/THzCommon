#include "THzCommon/network/address.hpp"

#include "privatecommon.hpp"

namespace Terrahertz {

std::optional<IPV_Addresses> resolveIPAddresses(std::string_view address, Internal::SocketApi const &) noexcept
{
    addrinfo hints{};
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *infoPtr = nullptr;

    auto const error = getaddrinfo(address.data(), nullptr, &hints, &infoPtr);
    if (error != 0)
    {
        return {};
    }

    IPV_Addresses result{};
    for (auto currentInfo = infoPtr; currentInfo != nullptr; currentInfo = currentInfo->ai_next)
    {
        if (currentInfo->ai_family == AF_INET)
        {
            auto const address = reinterpret_cast<sockaddr_in const *>(currentInfo->ai_addr);
            result.emplace_back(Internal::convertIPAddress(address->sin_addr));
        }
        else if (currentInfo->ai_family == AF_INET6)
        {
            auto const address = reinterpret_cast<sockaddr_in6 const *>(currentInfo->ai_addr);
            result.emplace_back(Internal::convertIPAddress(address->sin6_addr));
        }
    }
    freeaddrinfo(infoPtr);
    return result;
}

} // namespace Terrahertz
