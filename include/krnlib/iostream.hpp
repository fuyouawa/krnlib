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

#if defined(DBG) || defined(_DEBUG)
#define KRNLIB_DEBUG
#elif defined(KRNLIB_DEBUG)
#undef KRNLIB_DEBUG
#endif


namespace krnlib {
template<typename... ArgsType>
void Print(std::string_view fmt, ArgsType&&... args) {
	auto msg = KRNLIB_FMT vformat(fmt, KRNLIB_FMT make_format_args(args...));
#ifdef WINNT
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, msg.c_str());
#else
	printf(msg.c_str());
#endif // WINNT
}

template<typename... ArgsType>
void DebugPrint(std::string_view fmt, ArgsType&&... args) {
#ifdef KRNLIB_DEBUG
	Print(fmt, std::forward<ArgsType>(args)...);
#endif // KRNLIB_DEBUG
}
}