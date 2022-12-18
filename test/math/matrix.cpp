#include "THzCommon/math/matrix.hpp"

#include "THzCommon/logging/logging.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_Matrix : public testing::Test
{
    void SetUp() noexcept override { Logger::globalInstance().maxLevel() = LogLevel::Warning; }
};

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

TEST_F(Math_Matrix, RetrievingSetValues)
{
    Matrix<std::int32_t, 3U, 2U> sut{};

    auto index = 1;
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            sut(x, y) = index;
            ++index;
        }
    }
    index = 1;
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            EXPECT_EQ(sut(x, y), index);
            ++index;
        }
    }
}

TEST_F(Math_Matrix, InitializerList)
{
    Matrix<double, 3U, 2U> const sut{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

    auto index = 1;
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            EXPECT_EQ(sut(x, y), index);
            ++index;
        }
    }
}

TEST_F(Math_Matrix, MultiplyByScalar)
{
    Matrix<double, 3U, 2U> sut{1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

    auto const scalar  = 2.0;
    auto const epsilon = 1e-24;
    auto       index   = 1.0;

    sut *= scalar;
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            EXPECT_NEAR(sut(x, y), index * scalar, epsilon);
            ++index;
        }
    }

    auto sut2 = sut * scalar;
    for (auto y = 0U; y < sut.rows(); ++y)
    {
        for (auto x = 0U; x < sut.columns(); ++x)
        {
            EXPECT_NEAR(sut2(x, y), sut(x, y) * scalar, epsilon);
        }
    }
}

} // namespace Terrahertz::UnitTests
