#pragma once
#include "krnlib/detail/string_details.hpp"
#include "krnfmt/format.h"

KRNLIB_BEGIN_NAMESPACE

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

/**
 * @brief 将整数转换为字符串
 * @tparam T 整数类型
 * @param val 要转换的数值
*/
template<class T>
_NODISCARD inline string to_string(T val) {
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
template<class T>
_NODISCARD inline wstring to_wstring(T val) {
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
_NODISCARD inline int stoi(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

/**
 * @brief 将字符串转换为long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long stol(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

/**
 * @brief 将字符串转换为int类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long stoul(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

/**
 * @brief 将字符串转换为long long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long long stoll(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

/**
 * @brief 将字符串转换为unsigned long long类型数值
 * @param str 要转换的字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long long stoull(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}



/**
 * @brief 将宽字符串转换为int类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline int stoi(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

/**
 * @brief 将宽字符串转换为long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long stol(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

/**
 * @brief 将宽字符串转换为unsigned long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long stoul(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

/**
 * @brief 将宽字符串转换为long long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline long long stoll(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

/**
 * @brief 将宽字符串转换为unsigned long long类型数值
 * @param str 要转换的宽字符串
 * @param idx 可选参数，指向转换结束的位置
 * @param base 可选参数，数值的进制
*/
_NODISCARD inline unsigned long long stoull(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}

inline string UnicodeStringToStlString(PCUNICODE_STRING ustr) {
    ANSI_STRING astr;
    RtlUnicodeStringToAnsiString(&astr, ustr, TRUE);
    string res{astr.Buffer, astr.Length};
    RtlFreeAnsiString(&astr);
    return res;
}

KRNLIB_END_NAMESPACE

template<>
struct fmt::formatter<UNICODE_STRING> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const UNICODE_STRING& str, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{}", KRNLIB UnicodeStringToStlString(&str));
    }
};