#pragma once
#include <string>
#include "krnlib/memory.hpp"

namespace krnlib {
using string = std::basic_string<char, std::char_traits<char>, allocator<char>>;
using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;

namespace details {
template <class ElemT, class ValueT, class StringT=std::basic_string<ElemT, std::char_traits<ElemT>, krnlib::allocator<ElemT>>>
_NODISCARD StringT IntegralToString(const ValueT val) {
    // convert val to string
    static_assert(std::is_integral_v<ValueT>, "ValueT must be integral");
    using UValueT = std::make_unsigned_t<ValueT>;
    ElemT buf[21]; // can hold -2^63 and 2^64 - 1, plus NUL
    ElemT* const buf_end = std::end(buf);
    ElemT* rnext = buf_end;
    const auto uval = static_cast<UValueT>(val);
    if (val < 0) {
        rnext = std::_UIntegral_to_buff(rnext, 0 - uval);
        *--rnext = '-';
    }
    else {
        rnext = std::_UIntegral_to_buff(rnext, uval);
    }
    return StringT(rnext, buf_end);
}

template <class ElemT, class ValueT, class StringT = std::basic_string<ElemT, std::char_traits<ElemT>, krnlib::allocator<ElemT>>>
_NODISCARD StringT UIntegralToString(const ValueT val) {
    // convert val to string
    static_assert(std::is_integral_v<ValueT>, "ValueT must be integral");
    static_assert(std::is_unsigned_v<ValueT>, "ValueT must be unsigned");
    ElemT buf[21]; // can hold 2^64 - 1, plus NUL
    ElemT* const buf_end = std::end(buf);
    ElemT* const rnext = std::_UIntegral_to_buff(buf_end, val);
    return StringT(rnext, buf_end);
}
}

template<class T>
_NODISCARD inline krnlib::string to_string(T val) {
    using DecayT = std::decay_t<T>;
    StaticAssertFloatingPoint<T>();

    if constexpr (std::is_unsigned_v<DecayT>)
        return details::UIntegralToString<char>(val);
    else
        return details::IntegralToString<char>(val);
}

template<class T>
_NODISCARD inline krnlib::wstring to_wstring(T val) {
    using DecayT = std::decay_t<T>;
    StaticAssertFloatingPoint<T>();

    if constexpr (std::is_unsigned_v<DecayT>)
        return details::UIntegralToString<wchar_t>(val);
    else
        return details::IntegralToString<wchar_t>(val);
}
}