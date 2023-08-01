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
 * @tparam T 数值类型
*/
template<class T>
class Option
{
public:
	/**
	 * @brief 接收Some, 表示有值
	 * @param some 内部存着函数想要返回的数据
	*/
	Option(Some&& some) : somewp_(std::move(some)) {}
	/**
	 * @brief 接收None, 表示空
	*/
	Option(None) : somewp_() {}
	~Option() {}

	/**
	 * @brief 是否有值
	 * @return true有值, false为空
	*/
	bool IsSome() const noexcept {
		return somewp_.IsValid();
	}

	/**
	 * @brief 是否为空
	 * @return true为空, false有值
	*/
	bool IsNone() const noexcept {
		return !IsSome();
	}

	/**
	 * @brief 获取值
	 * @return 值
	*/
	T SomeVal() noexcept {
		return somewp_.GetVal();
	}

private:
	details::SeniorEnumElemWrapper<T, Some> somewp_;
};
}