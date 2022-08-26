#include "Logging/Logging.h"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct TestProject
{
    static constexpr char const *name() noexcept { return "Test"; }
};

struct LoggingTests : public testing::Test
{};

TEST_F(LoggingTests, LogLevelCharactersCorrect)
{
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Error>(), 'E');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Warning>(), 'W');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Info>(), 'I');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Trace>(), 'T');
}

TEST_F(LoggingTests, NewLoggerHasEmptyFilepath) {}

TEST_F(LoggingTests, LogLevelsAreCorrectlyUsed) {}

TEST_F(LoggingTests, TimeMostlyRight) {}

TEST_F(LoggingTests, SetProjectNameMaxLength) {}

TEST_F(LoggingTests, Test0)
{
    Logger::globalInstance().setMaxLevel(LogLevel::Trace);
    logMessage<LogLevel::Info, TestProject>("Test");
}

} // namespace Terrahertz::UnitTests
