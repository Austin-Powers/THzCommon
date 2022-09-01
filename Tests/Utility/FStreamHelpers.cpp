#include "Utility/FStreamHelpers.h"

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

static constexpr char fileName[]{"test.data"};

struct Utility_FStreamHelpers : public testing::Test
{
    std::ofstream outputStream{fileName, std::ios::binary};

    void SetUp() noexcept override {}

    void TearDown() noexcept override
    {
        if (outputStream.is_open())
        {
            outputStream.close();
        }
        std::remove(fileName);
    }
};

TEST_F(Utility_FStreamHelpers, WritingSingleItemToStream)
{
    std::int32_t const iValue{-1234};
    double const       dValue{1337e-2};

    writeToStream(outputStream, iValue);
    writeToStream(outputStream, dValue);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};
    std::uint32_t iLoaded{};
    double        dLoaded{};
    ASSERT_TRUE(inputStream.is_open());
    inputStream.read(std::bit_cast<char *>(&iLoaded), sizeof(iLoaded));
    inputStream.read(std::bit_cast<char *>(&dLoaded), sizeof(dLoaded));
    EXPECT_EQ(iValue, iLoaded);
    EXPECT_EQ(dValue, dLoaded);
}

TEST_F(Utility_FStreamHelpers, WritingArrayToStream) {}

TEST_F(Utility_FStreamHelpers, WritingSpanToStream) {}

TEST_F(Utility_FStreamHelpers, ReadFromStreamWithClosedFile) {}

TEST_F(Utility_FStreamHelpers, ReadFromStreamWhileFileIsEnding) {}

TEST_F(Utility_FStreamHelpers, ReadSingleItemFromStream) {}

TEST_F(Utility_FStreamHelpers, ReadArrayFromStream) {}

TEST_F(Utility_FStreamHelpers, ReadSpanFromStreamIncomplete) {}

TEST_F(Utility_FStreamHelpers, ReadSpanFromStreamComplete) {}

} // namespace Terrahertz::UnitTests
