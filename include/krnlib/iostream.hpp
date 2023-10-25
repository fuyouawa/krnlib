#pragma once
#ifdef WINNT
#include <ntifs.h>
#include <krnfmt/format.h>
#else
#include <cassert>
#include <format>
#endif // WINNT

#ifndef KRNLIB_FMT
#ifdef WINNT
#define KRNLIB_FMT fmt::
#else
#define KRNLIB_FMT std::
#endif // WINNT
#endif // !KRNLIB_FMT


namespace krnlib {
template<class... ArgsType>
void Print(std::string_view fmt, ArgsType&&... args) {
	auto msg = KRNLIB_FMT vformat(fmt, KRNLIB_FMT make_format_args(std::forward<ArgsType>(args)...));
#ifdef WINNT
	DbgPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, msg.c_str()));
#else
	printf(msg.c_str());
#endif // WINNT
}

template<class... ArgsType>
void DebugPrint(std::string_view fmt, ArgsType&&... args) {
#ifdef DBG
	Print(fmt, std::forward<ArgsType>(args)...);
#endif // DBG
}
}