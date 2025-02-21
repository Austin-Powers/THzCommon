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
        auto const ipAddress   = getFirstIPV4From(ipAddresses);
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

TEST_F(NetworkUDPSocket, ReceiveIstNonblockingReturnsFalseOnNewSocket)
{
    auto const address = getLocalAddress();
    EXPECT_TRUE(address);

    UDPSocketV4 sut{};
    EXPECT_FALSE(sut.receiveIsNonblocking());
}

TEST_F(NetworkUDPSocket, SendIsNonblockingReturnsTrueOnNewSocket)
{
    auto const address = getLocalAddress();
    EXPECT_TRUE(address);

    UDPSocketV4 sut{};
    EXPECT_TRUE(sut.sendIsNonblocking());
}

TEST_F(NetworkUDPSocket, BasicDataTransfer)
{
    auto const senderAddress   = getLocalAddress();
    auto const receiverAddress = getLocalAddress();
    EXPECT_TRUE(senderAddress);
    EXPECT_TRUE(receiverAddress);
    UDPSocketV4 sender{};
    UDPSocketV4 receiver{};
    EXPECT_TRUE(sender.bind(*senderAddress));
    EXPECT_TRUE(receiver.bind(*receiverAddress));

    std::array<std::uint8_t, 16U> sendBuffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::array<std::uint8_t, 32U> receiveBuffer{};

    std::span<std::byte const> sendSpan{reinterpret_cast<std::byte *>(sendBuffer.data()), sendBuffer.size()};
    std::span<std::byte>       receiveSpan{reinterpret_cast<std::byte *>(receiveBuffer.data()), receiveBuffer.size()};

    auto const sendResult = sender.sendTo(*receiverAddress, sendSpan);
    EXPECT_FALSE(sendResult.isError());
    EXPECT_EQ(sendResult.value(), sendBuffer.size());

    ASSERT_TRUE(receiver.receiveIsNonblocking());
    auto const receiveResult = receiver.receiveFrom(nullptr, receiveSpan);
    EXPECT_FALSE(receiveResult.isError());
    EXPECT_EQ(receiveResult.value().size(), sendBuffer.size());
    for (auto i = 0U; i < sendBuffer.size(); ++i)
    {
        EXPECT_EQ(sendBuffer[i], receiveBuffer[i]);
    }
}

} // namespace Terrahertz::UnitTests
