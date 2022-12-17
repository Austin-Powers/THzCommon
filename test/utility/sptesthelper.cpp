#include "sptesthelper.hpp"

namespace Terrahertz::UnitTests {

struct DefaultInitTestHelper::Impl
{
    Impl() noexcept {}

    std::int32_t foo() const noexcept { return _bar; }

    std::int32_t _bar{-128};
};

DefaultInitTestHelper::DefaultInitTestHelper() noexcept { _impl.init(); }

DefaultInitTestHelper::~DefaultInitTestHelper() noexcept {}

std::int32_t DefaultInitTestHelper::foo() const noexcept { return _impl->foo(); }

struct StaticPImplTestHelper::Impl
{
    Impl(std::int32_t const number) noexcept : _number{number} {}

    std::int32_t _number;
};

StaticPImplTestHelper::StaticPImplTestHelper(std::int32_t const number) noexcept : _impl{} { _impl.init(number); }

StaticPImplTestHelper::~StaticPImplTestHelper() noexcept {}

std::int32_t StaticPImplTestHelper::getNumber() const noexcept { return _impl->_number; }

} // namespace Terrahertz::UnitTests
