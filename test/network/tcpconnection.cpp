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

    EXPECT_FALSE(sutServer.establish());
    EXPECT_FALSE(sutClient.establish());
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

    EXPECT_FALSE(sutServer.establish());
    EXPECT_FALSE(sutClient.establish());

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

/// @brief Fills the given buffer with test data.
///
/// @param buffer The buffer to fill.
/// @param length The amount of bytes to fill.
/// @param startValue The value to begin the sequence with.
/// @return A span over the filled bytes.
std::span<std::uint8_t> createTestDataSpan(
    std::array<std::uint8_t, 16U> &buffer,
    std::uint8_t const length,
    std::uint8_t startValue) noexcept
{
    std::span<std::uint8_t> result{buffer};
    result = result.subspan(length);
    for(auto &slot : result)
    {
        slot = startValue;
        ++startValue;
    }
    return result;
}

TEST_F(NetworkTCPConnection, TransferData)
{
    Address<IPVersion::V4> const address{{127, 0, 0, 1}, 14400};

    TCPConnection<IPVersion::V4> sutServer{address, true};
    TCPConnection<IPVersion::V4> sutClient{address};

    std::array<std::uint8_t, 16U> inputBuffer{};
    std::array<std::uint8_t, 16U> outputBuffer{};
    std::span<std::uint8_t>       outputSpan{outputBuffer};

    // establish
    //ASSERT_FALSE(sutServer.establish());
    //ASSERT_TRUE(sutClient.establish());
    //ASSERT_TRUE(sutServer.establish());

    // client -> server
    //std::uint8_t bytesToSend = 8U;
    //auto inputSpan = createTestDataSpan(inputBuffer, bytesToSend, 4U);
    //auto sendResult = sutClient.send(inputSpan);
    //ASSERT_FALSE(sendResult.isError());
    //EXPECT_EQ(sendResult.value(), bytesToSend);

    // server -> client
}

TEST_F(NetworkTCPConnection, OneSideClientSideCloses){}

TEST_F(NetworkTCPConnection, OneSideServerSideCloses){}

} // namespace Terrahertz::UnitTests
