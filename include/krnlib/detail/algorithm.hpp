#pragma once


#ifndef DEBUG_ASSERT
#ifdef WINNT
// 运行时断言
#define DEBUG_ASSERT(_exp) NT_ASSERT(_exp)
#else
// 运行时断言
#define DEBUG_ASSERT(_exp) assert(_exp)
#endif // WINNT
#endif // !DEBUG_ASSERT


#ifndef KRNLIB
#define KRNLIB krnlib::
#define KRNLIB_BEGIN_NAMESPACE namespace krnlib {
#define KRNLIB_END_NAMESPACE }
#endif // !_KRNLIB
