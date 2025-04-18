#ifndef THZ_COMMON_UTILITY_RESULT_HPP
#define THZ_COMMON_UTILITY_RESULT_HPP

#include <cerrno>
#include <type_traits>
#include <variant>

/// @brief The type returned by calling errno.
using errno_t = std::remove_reference<decltype(errno)>::type;

namespace Terrahertz {

/// @brief Can be returned by function calls that might fail with an error code.
///
/// @tparam T The return type used in normal operation.
template <typename T>
class Result
{
public:
    /// @brief Creates a result signalling an error.
    ///
    /// @param errorCode The error code.
    /// @return The result instance carrying the error.
    static Result error(errno_t const errorCode) noexcept { return Result(errorCode); }

    /// @brief Creates a result signalling an error, using the current value of errno.
    ///
    /// @return The result instance carrying the error.
    static Result error() noexcept { return Result(errno); }

    /// @brief Initializes a new result instance carrying the result.
    ///
    /// @param result The result value.
    inline Result(T result) noexcept : _result{result} {}

    /// @brief Checks if this result carries an error.
    ///
    /// @return True if the result is an error, false otherwise.
    bool isError() const noexcept { return std::get_if<T>(&_result) == nullptr; }

    /// @brief Returns the error code.
    ///
    /// @return The error code, 0 if this is not an error.
    errno_t errorCode() const noexcept
    {
        auto const code = std::get_if<errno_t>(&_result);
        return (code == nullptr) ? errno_t{} : (*code);
    }

    /// @brief Returns the value of the result.
    ///
    /// @return The value of the result.
    /// @throws bad_variant_access If the result carries an error.
    T value() const noexcept(false) { return std::get<T>(_result); }

    /// @brief Returns the value of the result.
    ///
    /// @return The value of the result.
    /// @throws bad_variant_access If the result carries an error.
    operator T() const noexcept(false) { return std::get<T>(_result); }

private:
    inline Result(errno_t const errorCode) noexcept : _result{errorCode} {}

    std::variant<T, errno_t> _result;
};

/// @brief Specialization in case T is identical to errno_t.
template <>
class Result<errno_t>
{
public:
    /// @brief Creates a result signalling an error.
    ///
    /// @param errorCode The error code.
    /// @return The result instance carrying the error.
    static Result error(errno_t const errorCode) noexcept { return Result(errorCode, true); }

    /// @brief Creates a result signalling an error using the current value of errno.
    ///
    /// @return The result instance carrying the error.
    static Result error() noexcept { return Result(errno, true); }

    /// @brief Initializes a new result instance carrying the result.
    ///
    /// @param result The result value.
    inline Result(errno_t result) noexcept : _result{result}, _error{false} {}

    /// @brief Checks if this result carries an error.
    ///
    /// @return True if the result is an error, false otherwise.
    bool isError() const noexcept { return _error; }

    /// @brief Returns the error code.
    ///
    /// @return The error code, 0 if this is not an error.
    errno_t errorCode() const noexcept { return _error ? _result : errno_t{}; }

    /// @brief Returns the value of the result.
    ///
    /// @return The value of the result.
    /// @throws bad_variant_access If the result carries an error.
    errno_t value() const noexcept(false)
    {
        if (_error)
        {
            throw std::bad_variant_access{};
        }
        return _result;
    }

    /// @brief Returns the value of the result.
    ///
    /// @return The value of the result.
    /// @throws bad_variant_access If the result carries an error.
    operator errno_t() const noexcept(false)
    {
        if (_error)
        {
            throw std::bad_variant_access{};
        }
        return _result;
    }

private:
    inline Result(errno_t const errorCode, bool const errorFlag) noexcept : _result{errorCode}, _error{errorFlag} {}

    errno_t _result;

    bool _error;
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_RESULT_HPP
