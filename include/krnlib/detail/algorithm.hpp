#pragma once
#ifdef WINNT
#include <ntifs.h>
#else
#include <cassert>
#endif // WINNT

#ifdef WINNT
// 运行时断言
#define DEBUG_ASSERT(_exp) NT_ASSERT(_exp)
#else
// 运行时断言
#define DEBUG_ASSERT(_exp) assert(_exp)
#endif // WINNT

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