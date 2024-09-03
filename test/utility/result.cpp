#include "THzCommon/utility/result.hpp"

#include <gtest/gtest.h>
#include <variant>

namespace Terrahertz::UnitTests {

struct UtilityResult : public testing::Test
{};

TEST_F(UtilityResult, BasicConstruction)
{
    Result<double> sut{0.815};
    EXPECT_FALSE(sut.isError());
    EXPECT_EQ(sut.errorCode(), 0);
    EXPECT_EQ(sut.value(), 0.815);
    EXPECT_EQ(static_cast<double>(sut), 0.815);
}

TEST_F(UtilityResult, ErrorConstruction)
{
    auto sut = Result<double>::error(123);
    EXPECT_TRUE(sut.isError());
    EXPECT_EQ(sut.errorCode(), 123);
    EXPECT_THROW(sut.value(), std::bad_variant_access);
    EXPECT_THROW(static_cast<double>(sut), std::bad_variant_access);
}

TEST_F(UtilityResult, BasicConstructionErrnoType)
{
    Result<errno_t> sut{2411};
    EXPECT_FALSE(sut.isError());
    EXPECT_EQ(sut.errorCode(), 0);
    EXPECT_EQ(sut.value(), 2411);
    EXPECT_EQ(static_cast<errno_t>(sut), 2411);
}

TEST_F(UtilityResult, ErrorConstructionErrnoType)
{
    auto sut = Result<errno_t>::error(123);
    EXPECT_TRUE(sut.isError());
    EXPECT_EQ(sut.errorCode(), 123);
    EXPECT_THROW(sut.value(), std::bad_variant_access);
    EXPECT_THROW(static_cast<errno_t>(sut), std::bad_variant_access);
}

} // namespace Terrahertz::UnitTests
