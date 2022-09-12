#include "Utility/FStreamHelpers.h"

#include <array>
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

TEST_F(Utility_FStreamHelpers, WritingContainerToStream)
{
    std::array<double, 4U> container{1.337, 0.815, 42.23, 47.11};
    writeToStream(outputStream, container);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};
    ASSERT_TRUE(inputStream.is_open());

    double loaded{};
    for (auto const expected : container)
    {
        inputStream.read(std::bit_cast<char *>(&loaded), sizeof(double));
        EXPECT_EQ(expected, loaded);
    }
}

TEST_F(Utility_FStreamHelpers, ReadFromStreamWithClosedFile)
{
    std::ifstream inputStream{};

    double value{};
    EXPECT_FALSE(readFromStream(inputStream, value));
}

TEST_F(Utility_FStreamHelpers, ReadFromStreamWhileFileIsEnding)
{
    std::uint16_t toWrite{};
    writeToStream(outputStream, toWrite);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};
    double        toRead{};
    EXPECT_FALSE(readFromStream(inputStream, toRead));
}

TEST_F(Utility_FStreamHelpers, ReadSingleItemFromStream)
{
    double toWrite{13.37};
    writeToStream(outputStream, toWrite);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};
    double        toRead{};
    EXPECT_TRUE(readFromStream(inputStream, toRead));
    EXPECT_EQ(toWrite, toRead);
}

TEST_F(Utility_FStreamHelpers, ReadContainerFromClosedStream)
{
    std::ifstream inputStream{};

    std::array<double, 4U> container{};
    EXPECT_EQ(readFromStream(inputStream, container), 0U);
}

TEST_F(Utility_FStreamHelpers, ReadSpanFromStreamIncomplete)
{
    std::array<double, 3U> toWrite{08.15, 46.11, 42.0};
    writeToStream(outputStream, toWrite);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};

    std::array<double, 4U> toRead{};
    ASSERT_EQ(readFromStream(inputStream, toRead), toWrite.size());
    for (auto i = 0U; i < toWrite.size(); ++i)
    {
        EXPECT_EQ(toRead[i], toWrite[i]);
    }
}

TEST_F(Utility_FStreamHelpers, ReadSpanFromStreamComplete)
{
    std::array<double, 4U> toWrite{13.37, 08.15, 46.11, 42.0};
    writeToStream(outputStream, toWrite);
    outputStream.close();

    std::ifstream inputStream{fileName, std::ios::binary};

    std::array<double, 4U> toRead{};
    ASSERT_EQ(readFromStream(inputStream, toRead), toWrite.size());
    for (auto i = 0U; i < toWrite.size(); ++i)
    {
        EXPECT_EQ(toRead[i], toWrite[i]);
    }
}

} // namespace Terrahertz::UnitTests
