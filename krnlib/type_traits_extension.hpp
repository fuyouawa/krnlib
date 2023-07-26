#pragma once
#include <type_traits>

namespace krnlib {
namespace details {
template<class CompareT, class ThisType, class... RestTypes>
static constexpr bool is_first_type_same_v() {
	return std::is_same_v<CompareT, ThisType>;
}
template<class CompareT>
static constexpr bool is_first_type_same_v() {
	return false;
}
template<class CompareT, class ThisType, class... RestTypes>
static constexpr bool is_pure_first_type_same_v() {
	return std::is_same_v<CompareT, std::_Remove_cvref_t<ThisType>>;
}
template<class CompareT>
static constexpr bool is_pure_first_type_same_v() {
	return false;
}
}

template<class CompareT, class... Types>
static constexpr bool is_first_type_same_v = details::is_first_type_same_v<CompareT, Types...>();
template<class CompareT, class... Types>
static constexpr bool is_pure_first_type_same_v = details::is_pure_first_type_same_v<CompareT, Types...>();
template<class CompareT, class... Types>
static constexpr bool is_all_same_v = ((std::is_same_v<CompareT, Types>) && ... );
template<class CompareT, class... Types>
static constexpr bool is_all_convertible_v = ((std::is_convertible_v<CompareT, Types>) &&  ... );

template<class T>
static constexpr bool is_number_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
template<class T>
static constexpr bool is_charptr_v = std::_Is_any_of_v<T, const char*, char*, char[]>;
template<class T>
static constexpr bool is_wcharptr_v = std::_Is_any_of_v<T, const wchar_t*, wchar_t*, wchar_t[]>;
template<class T>
static constexpr bool is_str_v = is_charptr_v<T> || is_wcharptr_v<T>;
template<class T>
static constexpr bool is_struct_v = std::is_pod_v<T> && !is_number_v<T> && !std::is_pointer_v<T>;
template<class T>
static constexpr bool is_class_v = !std::is_pod_v<T>;
template<class T>
static constexpr bool is_object_v = is_struct_v<T> || is_class_v<T>;
template<class T>
static constexpr bool is_not_str_ptr_v = std::is_pointer_v<T> && !is_str_v<T>;

template<class T>
using remove_vref_t = std::remove_volatile_t<std::remove_reference_t<T>>;
}