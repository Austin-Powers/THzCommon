#include "THzCommon/network/tcpsocket.hpp"
#include "THzCommon/network/address.hpp"

#include <gtest/gtest.h>
#include <random>

namespace Terrahertz::UnitTests {

struct NetworkTCPSocket : public testing::Test
{
	using TCPSocketV4 = TCPSocket<IPVersion::V4>;
	
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

TEST_F(NetworkTCPSocket, DefaultConstruction)
{
	TCPSocketV4 sut{};
	EXPECT_NE(sut.handle(), 0);
	EXPECT_FALSE(sut.getReuseAddr().value());
	EXPECT_TRUE(sut.good());
}

TEST_F(NetworkTCPSocket, ReuseAddr)
{
	TCPSocketV4 sut{};
	EXPECT_FALSE(sut.getReuseAddr().value());
	EXPECT_TRUE(sut.setReuseAddr(true));
	EXPECT_TRUE(sut.getReuseAddr().value());
}

TEST_F(NetworkTCPSocket, Close)
{
	TCPSocketV4 sut{};
	sut.close();
	EXPECT_FALSE(sut.good());
}

TEST_F(NetworkTCPSocket, Bind)
{
	auto const address = getLocalAddress();
	EXPECT_TRUE(address);
	
	TCPSocketV4 sut{};
	EXPECT_TRUE(sut.bind(*address));
	EXPECT_TRUE(sut.good());
}

}
