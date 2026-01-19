#ifndef THZ_COMMON_UTILITY_STRINGHELPERS_HPP
#define THZ_COMMON_UTILITY_STRINGHELPERS_HPP

#include <cctype>
#include <concepts>
#include <ranges>
#include <string>

namespace Terrahertz {

// clang-format off

/// @brief Concept of a character type.
template <typename T>
concept Character = requires {
    { typename std::char_traits<T>::char_type{} } -> std::same_as<T>;
};

// clang-format on

/// @brief Changes the case of the given character to lower case.
///
/// @tparam TChar The type of character.
/// @param c The character.
/// @return The lower case character.
template <Character TChar>
TChar toLower(TChar const c) noexcept
{
    return std::tolower(c);
}

/// @brief Changes the case of the given character to upper case.
///
/// @tparam TChar The type of character.
/// @param c The character.
/// @return The upper case character.
template <Character TChar>
TChar toUpper(TChar const c) noexcept
{
    return std::toupper(c);
}

/// @brief Changes all characters of the given string to lower case.
///
/// @tparam TChar The type of character used by the string.
/// @param input The string to change to lower case.
template <Character TChar>
void toLowerInplace(std::basic_string<TChar> &input) noexcept
{
    for (auto &c : input)
    {
        c = toLower(c);
    }
}

/// @brief Changes all characters of the given string to upper case.
///
/// @tparam TChar The type of character used by the string.
/// @param input The string to change to upper case.
template <Character TChar>
void toUpperInplace(std::basic_string<TChar> &input) noexcept
{
    for (auto &c : input)
    {
        c = toUpper(c);
    }
}

/// @brief Returns a copy of the given string with all characters changed to lower case.
///
/// @tparam TChar The type of character used by the string.
/// @param input The string to change to lower case.
/// @return A copy of the given string with all characters in lower case.
template <Character TChar>
std::basic_string<TChar> toLower(std::basic_string<TChar> const &input) noexcept
{
    std::basic_string<TChar> result = input;
    toLowerInplace(result);
    return result;
}

/// @brief Returns a copy of the given string with all characters changed to upper case.
///
/// @tparam TChar The type of character used by the string.
/// @param input The string to change to upper case.
/// @return A copy of the given string with all characters in upper case.
template <Character TChar>
std::basic_string<TChar> toUpper(std::basic_string<TChar> const &input) noexcept
{
    std::basic_string<TChar> result = input;
    toUpperInplace(result);
    return result;
}

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_STRINGHELPERS_HPP
