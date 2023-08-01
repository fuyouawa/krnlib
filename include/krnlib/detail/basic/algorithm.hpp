#pragma once
#include "krnlib/detail/basic/type_traits_extension.hpp"
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

//namespace krnlib {
//namespace details {
//template<size_t cur_va_idx, class TraverseCallbackT>
//inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count) {}
//
//template<size_t cur_va_idx, class TraverseCallbackT, class ThisType, class... RestTypes>
//inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count, ThisType&& this_type, RestTypes&&... rest_type) {
//	if (cur_va_idx > begin_idx + count)
//		return;
//	if (cur_va_idx >= begin_idx)
//		callback(std::forward<ThisType>(this_type), cur_va_idx);
//	TraverseVariable<cur_va_idx + 1, TraverseCallbackT, RestTypes...>(callback, begin_idx, count, std::forward<RestTypes>(rest_type)...);
//}
//}
//
//template<class TraverseCallbackT, class... Types>
//inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count, Types&&... types)
//{
//	details::TraverseVariable<0, TraverseCallbackT, Types...>(
//		callback,
//		begin_idx,
//		count,
//		std::forward<Types>(types)...);
//}
//
//template<class CheckT, class... Types>
//inline constexpr void AssertCheckVariableTypesSame() {
//	static_assert(is_all_same_v<CheckT, Types...>, "In the passed argument list, the parameter types are not consistent, and parameters of different types from the specified one have appeared!");
//}
//
//template<class CheckT, class... Types>
//inline constexpr void AssertCheckVariableTypesConvertible() {
//	static_assert(is_all_convertible_v<CheckT, Types...>, "In the passed argument list, there are parameters that cannot be implicitly converted to the specified type!");
//}
//}