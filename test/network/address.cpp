#include "THzCommon/network/address.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

TEST(NetworkAddress, ResolvingLocalhost)
{
    auto const addresses = resolveIPAddresses("localhost");
    EXPECT_TRUE(addresses);
    EXPECT_GE(addresses->size(), 1U); // ::1 and/or 127.0.0.1
    for (auto const &address : *addresses)
    {
        auto const v4 = std::get_if<Internal::IPV4Address>(&address);
        if (v4 != nullptr)
        {
            EXPECT_EQ(v4->data()[0], 127U);
            EXPECT_EQ(v4->data()[1], 0U);
            EXPECT_EQ(v4->data()[2], 0U);
            EXPECT_EQ(v4->data()[3], 1U);
        }
        auto const v6 = std::get_if<Internal::IPV6Address>(&address);
        if (v6 != nullptr)
        {
            EXPECT_EQ(v6->data()[0], 0U);
            EXPECT_EQ(v6->data()[1], 0U);
            EXPECT_EQ(v6->data()[2], 0U);
            EXPECT_EQ(v6->data()[3], 0U);
            EXPECT_EQ(v6->data()[4], 0U);
            EXPECT_EQ(v6->data()[5], 0U);
            EXPECT_EQ(v6->data()[6], 0U);
            EXPECT_EQ(v6->data()[7], 1U);
        }
    }
}

TEST(NetworkAddress, GetFirstIPV_AddressReturnsNothingIfAddressesIsEmpty)
{
    std::optional<IPV_Addresses> addresses{};
    auto const result4 = getFirstIPV4From(addresses);
    auto const result6 = getFirstIPV6From(addresses);
    
    EXPECT_FALSE(result4);
    EXPECT_FALSE(result6);
}

TEST(NetworkAddress, GetFirstIPV4ReturnsFirstIPV4Address)
{
    Internal::IPV6Address ipv6{12, 12, 12, 12, 12, 12, 12, 12};
    Internal::IPV4Address ipv4A{123, 0, 1, 111};
    Internal::IPV4Address ipv4B{234, 189, 74, 222};
    IPV_Addresses addresses{};
    addresses.push_back(ipv6);
    addresses.push_back(ipv4A);
    addresses.push_back(ipv4B);
    
    auto const result = getFirstIPV4From(addresses);
    
    EXPECT_TRUE(result);
    for (auto i = 0U; i < 4U; ++i)
    {
        EXPECT_EQ(result->data()[i], ipv4A[i]);
    }
}

TEST(NetworkAddress, GetFirstIPV6ReturnsFirstIPV6Address)
{
    Internal::IPV6Address ipv6A{123, 122, 102, 12, 142, 162, 212, 222};
    Internal::IPV6Address ipv6B{12, 12, 12, 12, 12, 12, 12, 12};
    Internal::IPV4Address ipv4{123, 0, 1, 111};
    IPV_Addresses addresses{};
    addresses.push_back(ipv4);
    addresses.push_back(ipv6A);
    addresses.push_back(ipv6B);
    
    auto const result = getFirstIPV6From(addresses);
    
    EXPECT_TRUE(result);
    for (auto i = 0U; i < 8U; ++i)
    {
        EXPECT_EQ(result->data()[i], ipv6A[i]);
    }
}

} // namespace Terrahertz::UnitTests
