#include "THzCommon/configuration/configurationstorage.hpp"

#include <gtest/gtest.h>
#include <string_view>

namespace Terrahertz::UnitTests {

struct ConfigurationConfigurationStorage : public testing::Test
{};

TEST_F(ConfigurationConfigurationStorage, LoadFromStringFailsOnInvalidInput)
{
    std::string_view     testData = "[test \n";
    ConfigurationStorage sut{};
    EXPECT_FALSE(sut.load(testData));
}

TEST_F(ConfigurationConfigurationStorage, LoadFromStringSuccessful)
{
    std::string_view testData = "test = 123;comment\n"
                                "[section]\n"
                                ";comment"
                                "test = 2234\n"
                                "test2 = test\n"
                                "[section.subsection]\n"
                                "test = none\n";

    ConfigurationStorage sut{};
    EXPECT_TRUE(sut.load(testData));
}

TEST_F(ConfigurationConfigurationStorage, LoadFromStringOverwritesOldConfig) {}

TEST_F(ConfigurationConfigurationStorage, LoadFromFile) {}

} // namespace Terrahertz::UnitTests
