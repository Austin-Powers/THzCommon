#ifndef THZ_TEST_COMMON_UTILITY_SPTESTHELPER_HPP
#define THZ_TEST_COMMON_UTILITY_SPTESTHELPER_HPP

#include "THzCommon/utility/staticPImpl.hpp"

#include <cstdint>

namespace Terrahertz::UnitTests {

/// @brief Class for testing the StaticPImpl helper with a default constructor.
class DefaultInitTestHelper
{
public:
    DefaultInitTestHelper() noexcept;

    ~DefaultInitTestHelper() noexcept;

    std::int32_t foo() const noexcept;

private:
    struct Impl;

    StaticPImpl<Impl, 4U> _impl{};
};

/// @brief Class for testing the StaticPImpl helper.
class StaticPImplTestHelper
{
public:
    StaticPImplTestHelper(std::int32_t const number) noexcept;

    ~StaticPImplTestHelper() noexcept;

    std::int32_t getNumber() const noexcept;

private:
    struct Impl;

    StaticPImpl<Impl, 4U> _impl;
};

} // namespace Terrahertz::UnitTests

#endif // !THZ_TEST_COMMON_UTILITY_SPTESTHELPER_HPP
