#pragma once
#include "krnlib/detail/string_detail.hpp"

namespace krnlib {
using string = std::basic_string<char, std::char_traits<char>, allocator<char>>;
using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;

template<class T>
_NODISCARD inline krnlib::string to_string(T val) {
    StaticAssertFloatingPoint<T>();
    if constexpr (std::is_unsigned_v<std::decay_t<T>>)
        return details::UIntegralToString<char>(val);
    else
        return details::IntegralToString<char>(val);
}

template<class T>
_NODISCARD inline krnlib::wstring to_wstring(T val) {
    StaticAssertFloatingPoint<T>();
    if constexpr (std::is_unsigned_v<std::decay_t<T>>)
        return details::UIntegralToString<wchar_t>(val);
    else
        return details::IntegralToString<wchar_t>(val);
}

_NODISCARD inline int stoi(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

_NODISCARD inline long stol(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

_NODISCARD inline unsigned long stoul(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

_NODISCARD inline long long stoll(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

_NODISCARD inline unsigned long long stoull(const string& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}



_NODISCARD inline int stoi(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<int>(str, idx, base, "invalid stoi argument", "stoi argument out of range");
}

_NODISCARD inline long stol(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long>(str, idx, base, "invalid stol argument", "stol argument out of range");
}

_NODISCARD inline unsigned long stoul(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long>(str, idx, base, "invalid stoul argument", "stoul argument out of range");
}

_NODISCARD inline long long stoll(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<long long>(str, idx, base, "invalid stoll argument", "stoll argument out of range");
}

_NODISCARD inline unsigned long long stoull(const wstring& str, size_t* idx = nullptr, int base = 10) {
    return details::StrintToIntegral<unsigned long long>(str, idx, base, "invalid stoull argument", "stoull argument out of range");
}
}