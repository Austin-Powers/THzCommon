#include "THzCommon/configuration/configurationbuilder.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct ConfigurationConfigrationBuilder : public testing::Test
{};

TEST_F(ConfigurationConfigrationBuilder, DefaultConstruction)
{
    ConfigurationBuilder sut{};
    std::string const    expectedResult = "\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, AddEntryToRoot)
{
    ConfigurationBuilder sut{};
    sut.addEntry("test_no_comment", "123");
    sut.addEntry("test_with_comment", "abc", "comment");

    std::string const expectedResult = "test_no_comment = 123\ntest_with_comment = abc;comment\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, AddEntryOverwritesExistingEntry)
{
    ConfigurationBuilder sut{};
    sut.addEntry("test", "123", "abc");
    sut.addEntry("test", "234", "bcd");

    std::string const expectedResult = "test = 234;bcd\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, BasicSubConfiguration)
{
    ConfigurationBuilder sut{};
    sut.addEntry("before", "123");
    auto &sub = sut.getCreateSubConfiguration("subcon", "baba");
    sub.addEntry("test", "234", "comment");
    sut.addEntry("after", "345");

    std::string const expectedResult = "before = 123\nafter = 345\n[subcon];baba\ntest = 234;comment\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, RetrievingAlreadyExistingSubConfiguration)
{
    ConfigurationBuilder sut{};

    auto &subA = sut.getCreateSubConfiguration("subcon");
    subA.addEntry("A", "1");

    auto &subB = sut.getCreateSubConfiguration("subcon", "not going to happen");
    subB.addEntry("B", "2");

    std::string const expectedResult = "[subcon]\nA = 1\nB = 2\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, MultipleSubConfigurations)
{
    ConfigurationBuilder sut{};

    auto &subA = sut.getCreateSubConfiguration("subA");
    subA.addEntry("A", "1");
    auto &subB = sut.getCreateSubConfiguration("subB");
    subB.addEntry("B", "2");

    std::string const expectedResult = "[subA]\nA = 1\n[subB]\nB = 2\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

TEST_F(ConfigurationConfigrationBuilder, NestedSubConfigurations)
{
    ConfigurationBuilder sut{};

    auto &subA = sut.getCreateSubConfiguration("subA");
    subA.addEntry("A", "1");
    auto &subAA = subA.getCreateSubConfiguration("A", "nesting");
    subAA.addEntry("A", "10");
    auto &subAAA = subAA.getCreateSubConfiguration("AA", "nesting in nesting");
    subAAA.addEntry("A", "1024");
    auto &subB = sut.getCreateSubConfiguration("subB");
    subB.addEntry("B", "2");

    std::string const expectedResult =
        "[subA]\nA = 1\n[subA.A];nesting\nA = 10\n[subA.A.AA];nesting in nesting\nA = 1024\n[subB]\nB = 2\n";
    EXPECT_EQ(sut.buildConfigurationString(), expectedResult);
}

} // namespace Terrahertz::UnitTests
