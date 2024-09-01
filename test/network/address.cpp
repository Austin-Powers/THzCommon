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

} // namespace Terrahertz::UnitTests
