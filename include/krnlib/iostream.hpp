#pragma once
#include "krnlib/detail/algorithm.hpp"
#include <krnfmt/format.h>

namespace krnlib {
	template<class... ArgsT>
	void Print(fmt::format_string<ArgsT...> fmt, ArgsT&&... args) {
		auto msg = fmt::format(fmt, std::forward<ArgsT>(args)...);
		DebugPrintf(msg.c_str());
	}
}