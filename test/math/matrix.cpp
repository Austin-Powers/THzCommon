#include "THzCommon/math/matrix.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_Matrix : public testing::Test
{};

TEST_F(Math_Matrix, DefaultConstruction)
{
    Matrix<double, 3U, 2U> const sut{};
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            EXPECT_EQ(sut(x, y), double{});
        }
    }
}

} // namespace Terrahertz::UnitTests
