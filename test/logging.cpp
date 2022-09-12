#include "THzCommon/logging/logging.h"

#include <cstdio>
#include <gtest/gtest.h>
#include <memory>
#include <string>

namespace Terrahertz::UnitTests {

struct TestProject
{
    static constexpr char const *name() noexcept { return "THzCommon.LoggingTests"; }
};

struct LongTestProject
{
    static constexpr char const *name() noexcept
    {
        return "THzCommon.LoggingTestsWithExtremlyLongNameForRegularLoggingPurposes";
    }
};

struct Logging_Logger : public testing::Test
{
    std::unique_ptr<Logger> logger{};

    std::string loggerFilepath{};

    void SetUp() noexcept override
    {
        logger = std::make_unique<Logger>();
        logger->setFilepath("test_");
        loggerFilepath = logger->filepath();
    }

    void TearDown() noexcept override { std::remove(loggerFilepath.c_str()); }
};

TEST_F(Logging_Logger, LogLevelCharactersCorrect)
{
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Error>(), 'E');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Warning>(), 'W');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Info>(), 'I');
    EXPECT_EQ(getLogLevelCharacter<LogLevel::Trace>(), 'T');
}

TEST_F(Logging_Logger, LoggerInitializedWithCorrectValues)
{
    Logger logger{};
    EXPECT_EQ(logger.maxLevel(), LogLevel::Error);
    EXPECT_TRUE(logger.filepath().empty());
    EXPECT_EQ(logger.maxProjectNameLength(), 17U);
}

TEST_F(Logging_Logger, FilepathGetsExtended)
{
    std::string filepath{"test_"};
    Logger      logger{};
    logger.setFilepath(filepath);
    auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char       timeString[sizeof "19700101_0000.log"];
    struct tm  tmTime;
    gmtime_s(&tmTime, &time);
    strftime(timeString, sizeof timeString, "%Y%m%d_%H%M.log", &tmTime);
    filepath += timeString;
    EXPECT_EQ(logger.filepath(), filepath);
}

TEST_F(Logging_Logger, LoggerIncreasesMaxNameLength)
{
    EXPECT_LT(logger->maxProjectNameLength(), std::strlen(TestProject::name()));
    logger->addProject<TestProject>();
    EXPECT_EQ(logger->maxProjectNameLength(), std::strlen(TestProject::name()));
}

TEST_F(Logging_Logger, MaxNameLengthLimited)
{
    logger->addProject<LongTestProject>();
    EXPECT_EQ(logger->maxProjectNameLength(), Logger::ProjectNameLengthLimit);
}

TEST_F(Logging_Logger, MessagesLoggedToFile)
{
    // LogLevel tests
    logger->addProject<TestProject>();
    logger->log<LogLevel::Error, TestProject>("ErrorError");
    logger->log<LogLevel::Warning, TestProject>("ErrorWarning");
    logger->log<LogLevel::Info, TestProject>("ErrorInfo");
    logger->log<LogLevel::Trace, TestProject>("ErrorTrace");
    logger->maxLevel() = LogLevel::Warning;
    logger->log<LogLevel::Error, TestProject>("WarningError");
    logger->log<LogLevel::Warning, TestProject>("WarningWarning");
    logger->log<LogLevel::Info, TestProject>("WarningInfo");
    logger->log<LogLevel::Trace, TestProject>("WarningTrace");
    logger->maxLevel() = LogLevel::Info;
    logger->log<LogLevel::Error, TestProject>("InfoError");
    logger->log<LogLevel::Warning, TestProject>("InfoWarning");
    logger->log<LogLevel::Info, TestProject>("InfoInfo");
    logger->log<LogLevel::Trace, TestProject>("InfoTrace");
    logger->maxLevel() = LogLevel::Trace;
    logger->log<LogLevel::Error, TestProject>("TraceError");
    logger->log<LogLevel::Warning, TestProject>("TraceWarning");
    logger->log<LogLevel::Info, TestProject>("TraceInfo");
    logger->log<LogLevel::Trace, TestProject>("TraceTrace");

    // Project tests
    logger->addProject<LongTestProject>();
    logger->log<LogLevel::Info, TestProject>("TestProject");
    logger->log<LogLevel::Info, LongTestProject>("LongTestProject");
    logger.reset();

    // check file content
    std::ifstream file{loggerFilepath};
    ASSERT_TRUE(file.is_open());
    auto const checkLine = [&file](std::string const &expectation) noexcept {
        static size_t timeCharacters = sizeof "0000-00-00 00:00:00:000" - 1U;
        std::string   line{};
        std::getline(file, line);
        ASSERT_FALSE(line.empty());
        EXPECT_STREQ(line.c_str() + timeCharacters, expectation.c_str());
    };

    checkLine(" E THzCommon.LoggingTests ErrorError");
    checkLine(" E THzCommon.LoggingTests WarningError");
    checkLine(" W THzCommon.LoggingTests WarningWarning");
    checkLine(" E THzCommon.LoggingTests InfoError");
    checkLine(" W THzCommon.LoggingTests InfoWarning");
    checkLine(" I THzCommon.LoggingTests InfoInfo");
    checkLine(" E THzCommon.LoggingTests TraceError");
    checkLine(" W THzCommon.LoggingTests TraceWarning");
    checkLine(" I THzCommon.LoggingTests TraceInfo");
    checkLine(" T THzCommon.LoggingTests TraceTrace");
    checkLine(" I THzCommon.LoggingTests TestProject");
    checkLine(" I THzCommon.LoggingTestsWithExtremlyLongNameForReg LongTestProject");
}

} // namespace Terrahertz::UnitTests
