#pragma once
#include "krnlib/detail/senior_enum_elem.hpp"

namespace krnlib {
namespace details {
struct SomeTag {};
}

using Some = details::SeniorEnumElemValue<details::SomeTag>;
struct None {};

/**
 * @brief 抽象可空类型的理念, Option表示可空类型, Some表示有值, None表示空
 * @tparam T Some的数据类型
*/
template<class T>
class Option
{
public:
	/**
	 * @brief 接收Some, 表示有值
	*/
	Option(Some&& some) : somewp_(std::move(some)) {}
	/**
	 * @brief 接收None, 表示空
	*/
	Option(None) : somewp_() {}
	~Option() {}

	/**
	 * @brief 是否有值
	*/
	bool IsSome() const noexcept {
		return somewp_.IsValid();
	}

	/**
	 * @brief 是否为空
	*/
	bool IsNone() const noexcept {
		return !IsSome();
	}

	/**
	 * @brief 获取值
	*/
	T SomeVal() noexcept {
		return somewp_.GetVal();
	}

private:
	details::SeniorEnumElemWrapper<T, Some> somewp_;
};
}