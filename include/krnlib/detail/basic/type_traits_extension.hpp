#pragma once
#include <type_traits>

#define ALL_INTEGRAL_TYPES bool, char, signed char, unsigned char, wchar_t, char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long

#define ALL_FLOATING_POINT_TYPES float, double, long double

namespace krnlib {
namespace details {
//template<class CompareT, class ThisType, class... RestTypes>
//static constexpr bool is_first_type_same() {
//	return std::is_same_v<CompareT, ThisType>;
//}
//template<class CompareT>
//static constexpr bool is_first_type_same() {
//	return false;
//}
//template<class CompareT, class ThisType, class... RestTypes>
//static constexpr bool is_first_type_same() {
//	return std::is_same_v<CompareT, ThisType>;
//}
//template<class CompareT, class ThisType, class... RestTypes>
//static constexpr bool is_decay_first_type_same() {
//	return std::is_same_v<CompareT, std::decay_t<ThisType>>;
//}
//template<class CompareT>
//static constexpr bool is_decay_first_type_same() {
//	return false;
//}
//
//template<class CompareT, class ThisType, class... RestTypes>
//static constexpr bool is_only_one_type_and_same() {
//	return std::is_same_v<CompareT, ThisType> && sizeof...(RestTypes) == 0;
//}
//template<class CompareT>
//static constexpr bool is_only_one_type_and_same() {
//	return false;
//}
//
//template<class ToT, class ThisFromT, class... RestFromT>
//static constexpr bool is_first_type_convertible() {
//	return std::is_convertible_v<ThisFromT, ToT>;
//}
//template<class ToT>
//static constexpr bool is_first_type_convertible() {
//	return false;
//}
//
//template<class ToT, class ThisFromT, class... RestFromT>
//static constexpr bool is_only_one_type_and_is_convertible() {
//	return std::is_convertible_v<ThisFromT, ToT> && sizeof...(RestFromT) == 0;
//}
//template<class ToT>
//static constexpr bool is_only_one_type_and_is_convertible() {
//	return false;
//}

template<class T, class U>
static constexpr bool is_decay_same_v = std::is_same_v<std::decay_t<T>, std::decay_t<U>>;
template<class T, class U>
static constexpr bool is_decay_convertible_v = std::is_convertible_v<std::decay_t<T>, std::decay_t<U>>;
}

//template<class CompareT, class... Types>
//static constexpr bool is_first_type_same_v = details::is_first_type_same<CompareT, Types...>();
//template<class CompareT, class... Types>
//static constexpr bool is_decay_first_type_same_v = details::is_decay_first_type_same<CompareT, Types...>();
//template<class CompareT, class... Types>
//static constexpr bool is_only_one_type_and_same_v = details::is_only_one_type_and_same<CompareT, Types...>();
//template<class ToT, class... FromT>
//static constexpr bool is_first_type_convertible_v = details::is_first_type_convertible<ToT, FromT...>();
//template<class ToT, class... FromT>
//static constexpr bool is_only_one_type_and_is_convertible_v = details::is_only_one_type_and_is_convertible<ToT, FromT...>();

template<class T>
inline constexpr bool is_number_v = std::is_integral_v<T> || std::is_floating_point_v<T>;
template<class T>
inline constexpr bool is_charptr_v = std::_Is_any_of_v<T, const char*, char*, char[], const char[]>;
template<class T>
inline constexpr bool is_wcharptr_v = std::_Is_any_of_v<T, const wchar_t*, wchar_t*, wchar_t[], const wchar_t[]>;
template<class T>
inline constexpr bool is_str_v = is_charptr_v<T> || is_wcharptr_v<T>;
template<class T>
inline constexpr bool is_nullptr_v = std::is_same_v<T, std::nullptr_t>;
template<class T>
inline constexpr bool is_struct_v = std::is_pod_v<T> && !is_number_v<T> && !std::is_pointer_v<T> && !std::is_array_v<T> && !is_nullptr_v<T>;
template<class T>
inline constexpr bool is_class_v = !std::is_pod_v<T>;
template<class T>
inline constexpr bool is_object_v = is_struct_v<T> || is_class_v<T>;
template<class T>
inline constexpr bool is_pointer_v = std::is_pointer_v<T>;
template<class T>
inline constexpr bool is_not_str_ptr_v = is_pointer_v<T> && !is_str_v<T>;

template<class CompareT, class... Types>
inline constexpr bool is_all_same_v = (sizeof...(Types) != 0 && (... && std::is_same_v<CompareT, Types>));
template<class ToT, class... FromTypes>
inline constexpr bool is_all_convertible_v = (sizeof...(FromTypes) != 0 && (... && std::is_convertible_v<FromTypes, ToT>));
template<class CompareT, class... Types>
inline constexpr bool is_any_of_v = (sizeof...(Types) != 0 && (... || std::is_same_v<CompareT, Types>));
template<class ToT, class... FromTypes>
inline constexpr bool is_any_of_convertible_v = (sizeof...(FromTypes) != 0 && (... || std::is_convertible_v<FromTypes, ToT>));

template<class CompareT, class... Types>
inline constexpr bool is_decay_all_same_v = (sizeof...(Types) != 0 && (... && details::is_decay_same_v<CompareT, Types>));
template<class ToT, class... FromTypes>
inline constexpr bool is_decay_all_convertible_v = (sizeof...(FromTypes) != 0 && (... && details::is_decay_convertible_v<FromTypes, ToT>));
template<class CompareT, class... Types>
inline constexpr bool is_decay_any_of_v = (sizeof...(Types) != 0 && (... || details::is_decay_same_v<CompareT, Types>));
template<class ToT, class... FromTypes>
inline constexpr bool is_decay_any_of_convertible_v = (sizeof...(FromTypes) != 0 && (... || details::is_decay_convertible_v<FromTypes, ToT>));

enum ValType
{
	kIntegralT,
	kFloatingPointT,
	kPointerNotStrT,
	kCharptrT,
	kWCharptrT,
	kArrayT,
	kStructT,
	kClassT,
	kUnknowT
};

template<class T>
static constexpr ValType GetValType() {
	if constexpr (std::is_integral_v<T>) {
		return ValType::kIntegralT;
	}
	else if constexpr (std::is_floating_point_v<T>) {
		return ValType::kFloatingPointT;
	}
	else if constexpr (is_not_str_ptr_v<T>) {
		return ValType::kPointerNotStrT;
	}
	else if constexpr (std::is_array_v<T>) {
		return ValType::kArrayT;
	}
	else if constexpr (is_charptr_v<T>) {
		return ValType::kCharptrT;
	}
	else if constexpr (is_wcharptr_v<T>) {
		return ValType::kWCharptrT;
	}
	else if constexpr (is_class_v<T>) {
		return ValType::kClassT;
	}
	else if constexpr (is_struct_v<T>) {
		return ValType::kStructT;
	}
	else {
		static_assert(std::_Always_false<T>, "Unknow type?");
		return ValType::kUnknowT;
	}
}

template<class T>
static constexpr ValType GetDecayValType() {
	return GetValType<std::decay_t<T>>();
}
}