#pragma once
#include "krnlib/type_traits_extension.hpp"
#ifdef WINNT
#include <ntifs.h>
#else
#include <cassert>
#endif // WINNT

#ifdef WINNT
#define KRNLIB_ASSERT(_exp) NT_ASSERT(_exp)
#else
#define KRNLIB_ASSERT(_exp) assert(_exp)
#endif // WINNT

#ifdef WINNT
#define AutoPrint(format, ...) KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, format, __VA_ARGS__))
#else
#define AutoPrint(format, ...) printf(format, __VA_ARGS__)
#endif // WINNT

namespace krnlib {
namespace details {
template<size_t cur_va_idx, class TraverseCallbackT>
inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count) {}

template<size_t cur_va_idx, class TraverseCallbackT, class ThisType, class... RestTypes>
inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count, ThisType&& this_type, RestTypes&&... rest_type) {
	if (cur_va_idx > begin_idx + count)
		return;
	if (cur_va_idx >= begin_idx)
		callback(std::forward<ThisType>(this_type), cur_va_idx);
	TraverseVariable<cur_va_idx + 1, TraverseCallbackT, RestTypes...>(callback, begin_idx, count, std::forward<RestTypes>(rest_type)...);
}

template<class CheckT>
inline constexpr void AssertCheckVariableTypes() { }

template<class CheckT, class ThisType, class... RestTypes>
inline constexpr void AssertCheckVariableTypes() {
	static_assert(std::is_same_v<CheckT, ThisType>, "In the passed argument list, the parameter types are not consistent, and parameters of different types from the specified one have appeared!");
	AssertCheckVariableTypes<CheckT, RestTypes...>();
}

template<class CheckT>
inline constexpr void AssertCheckVariableTypesImplicit() { }

template<class CheckT, class ThisType, class... RestTypes>
inline constexpr void AssertCheckVariableTypesImplicit() {
	static_assert(std::is_convertible_v<ThisType, CheckT>, "In the passed argument list, there are parameters that cannot be implicitly converted to the specified type!");
	AssertCheckVariableTypesImplicit<CheckT, RestTypes...>();
}
}

template<class TraverseCallbackT, class... Types>
inline void TraverseVariable(const TraverseCallbackT& callback, size_t begin_idx, size_t count, Types&&... types)
{
	details::TraverseVariable<0, TraverseCallbackT, Types...>(
		callback,
		begin_idx,
		count,
		std::forward<Types>(types)...);
}

template<class CheckT, class... Types>
inline constexpr void AssertCheckVariableTypes() {
	static_assert(is_all_same_v<CheckT, Types...>, "In the passed argument list, the parameter types are not consistent, and parameters of different types from the specified one have appeared!")
}

template<class CheckT, class... Types>
inline constexpr void AssertCheckVariableTypesImplicit() {
	static_assert(is_all_convertible_v<CheckT, Types...>, "In the passed argument list, there are parameters that cannot be implicitly converted to the specified type!")
}
}