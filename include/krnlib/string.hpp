#pragma once
#include "krnfmt/format.h"

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