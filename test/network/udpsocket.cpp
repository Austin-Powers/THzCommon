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

TEST_F(NetworkUDPSocket, ReuseAddr)
{
	UDPSocketV4 sut{};
	EXPECT_FALSE(sut.getReuseAddr().value());
	EXPECT_TRUE(sut.setReuseAddr(true));
	EXPECT_TRUE(sut.getReuseAddr().value());
}

TEST_F(NetworkUDPSocket, Close)
{
	UDPSocketV4 sut{};
	sut.close();
	EXPECT_FALSE(sut.good());
}

TEST_F(NetworkUDPSocket, Bind)
{
	auto const address = getLocalAddress();
	EXPECT_TRUE(address);
	
	UDPSocketV4 sut{};
	EXPECT_TRUE(sut.bind(*address));
	EXPECT_TRUE(sut.good());
}

TEST_F(NetworkUDPSocket, SelectReadYieldsNothingOnNewSocket)
{
	auto const address = getLocalAddress();
	EXPECT_TRUE(address);
	
	UDPSocketV4 sut{};
	EXPECT_TRUE(sut.bind(*address));
	
	Selector selector{};
	selector.add(sut);
	auto const result = selector.select();
	EXPECT_EQ(result.value(), 0U);
}

TEST_F(NetworkUDPSocket, SimpleDataTransfer)
{
	auto const senderAddress = getLocalAddress();
	auto const receiverAddress = getLocalAddress();
	EXPECT_TRUE(senderAddress);
	EXPECT_TRUE(receiverAddress);
	UDPSocketV4 sender{};
	UDPSocketV4 receiver{};
	EXPECT_TRUE(sender.bind(*senderAddress));
	EXPECT_TRUE(receiver.bind(*receiverAddress));
	
	std::array<std::uint8_t, 32U> sendBuffer{};
	std::array<std::uint8_t, 64U> receiveBuffer{};
	
}

}
