#include "THzCommon/structures/queue.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct StructuresQueue : public testing::Test
{
    using TestQueue = Queue<std::uint32_t, 8U>;

    TestQueue sut{};

    void fillQueue() noexcept
    {
        for (auto i = 0U; i < sut.size(); ++i)
        {
            sut.push(12U);
        }
    }
};

TEST_F(StructuresQueue, QueueEmptyOnConstruction)
{
    EXPECT_TRUE(sut.empty());
    EXPECT_EQ(sut.filled(), 0U);
    EXPECT_TRUE(sut.data().empty());
}

TEST_F(StructuresQueue, PushElements)
{
    sut.push(23U);
    {
        EXPECT_FALSE(sut.empty());
        EXPECT_EQ(sut.filled(), 1U);
        auto const data = sut.data();
        ASSERT_EQ(data.size(), 1U);
        EXPECT_EQ(data[0], 23U);
    }
    sut.push(42U);
    {
        EXPECT_FALSE(sut.empty());
        EXPECT_EQ(sut.filled(), 2U);
        auto const data = sut.data();
        ASSERT_EQ(data.size(), 2U);
        EXPECT_EQ(data[0], 23U);
        EXPECT_EQ(data[1], 42U);
    }
}

TEST_F(StructuresQueue, PushWhileFull)
{
    fillQueue();
    EXPECT_EQ(sut.filled(), sut.size());
    sut.push(23U);
    EXPECT_EQ(sut.filled(), sut.size());
    for (auto const value : sut.data())
    {
        EXPECT_EQ(value, 12U);
    }
}

TEST_F(StructuresQueue, Pop)
{
    sut.push(1U);
    sut.push(2U);
    sut.push(3U);
    sut.push(4U);
    sut.push(5U);
    sut.pop(2U);
    auto const data = sut.data();
    ASSERT_EQ(data.size(), 3U);
    EXPECT_EQ(data[0U], 3U);
    EXPECT_EQ(data[1U], 4U);
    EXPECT_EQ(data[2U], 5U);
}

TEST_F(StructuresQueue, PopOnFullQueue)
{
    fillQueue();
    sut.push(1U);
    sut.pop(1U);
    EXPECT_EQ(sut.filled(), sut.size() - 1U);
    EXPECT_EQ(sut.data().size(), sut.size() - 1U);
}

TEST_F(StructuresQueue, PopMoreThanIsInTheQueue)
{
    sut.push(1U);
    sut.push(2U);
    sut.push(3U);
    sut.push(4U);
    sut.push(5U);
    sut.pop(12U);
    EXPECT_TRUE(sut.empty());
    fillQueue();
    sut.pop(12U);
    EXPECT_TRUE(sut.empty());
}

} // namespace Terrahertz::UnitTests
