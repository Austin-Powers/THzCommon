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
	UDPSocketV4 socket{};
	EXPECT_NE(socket.handle(), 0);
	EXPECT_TRUE(socket.getReuseAddr().isError());
}

TEST_F(NetworkUDPSocket, Bind)
{
	
	UDPSocketV4 socket{};
	
}

}
