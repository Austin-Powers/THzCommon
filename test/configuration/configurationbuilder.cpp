#include "THzCommon/configuration/configurationbuilder.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct ConfigurationConfigrationBuilder : public testing::Test
{};

TEST_F(ConfigurationConfigrationBuilder, DefaultConstruction)
{
    ConfigurationBuilder sut{};
    std::string const    expectedResult = "\n";
    EXPECT_TRUE(sut.buildConfigurationString() == expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, AddEntryToRoot)
{
    ConfigurationBuilder sut{};
    sut.addEntry("test_no_comment", "123");
    sut.addEntry("test_with_comment", "abc", "comment");

    std::string const expectedResult = "test_no_comment = 123\ntest_with_comment = abc;comment\n";
    EXPECT_TRUE(sut.buildConfigurationString() == expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, AddEntryOverwritesExistingEntry)
{
    ConfigurationBuilder sut{};
    sut.addEntry("test", "123", "abc");
    sut.addEntry("test", "234", "bcd");

    std::string const expectedResult = "test = 234;bcd\n";
    EXPECT_TRUE(sut.buildConfigurationString() == expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, SubConfiguration)
{
    ConfigurationBuilder sut{};
    sut.addEntry("before", "123");
    auto &sub = sut.createSubConfiguration("subcon", "baba");
    sub.addEntry("test", "234", "comment");
    sut.addEntry("after", "345");

    std::string const expectedResult = "before = 123\nafter = 345\n\n[subcon];baba\ntest = 234;comment\n";
    EXPECT_TRUE(sut.buildConfigurationString() == expectedResult);
}

} // namespace Terrahertz::UnitTests
