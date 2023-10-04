#pragma once
#ifdef WINNT
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>
#endif // WINNT
#include <string>
#include <string_view>
#include <tuple>
#include <limits>
#include "krnlib/detail/allocator.hpp"

KRNLIB_BEGIN_NAMESPACE

template <class ElemT, class TraitsT = std::char_traits<ElemT>, class AllocT = allocator<ElemT>>
using basic_string = std::basic_string<ElemT, TraitsT, AllocT>;

namespace details {
template <class ElemT, class ValueT>
_NODISCARD basic_string<ElemT> IntegralToString(const ValueT val) {
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
    return basic_string<ElemT>(rnext, buf_end);
}

template <class ElemT, class ValueT>
_NODISCARD basic_string<ElemT> UIntegralToString(const ValueT val) {
    // convert val to string
    static_assert(std::is_integral_v<ValueT>, "ValueT must be integral");
    static_assert(std::is_unsigned_v<ValueT>, "ValueT must be unsigned");
    ElemT buf[21]; // can hold 2^64 - 1, plus NUL
    ElemT* const buf_end = std::end(buf);
    ElemT* const rnext = std::_UIntegral_to_buff(buf_end, val);
    return basic_string<ElemT>(rnext, buf_end);
}

template<class ElemT>
bool BasicCheckStringCanToNumber(const ElemT* next_str, const ElemT* const end_str, int base) {
    for (; next_str != end_str && isspace(*next_str); next_str++);
    if (
        next_str == end_str ||
        !isdigit(*next_str) ||
        (base <= 1 || (base > 10 && base != 16))
        )
        return false;
    return true;
}


template<class RetT, class ElemT>
_NODISCARD auto StrPtrToIntegral(const ElemT*& next_str, const ElemT* const end_str, int base) {
    constexpr bool is_wchar_elem = std::is_same_v<ElemT, wchar_t>;
    RetT res = 0;
    bool has_invalid_argument_err = false;
    bool has_out_of_range_err = false;

    for (; next_str != end_str; next_str++) {
        int digit;
        ElemT c = *next_str;
        if (isdigit(c)) {
            if constexpr (is_wchar_elem)
                digit = c - L'0';
            else
                digit = c - '0';
        }
        else if (base == 16 && isxdigit(c)) {
            if constexpr (is_wchar_elem)
                digit = toupper(c) - L'A' + 10;
            else
                digit = toupper(c) - 'A' + 10;
        }
        else break;
        if (digit >= base) {
            has_invalid_argument_err = true;
            break;
        }
        if (res > ((std::numeric_limits<RetT>::max)() - digit) / base) {
            has_out_of_range_err = true;
            break;
        }
        res = res * base + digit;
    }
    return std::make_tuple(res, has_invalid_argument_err, has_out_of_range_err);
}

template<class ElemT>
_NODISCARD bool GetNegative(ElemT ch) {
    constexpr bool is_wchar_elem = std::is_same_v<ElemT, wchar_t>;
    bool negative = false;

    if constexpr (is_wchar_elem) {
        if (ch == L'-' || ch == L'+') {
            negative = (ch == L'-');
        }
    }
    else {
        if (ch == '-' || ch == '+') {
            negative = (ch == '-');
        }
    }
    return negative;
}

template<class RetT, class ElemT>
_NODISCARD RetT StrintToIntegral(const basic_string<ElemT>& str, size_t* idx, int base, const char* invalid_argument_err, const char* out_of_range_err) {
    static_assert(std::is_integral_v<RetT>, "RetT must be integral");
    constexpr bool is_wchar_elem = std::is_same_v<ElemT, wchar_t>;
    constexpr bool is_unsigned_ret = std::is_unsigned_v<RetT>;

    const ElemT* const beg_str = str.c_str();
    const ElemT* const end_str = beg_str + str.size();
    const ElemT* next_str = beg_str;
    
    if (!BasicCheckStringCanToNumber(next_str, end_str, base))
        std::_Xinvalid_argument(invalid_argument_err);

    bool negative = GetNegative(*next_str);
    if (negative) {
        if constexpr (is_unsigned_ret)
            std::_Xinvalid_argument(invalid_argument_err);
        ++next_str;
    }

    auto [res, has_invalid_argument_err, has_out_of_range_err] = StrPtrToIntegral<RetT>(next_str, end_str, base);
    if (has_invalid_argument_err)
        std::_Xinvalid_argument(invalid_argument_err);
    if (has_out_of_range_err)
        std::_Xout_of_range(out_of_range_err);

    if (idx) {
        *idx = next_str - beg_str;
    }

    //for (; next_str != end_str && isspace(*next_str); next_str++);
    //if (next_str != end_str)
    //    std::_Xinvalid_argument(invalid_argument_err);

    if constexpr (is_unsigned_ret)
        return res;
    else
        return negative ? -res : res;
}
}

KRNLIB_END_NAMESPACE