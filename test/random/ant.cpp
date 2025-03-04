#include "THzCommon/random/ant.hpp"

#include <cstdlib>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct RandomAnt : public testing::Test
{};

TEST_F(RandomAnt, GenerateRandomNumbers)
{
    Ant  sut{};
    auto previous = sut.nextByte();
    auto current  = sut.nextByte();

    auto diffCounter = 0U;
    for (auto i = 0U; i < 128U; ++i)
    {
        previous = current;
        current  = sut.nextByte();
        if (std::abs(static_cast<int>(previous) - current) > 10)
        {
            ++diffCounter;
        }
    }
    EXPECT_GE(diffCounter, 75U);
}

TEST_F(RandomAnt, DistributionOfRandomNumbersIsUniform)
{
    Ant sut{};

    std::array<std::uint16_t, 16U> distribution{};

    uint16_t const chunksize = 256U / distribution.size();
    for (auto i = 0U; i < 0xFFFFU; ++i)
    {
        auto const randomNumber = sut.nextByte();
        for (auto j = 1U; j <= distribution.size(); ++j)
        {
            if (randomNumber < (j * chunksize))
            {
                ++distribution[j - 1U];
                break;
            }
        }
    }
    for (auto i = 0U; i < distribution.size(); ++i)
    {
        EXPECT_GE(distribution[i], 3900U);
    }
}

TEST_F(RandomAnt, StepCounterWorkingCorrectly)
{
    Ant sut{};
    for (auto i = 0U; i < 16U; ++i)
    {
        EXPECT_EQ(sut.step(), i * 8U);
        sut.nextByte();
    }
}

TEST_F(RandomAnt, SynchronisationBackwards)
{
    Ant sut{};

    std::array<std::uint8_t, 8U> numbers{};
    for (auto &i : numbers)
    {
        i = sut.nextByte();
    }
    sut.syncTo(0);
    for (auto const i : numbers)
    {
        EXPECT_EQ(sut.nextByte(), i);
    }
}

TEST_F(RandomAnt, SynchronisationForwards)
{
    Ant sut{};

    std::array<std::uint8_t, 12U> numbers{};
    for (auto &i : numbers)
    {
        i = sut.nextByte();
    }
    sut.syncTo(0);
    sut.syncTo(32);
    for (auto i = 0U; i < numbers.size(); ++i)
    {
        if (i > 3)
        {
            EXPECT_EQ(sut.nextByte(), numbers[i]);
        }
    }
}

TEST_F(RandomAnt, SavingToTooSmallBufferFails)
{
    Ant                    sut{};
    std::array<char, 128U> buffer{};
    gsl::span<char>        bufferSpan{buffer.data(), buffer.size()};

    auto const encodedSpan = sut.save(bufferSpan);
    EXPECT_EQ(encodedSpan.size(), 0U);
}

TEST_F(RandomAnt, LoadingFromTooSmallBufferFails)
{
    Ant                    sut{};
    std::array<char, 128U> buffer{};
    gsl::span<char>        bufferSpan{buffer.data(), buffer.size()};

    EXPECT_FALSE(sut.load(bufferSpan));
}

TEST_F(RandomAnt, SaveAndLoad)
{
    Ant base{};
    Ant copy{};

    base.nextByte();
    base.nextByte();

    std::array<char, 4096U> buffer{};
    gsl::span<char>         bufferSpan{buffer.data(), buffer.size()};

    auto const encodedData = base.save(bufferSpan);
    EXPECT_EQ(encodedData.size(), 2744U);
    ASSERT_TRUE(copy.load(encodedData));
    EXPECT_EQ(base.step(), copy.step());

    EXPECT_EQ(base._posX, copy._posX);
    EXPECT_EQ(base._posY, copy._posY);
    EXPECT_EQ(base._direction, copy._direction);
    EXPECT_EQ(base._sequence, copy._sequence);

    for (auto i = 0U; i < 16; ++i)
    {
        EXPECT_EQ(base.nextByte(), copy.nextByte());
    }
}

} // namespace Terrahertz::UnitTests
