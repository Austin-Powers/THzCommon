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

} // namespace Terrahertz::UnitTests
