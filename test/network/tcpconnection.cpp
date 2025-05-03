#include "THzCommon/network/tcpconnection.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct NetworkTCPConnection : public testing::Test
{};

TEST_F(NetworkTCPConnection, EmptyAddress)
{
    Address<IPVersion::V4> serverAddress{};
    Address<IPVersion::V4> clientAddress{};

    TCPConnection<IPVersion::V4> sutServer{serverAddress, true};
    TCPConnection<IPVersion::V4> sutClient{clientAddress};

    std::array<std::uint8_t, 16U> buffer{};
    std::span<std::uint8_t>       span{buffer};

    EXPECT_TRUE(sutServer.receive(span).isError());
    EXPECT_TRUE(sutClient.receive(span).isError());
    EXPECT_TRUE(sutServer.send(span).isError());
    EXPECT_TRUE(sutClient.send(span).isError());
}

TEST_F(NetworkTCPConnection, OtherSideNotAvailable)
{
    Address<IPVersion::V4> serverAddress{{127, 0, 0, 1}, 15001};
    Address<IPVersion::V4> clientAddress{{127, 0, 0, 1}, 15000};

    TCPConnection<IPVersion::V4> sutServer{serverAddress, true};
    TCPConnection<IPVersion::V4> sutClient{clientAddress};

    std::array<std::uint8_t, 16U> buffer{};
    std::span<std::uint8_t>       span{buffer};

    auto receiveResult = sutClient.receive(span);
    ASSERT_FALSE(receiveResult.isError());
    EXPECT_EQ(receiveResult.value().size(), 0U);

    receiveResult = sutServer.receive(span);
    ASSERT_FALSE(receiveResult.isError());
    EXPECT_EQ(receiveResult.value().size(), 0U);

    auto sendResult = sutClient.send(span);
    ASSERT_FALSE(sendResult.isError());
    EXPECT_EQ(sendResult.value(), 0U);

    sendResult = sutServer.send(span);
    ASSERT_FALSE(sendResult.isError());
    EXPECT_EQ(sendResult.value(), 0U);
}

TEST_F(NetworkTCPConnection, TransferData)
{
    Address<IPVersion::V4> const address{{127, 0, 0, 1}, 14400};

    TCPConnection<IPVersion::V4> sutServer{address, true};
    TCPConnection<IPVersion::V4> sutClient{address};

    std::array<std::uint8_t, 16U> inputBuffer{};
    std::array<std::uint8_t, 16U> outputBuffer{};
    std::span<std::uint8_t>       inputSpan{inputBuffer};
    std::span<std::uint8_t>       outputSpan{outputBuffer};
}

} // namespace Terrahertz::UnitTests
