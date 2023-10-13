#pragma once
#ifdef WINNT
#include <ntifs.h>
#else
#include <cassert>
#endif // WINNT
#include <krnfmt/format.h>

#ifndef DebugPrintf
#ifdef WINNT
// 打印信息
#define DebugPrintf(...) KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, __VA_ARGS__))
#else
#if DBG 
// 打印信息
#define DebugPrintf(...) printf(__VA_ARGS__)
#else
#define DebugPrintf(...)
#endif
#endif // WINNT
#endif // !DebugPrintf

namespace krnlib {
template<class... ArgsT>
void Print(fmt::format_string<ArgsT...> fmt, ArgsT&&... args) {
	auto msg = fmt::format(fmt, std::forward<ArgsT>(args)...);
	DebugPrintf(msg.c_str());
}
}