#include "THzCommon/configuration/configurationstorage.hpp"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string_view>

namespace Terrahertz::UnitTests {

struct ConfigurationConfigurationStorage : public testing::Test
{};

TEST_F(ConfigurationConfigurationStorage, LoadFromStringFailsOnBrokenSection)
{
    std::string_view     testData = "[test \n";
    ConfigurationStorage sut{};
    EXPECT_FALSE(sut.load(testData));
}

TEST_F(ConfigurationConfigurationStorage, LoadFromStringFailsOnInvalidKeyValuePair)
{
    std::string_view     testData = "key";
    ConfigurationStorage sut{};
    EXPECT_FALSE(sut.load(testData));
    testData = "= test";
    EXPECT_FALSE(sut.load(testData));
}

TEST_F(ConfigurationConfigurationStorage, LoadFromStringSuccessful)
{
    std::string_view testData = "test = 123;comment\n"
                                "[section]\n"
                                ";comment\n"
                                "test = 2234\n"
                                "test2 = test\n"
                                "empty = \n"
                                "\n"
                                "[section.subsection]\n"
                                "test = none\n";

    ConfigurationStorage sut{};
    EXPECT_TRUE(sut.load(testData));
    auto const config = sut.configuration();
    EXPECT_TRUE(config.valueOf("test") == "123");
    auto const configSection = config.subConfiguration("section");
    ASSERT_NE(configSection, nullptr);
    EXPECT_TRUE(configSection->valueOf("test") == "2234");
    EXPECT_TRUE(configSection->valueOf("test2") == "test");
    EXPECT_TRUE(configSection->valueOf("empty") == "");
    auto const configSubsection = configSection->subConfiguration("subsection");
    ASSERT_NE(configSubsection, nullptr);
    EXPECT_TRUE(configSubsection->valueOf("test") == "none");
}

TEST_F(ConfigurationConfigurationStorage, LoadFromStringOverwritesOldConfig)
{
    ConfigurationStorage sut{};

    std::string_view testData0 = "foo = 123\n";
    EXPECT_TRUE(sut.load(testData0));
    auto config = sut.configuration();
    EXPECT_TRUE(config.valueOf("foo") == "123");
    EXPECT_TRUE(config.valueOf("bar") == "");

    std::string_view testData1 = "bar = 234\n";
    EXPECT_TRUE(sut.load(testData1));
    config = sut.configuration();
    EXPECT_TRUE(config.valueOf("foo") == "");
    EXPECT_TRUE(config.valueOf("bar") == "234");
}

TEST_F(ConfigurationConfigurationStorage, LoadFromNonexistentFile)
{
    std::filesystem::path testFilePath = "nope.config";

    ConfigurationStorage sut{};
    EXPECT_FALSE(sut.load(testFilePath));
}

TEST_F(ConfigurationConfigurationStorage, LoadFromFile)
{
    // Create the test file
    std::filesystem::path testFilePath = "test.config";
    std::ofstream         testFile(testFilePath);
    testFile << "[test]" << std::endl;
    testFile << ";test" << std::endl;
    testFile << "test = 123" << std::endl;
    testFile << "empty =" << std::endl;
    testFile.close();

    ConfigurationStorage sut{};
    EXPECT_TRUE(sut.load(testFilePath));
    auto const configuration = sut.configuration();
    auto const subConfig     = configuration.subConfiguration("test");
    ASSERT_NE(subConfig, nullptr);
    EXPECT_TRUE(subConfig->valueOf("test") == "123");
    EXPECT_TRUE(subConfig->valueOf("empty") == "");

    // Remove the test file
    std::filesystem::remove(testFilePath.c_str());
}

} // namespace Terrahertz::UnitTests
