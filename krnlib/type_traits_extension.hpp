#pragma once
#include <type_traits>

namespace krnlib {
namespace details {
#pragma region 可变模板参数的判断是否相等
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

template<class CompareT, class ThisType, class... RestTypes>
static constexpr bool is_only_one_type_and_same_v() {
	return std::is_same_v<CompareT, ThisType> && sizeof...(RestTypes) == 0;
}
template<class CompareT>
static constexpr bool is_only_one_type_and_same_v() {
	return false;
}
#pragma endregion

#pragma region 可变模板参数的判断是否可以转换
template<class ToT, class ThisFromT, class... RestFromT>
static constexpr bool is_first_type_convertible_v() {
	return std::is_convertible_v<ThisFromT, ToT>;
}
template<class ToT>
static constexpr bool is_first_type_convertible_v() {
	return false;
}

template<class ToT, class ThisFromT, class... RestFromT>
static constexpr bool is_only_one_type_and_is_convertible_v_v() {
	return std::is_convertible_v<ThisFromT, ToT> && sizeof...(RestFromT) == 0;
}
template<class ToT>
static constexpr bool is_only_one_type_and_is_convertible_v_v() {
	return false;
}
#pragma endregion
}

// 判断可变模板参数的第一个参数是否和指定参数相等
template<class CompareT, class... Types>
static constexpr bool is_first_type_same_v = details::is_first_type_same_v<CompareT, Types...>();
// 判断可变模板参数的第一个参数(去除装饰后)是否和指定参数相等
template<class CompareT, class... Types>
static constexpr bool is_pure_first_type_same_v = details::is_pure_first_type_same_v<CompareT, Types...>();
// 判断可变模板参数是否只有一个参数并且是否和指定类型相等
template<class CompareT, class... Types>
static constexpr bool is_only_one_type_and_same_v = details::is_only_one_type_and_same_v<CompareT, Types...>();
// 判断可变模板参数的第一个参数是否可以转换为指定参数
template<class ToT, class... FromT>
static constexpr bool is_first_type_convertible_v = details::is_first_type_convertible_v<ToT, FromT...>();
// 判断可变模板参数是否只有一个参数并且可以转换为指定参数
template<class ToT, class... FromT>
static constexpr bool is_only_one_type_and_is_convertible_v_v = details::is_only_one_type_and_is_convertible_v_v<ToT, FromT...>();
// 判断可变模板参数所有参数是否和指定参数都相等
template<class CompareT, class... Types>
static constexpr bool is_all_same_v = ((std::is_same_v<CompareT, Types>) && ...);
// 判断可变模板参数所有参数是否都可以转换成指定参数
template<class ToT, class... FromT>
static constexpr bool is_all_convertible_v = ((std::is_convertible_v<FromT, ToT>) &&  ...);

// 是否是数字(包括整数, 小数)
template<class T>
static constexpr bool is_number_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
// 是否是字符串
template<class T>
static constexpr bool is_charptr_v = std::_Is_any_of_v<T, const char*, char*, char[]>;
// 是否是宽字符串
template<class T>
static constexpr bool is_wcharptr_v = std::_Is_any_of_v<T, const wchar_t*, wchar_t*, wchar_t[]>;
// 是否是字符串或者宽字符串
template<class T>
static constexpr bool is_str_v = is_charptr_v<T> || is_wcharptr_v<T>;
// 是否是结构体
template<class T>
static constexpr bool is_struct_v = std::is_pod_v<T> && !is_number_v<T> && !std::is_pointer_v<T>;
// 是否是类
template<class T>
static constexpr bool is_class_v = !std::is_pod_v<T>;
// 是否是对象(结构体+类)
template<class T>
static constexpr bool is_object_v = is_struct_v<T> || is_class_v<T>;
// 是否是非字符串的指针
template<class T>
static constexpr bool is_not_str_ptr_v = std::is_pointer_v<T> && !is_str_v<T>;
}