#pragma once
#include "krnlib/detail/algorithm.hpp"
#include <krnfmt/format.h>

KRNLIB_BEGIN_NAMESPACE

template<class... ArgsT>
void Print(fmt::format_string<ArgsT...> fmt, ArgsT&&... args) {
	auto msg = fmt::format(fmt, std::forward<ArgsT>(args)...);
	DebugPrintf(msg.c_str());
}

KRNLIB_END_NAMESPACE