#pragma once
#ifdef WINNT
#include "krnfmt/format.h"

template<>
struct fmt::formatter<UNICODE_STRING> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const UNICODE_STRING& str, FormatContext& ctx) {
        ANSI_STRING astr;
        RtlUnicodeStringToAnsiString(&astr, &str, TRUE);
        std::string stlstr{astr.Buffer, astr.Length};
        RtlFreeAnsiString(&astr);
        return fmt::format_to(ctx.out(), "{}", stlstr);
    }
};
#endif // WINNT
