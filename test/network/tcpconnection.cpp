#include "THzCommon/network/tcpconnection.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct NetworkTCPConnection : public testing::Test
{};

TEST_F(NetworkTCPConnection, EmptyAddress)
{
    Address<IPVersion::V4> clientAddress{};
    Address<IPVersion::V4> serverAddress{};

    TCPConnection sutClient{TCPConnection<IPVersion::V4>::Role::Client, clientAddress};
    TCPConnection sutServer{TCPConnection<IPVersion::V4>::Role::Server, serverAddress};

    std::array<std::uint8_t, 16U> buffer{};
    std::span<std::uint8_t>       span{buffer};

    EXPECT_TRUE(sutClient.receive(span).isError());
    EXPECT_TRUE(sutServer.receive(span).isError());
    EXPECT_TRUE(sutClient.send(span).isError());
    EXPECT_TRUE(sutServer.send(span).isError());
}

TEST_F(NetworkTCPConnection, OtherSideNotAvailable)
{
    Address<IPVersion::V4> clientAddress{{127, 0, 0, 1}, 15000};
    Address<IPVersion::V4> serverAddress{{127, 0, 0, 1}, 15001};

    TCPConnection sutClient{TCPConnection<IPVersion::V4>::Role::Client, clientAddress};
    TCPConnection sutServer{TCPConnection<IPVersion::V4>::Role::Server, serverAddress};

    std::array<std::uint8_t, 16U> buffer{};
    std::span<std::uint8_t>       span{buffer};

    ASSERT_FALSE(sutClient.receive(span).isError());
    ASSERT_FALSE(sutServer.receive(span).isError());
    ASSERT_FALSE(sutClient.send(span).isError());
    ASSERT_FALSE(sutServer.send(span).isError());
}

} // namespace Terrahertz::UnitTests
