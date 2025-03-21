#include "THzCommon/configuration/configuration.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct ConfigurationConfiguration : public testing::Test
{
    std::map<std::string, std::string> map{};
};

TEST_F(ConfigurationConfiguration, DefaultConstruction)
{
    Configuration sut{};
    EXPECT_FALSE(sut.isPresent("unknown"));
    EXPECT_TRUE(sut.valueOf("unknown").empty());
    EXPECT_EQ(sut.subConfiguration("unknown"), nullptr);
}

TEST_F(ConfigurationConfiguration, UnknownKeyIsNotPresent)
{
    Configuration sut{map};
    EXPECT_FALSE(sut.isPresent("unknown"));
    EXPECT_TRUE(sut.valueOf("unknown").empty());
    EXPECT_EQ(sut.subConfiguration("unknown"), nullptr);
}

TEST_F(ConfigurationConfiguration, RetrieveConfigurationValue)
{
    map.emplace("my_key", "awesome_value");
    Configuration sut{map};
    EXPECT_TRUE(sut.isPresent("my_key"));
    EXPECT_TRUE(sut.valueOf("my_key") == "awesome_value");
    EXPECT_EQ(sut.subConfiguration("my_key"), nullptr);
}

TEST_F(ConfigurationConfiguration, RetrieveSubConfiguration)
{
    map.emplace("my.awesome.key", "sub_value");
    Configuration sut{map};
    EXPECT_TRUE(sut.isPresent("my"));
    EXPECT_TRUE(sut.valueOf("my") == "");
}

} // namespace Terrahertz::UnitTests
