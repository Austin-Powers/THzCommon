#include "THzCommon/utility/staticPImpl.hpp"

#include "sptesthelper.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityStaticPImpl : public testing::Test
{};

TEST_F(UtilityStaticPImpl, DefaultInitialization)
{
    DefaultInitTestHelper helper{};
    EXPECT_EQ(helper.foo(), -128);
}

TEST_F(UtilityStaticPImpl, InitializationWithParameters)
{
    StaticPImplTestHelper helper{22};
    EXPECT_EQ(helper.getNumber(), 22);
}

} // namespace Terrahertz::UnitTests
