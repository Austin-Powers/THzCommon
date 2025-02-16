#include "THzCommon/network/udpsocket.hpp"
#include "THzCommon/network/address.hpp"

#include <gtest/gtest.h>
#include <random>

namespace Terrahertz::UnitTests {

struct NetworkUDPSocket : public testing::Test
{
	using UDPSocketV4 = UDPSocket<IPVersion::V4>;
	
	/// @brief Returns a localhost IPv4 address with an random port between 2000 and 4000, if possible.
	std::optional<Address<IPVersion::V4>> getLocalAddress() noexcept
	{
		auto const ipAddresses = resolveIPAddresses("localhost");
		auto const ipAddress = getFirstIPV4From(ipAddresses);
		if (ipAddress)
		{
			std::uniform_int_distribution<> distrib{2000, 4000};
			return Address<IPVersion::V4>{*ipAddress, static_cast<std::uint16_t>(distrib(randomEngine))};
		}
		return {};
	}
	
	std::mt19937 randomEngine{1337};
};

TEST_F(NetworkUDPSocket, DefaultConstruction)
{
	UDPSocketV4 sut{};
	EXPECT_NE(sut.handle(), 0);
	EXPECT_FALSE(sut.getReuseAddr().value());
	EXPECT_TRUE(sut.good());
}

TEST_F(NetworkUDPSocket, Bind)
{
	auto const address = getLocalAddress();
	EXPECT_TRUE(address);
	
	UDPSocketV4 sut{};
	EXPECT_TRUE(sut.bind(*address));
	EXPECT_TRUE(sut.good());
}

TEST_F(NetworkUDPSocket, ReuseAddr)
{
	UDPSocketV4 sut{};
	EXPECT_FALSE(sut.getReuseAddr().value());
	EXPECT_TRUE(sut.setReuseAddr(true));
	EXPECT_TRUE(sut.getReuseAddr().value());
}

TEST_F(NetworkUDPSocket, close)
{
	UDPSocketV4 sut{};
	sut.close();
	EXPECT_FALSE(sut.good());
}

}
