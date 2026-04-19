#include "THzCommon/memory/addresshelper.hpp"

#include <gtest/gtest.h>
#include <string>

namespace Terrahertz::UnitTests {

struct MemoryAddressHelper : public testing::Test
{};

TEST_F(MemoryAddressHelper, SameAddressWorksForIdenticalType)
{
    int test0{};
    int test1{};

    EXPECT_TRUE(sameAddress(&test0, &test0));
    EXPECT_FALSE(sameAddress(&test0, &test1));
}

TEST_F(MemoryAddressHelper, SameAddressWorksForDifferentTypes)
{
    int         test0{};
    char        test1{};
    double      test2{};
    std::string test3{};

    EXPECT_FALSE(sameAddress(&test0, &test1));
    EXPECT_FALSE(sameAddress(&test0, &test2));
    EXPECT_FALSE(sameAddress(&test0, &test3));
    EXPECT_FALSE(sameAddress(&test1, &test2));
    EXPECT_FALSE(sameAddress(&test1, &test3));
    EXPECT_FALSE(sameAddress(&test2, &test3));

    EXPECT_TRUE(sameAddress(&test0, reinterpret_cast<char *>(&test0)));
    EXPECT_TRUE(sameAddress(&test2, reinterpret_cast<char *>(&test2)));
    EXPECT_TRUE(sameAddress(&test3, reinterpret_cast<char *>(&test3)));
}

} // namespace Terrahertz::UnitTests
