#include "THzCommon/math/matrix.hpp"

#include "THzCommon/logging/logging.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Math_Matrix : public testing::Test
{
    double epsilon{1e-16};

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

    auto const scalar = 2.0;
    auto       index  = 1.0;

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

TEST_F(Math_Matrix, MultiplyByMatrix)
{
    Matrix<double, 4U, 4U> a{
        0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16};
    Matrix<double, 1U, 4U> b{1.0, 2.0, 3.0, 4.0};

    auto const c = a * b;
    EXPECT_NEAR(c(0U, 0U), 0.3, epsilon);
    EXPECT_NEAR(c(0U, 1U), 0.7, epsilon);
    EXPECT_NEAR(c(0U, 2U), 1.1, epsilon);
    EXPECT_NEAR(c(0U, 3U), 1.5, epsilon);
}

TEST_F(Math_Matrix, ConversionToArray)
{
    Matrix<double, 1U, 4U> const        sut{1.0, 2.0, 3.0, 4.0};
    Matrix<double, 1U, 4U>::StorageType arr = sut;
    for (auto i = 0U; i < sut.rows(); ++i)
    {
        EXPECT_EQ(arr[i], sut(0U, i));
    }
}

} // namespace Terrahertz::UnitTests
