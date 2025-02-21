#include "THzCommon/network/tcpsocket.hpp"

#include "THzCommon/network/address.hpp"

#include <gtest/gtest.h>
#include <random>
#include <thread>

namespace Terrahertz::UnitTests {

struct NetworkTCPSocket : public testing::Test
{
    using TCPSocketV4 = TCPSocket<IPVersion::V4>;

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

TEST_F(NetworkTCPSocket, Listen)
{
    auto const address = getLocalAddress();
    EXPECT_TRUE(address);

    TCPSocketV4 sut{};
    EXPECT_TRUE(sut.bind(*address));
    EXPECT_TRUE(sut.listen(2U));
}

TEST_F(NetworkTCPSocket, EstablishConnection)
{
    auto const address = getLocalAddress();
    EXPECT_TRUE(address);

    TCPSocketV4 server{};
    EXPECT_TRUE(server.bind(*address));
    EXPECT_TRUE(server.listen(2U));

    EXPECT_FALSE(server.acceptIsNonblocking());
    EXPECT_FALSE(server.receiveIsNonblocking());
    EXPECT_FALSE(server.sendIsNonblocking());

    TCPSocketV4 client{};
    EXPECT_FALSE(client.receiveIsNonblocking());
    // Behavior is OS dependent
    // EXPECT_FALSE(client.sendIsNonblocking());
    EXPECT_TRUE(client.connect(*address));
    EXPECT_FALSE(client.receiveIsNonblocking());
    EXPECT_TRUE(client.sendIsNonblocking());

    // wait a bit to stabilize test result
    std::this_thread::sleep_for(std::chrono::milliseconds{10U});

    ASSERT_TRUE(server.acceptIsNonblocking());
    auto connectionSocket = server.accept(nullptr);
    ASSERT_TRUE(connectionSocket.good());
    EXPECT_FALSE(client.receiveIsNonblocking());
    EXPECT_TRUE(client.sendIsNonblocking());
}

TEST_F(NetworkTCPSocket, BidirectionalDataTransfer)
{
    auto const address = getLocalAddress();
    EXPECT_TRUE(address);

    TCPSocketV4 server{};
    EXPECT_TRUE(server.bind(*address));
    std::cout << errno << std::endl;
    EXPECT_TRUE(server.listen(2U));

    TCPSocketV4 client{};
    EXPECT_TRUE(client.connect(*address));

    // wait a bit to stabilize test result
    std::this_thread::sleep_for(std::chrono::milliseconds{10U});

    ASSERT_TRUE(server.acceptIsNonblocking());
    auto connectionSocket = server.accept(nullptr);

    std::array<std::uint8_t, 16U> sendBuffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::array<std::uint8_t, 32U> receiveBuffer{};

    std::span<std::byte const> sendSpan{reinterpret_cast<std::byte *>(sendBuffer.data()), sendBuffer.size()};
    std::span<std::byte>       receiveSpan{reinterpret_cast<std::byte *>(receiveBuffer.data()), receiveBuffer.size()};

    auto const sendResult0 = client.send(sendSpan);
    EXPECT_FALSE(sendResult0.isError());
    EXPECT_EQ(sendResult0.value(), sendBuffer.size());

    // wait a bit to stabilize test result
    std::this_thread::sleep_for(std::chrono::milliseconds{10U});

    ASSERT_TRUE(connectionSocket.receiveIsNonblocking());
    auto const receiveResult0 = connectionSocket.receive(receiveSpan);
    EXPECT_FALSE(receiveResult0.isError());
    EXPECT_EQ(receiveResult0.value().size(), sendBuffer.size());
    for (auto i = 0U; i < sendBuffer.size(); ++i)
    {
        EXPECT_EQ(sendBuffer[i], receiveBuffer[i]);
    }

    // Reset to test the other direction
    for (auto &value : receiveBuffer)
    {
        value = 0U;
    }

    auto const sendResult1 = connectionSocket.send(sendSpan);
    EXPECT_FALSE(sendResult1.isError());
    EXPECT_EQ(sendResult1.value(), sendBuffer.size());

    // wait a bit to stabilize test result
    std::this_thread::sleep_for(std::chrono::milliseconds{10U});

    ASSERT_TRUE(client.receiveIsNonblocking());
    auto const receiveResult1 = client.receive(receiveSpan);
    EXPECT_FALSE(receiveResult1.isError());
    EXPECT_EQ(receiveResult1.value().size(), sendBuffer.size());
    for (auto i = 0U; i < sendBuffer.size(); ++i)
    {
        EXPECT_EQ(sendBuffer[i], receiveBuffer[i]);
    }
}

} // namespace Terrahertz::UnitTests
