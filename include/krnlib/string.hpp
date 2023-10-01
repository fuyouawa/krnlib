#pragma once
#include "krnlib/detail/string_detail.hpp"
#include <fustd/generic/type_traits.hpp>

namespace krnlib {
using string = basic_string<char>;
using wstring = basic_string<wchar_t>;



/**
 * @brief 将整数转换为字符串
 * @tparam T 整数类型
 * @param val 要转换的数值
*/
template<fustd::integeral_t T>
_NODISCARD inline krnlib::string to_string(T val) {
    if constexpr (std::is_unsigned_v<std::decay_t<T>>)
        return details::UIntegralToString<char>(val);
    else
        return details::IntegralToString<char>(val);
}

/**
 * @brief 将整数转换为宽字符串
 * @tparam T 整数类型
 * @param val 要转换的数值
*/
template<fustd::integeral_t T>
_NODISCARD inline krnlib::wstring to_wstring(T val) {
    if constexpr (std::is_unsigned_v<std::decay_t<T>>)
        return details::UIntegralToString<wchar_t>(val);
    else
        return details::IntegralToString<wchar_t>(val);
}



/**
 * @brief 将字符串转换为int类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline int stoi(const krnlib::string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

/**
 * @brief 将字符串转换为long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long stol(const krnlib::string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

/**
 * @brief 将字符串转换为int类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long stoul(const krnlib::string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

/**
 * @brief 将字符串转换为long long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long long stoll(const krnlib::string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

/**
 * @brief 将字符串转换为unsigned long long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long long stoull(const krnlib::string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}



/**
 * @brief 将宽字符串转换为int类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline int stoi(const krnlib::wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

/**
 * @brief 将宽字符串转换为long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long stol(const krnlib::wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

/**
 * @brief 将宽字符串转换为unsigned long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long stoul(const krnlib::wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

/**
 * @brief 将宽字符串转换为long long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long long stoll(const krnlib::wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

/**
 * @brief 将宽字符串转换为unsigned long long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long long stoull(const krnlib::wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}
}