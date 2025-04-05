#include "THzCommon/configuration/configurationstorage.hpp"

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

TEST_F(ConfigurationConfigurationStorage, LoadFromStringOverwritesOldConfig) {}

TEST_F(ConfigurationConfigurationStorage, LoadFromFile) {}

} // namespace Terrahertz::UnitTests
