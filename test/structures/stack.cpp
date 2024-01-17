#include "THzCommon/structures/stack.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct StructuresStack : public testing::Test
{
    using TestStack = Stack<std::uint32_t, 8U>;

    TestStack sut{};

    void fillStack() noexcept
    {
        for (auto i = 0U; i < sut.size(); ++i)
        {
            sut.push(12U);
        }
    }
};

TEST_F(StructuresStack, StackEmptyOnConstruction)
{
    EXPECT_FALSE(sut.full());
    EXPECT_EQ(sut.filled(), 0U);
    EXPECT_TRUE(sut.data().empty());
    EXPECT_EQ(sut.top(), nullptr);
}

TEST_F(StructuresStack, PushElements)
{
    sut.push(23U);
    {
        EXPECT_FALSE(sut.full());
        EXPECT_EQ(sut.filled(), 1U);

        auto const data = sut.data();
        ASSERT_EQ(data.size(), 1U);
        EXPECT_EQ(data[0], 23U);
        EXPECT_NE(sut.top(), nullptr);
        EXPECT_EQ((*sut.top()), 23U);
    }

    sut.push(42U);
    {
        EXPECT_FALSE(sut.full());
        EXPECT_EQ(sut.filled(), 2U);

        auto const data = sut.data();
        ASSERT_EQ(data.size(), 2U);
        EXPECT_EQ(data[0], 23U);
        EXPECT_EQ(data[1], 42U);
        EXPECT_NE(sut.top(), nullptr);
        EXPECT_EQ((*sut.top()), 42U);
    }
}

TEST_F(StructuresStack, PushWhileFull)
{
    fillStack();
    EXPECT_TRUE(sut.full());
    EXPECT_EQ(sut.filled(), sut.size());

    sut.push(12U);
    EXPECT_TRUE(sut.full());
    EXPECT_EQ(sut.filled(), sut.size());
}

TEST_F(StructuresStack, Pop)
{
    sut.push(1U);
    sut.push(2U);
    sut.push(3U);
    EXPECT_EQ(sut.pop(), 3U);
    EXPECT_EQ(sut.filled(), 2U);
    EXPECT_EQ(sut.pop(), 2U);
    EXPECT_EQ(sut.filled(), 1U);
    EXPECT_EQ(sut.pop(), 1U);
    EXPECT_EQ(sut.filled(), 0U);
}

TEST_F(StructuresStack, PopOnFullStack)
{
    fillStack();
    sut.push(12U);
    EXPECT_EQ(sut.pop(), 12U);
    EXPECT_FALSE(sut.full());
}

TEST_F(StructuresStack, PopTill)
{
    fillStack();
    sut.popTill(5U);
    EXPECT_EQ(sut.filled(), 5U);
}

TEST_F(StructuresStack, Clear)
{
    fillStack();
    sut.clear();
    EXPECT_EQ(sut.filled(), 0U);
    sut.push(12U);
    sut.push(12U);
    sut.push(12U);
    sut.clear();
    EXPECT_EQ(sut.filled(), 0U);
}

} // namespace Terrahertz::UnitTests
