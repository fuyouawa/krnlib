﻿#pragma once
#include "krnlib/detail/senior_enum_elem.hpp"
#include <stdexcept>

namespace krnlib {
namespace details {
struct OkTag {};
struct ErrTag {};
}

using Ok = details::SeniorEnumElemValue<details::OkTag>;
using Err = details::SeniorEnumElemValue<details::ErrTag>;

template<class T, class E>
class Result
{
public:
	/**
	 * @brief 接收Ok, 表示函数执行成功
	 * @param ok 内部存着函数想要返回的数据
	*/
	Result(Ok&& ok) : okwp_(std::move(ok)), errwp_() {}
	/**
	 * @brief 接收Ok, 表示有错误
	 * @param err 内部存着错误信息
	*/
	Result(Err&& err) : errwp_(std::move(err)), okwp_() {}
	~Result() {}

	/**
	 * @brief 是否执行成功
	 * @return true为成功, false为出错
	*/
	bool IsOk() const noexcept {
		return okwp_.IsValid();
	}

	/**
	 * @brief 是否出现错误
	 * @return true为出错, false为成功
	*/
	bool IsErr() const noexcept {
		return !IsOk();
	}

	/**
	 * @brief 获取函数想要返回的数据
	 * @return 数据
	*/
	T OkVal() const noexcept {
		return okwp_.GetVal();
	}

	/**
	 * @brief 获取错误信息
	 * @return 错误信息
	*/
	E ErrVal() const noexcept {
		return errwp_.GetVal();
	}

	/**
	 * @brief 如果Ok, 返回Ok中的数据; 如果Err, 调用op参数并传入Err中的数据
	 * @param op 可调用对象, 接收Err的数据
	 * @return Ok中的数据
	*/
	T UnwrapOrElse(const std::function<void(E)>& op) {
		if (IsOk())
			return OkVal();
		else
			op(ErrVal());
		//DEBUG_ASSERT(IsOk());
	}

	/**
	 * @brief 如果Ok, 返回Ok中的数据; 如果Err, 抛出异常
	 * @param msg 异常信息
	 * @return Ok中的数据
	*/
	T Expect(const char* msg) {
		if (IsOk())
			return OkVal();
		else
			std::_Xruntime_error(msg);
	}

	/**
	 * @brief 如果Ok, 返回Ok中的数据; 如果Err, 抛出异常
	 * @return Ok中的数据
	*/
	T Unwrap() {
		if (IsOk())
			return OkVal();
		else
			std::_Xruntime_error("Error occurred!");
	}

private:
	details::SeniorEnumElemWrapper<T, Ok> okwp_;
	details::SeniorEnumElemWrapper<E, Err> errwp_;
};
}