#pragma once
#include "krnlib/functional.hpp"

namespace krnlib {

template<class AssignParamT>
class BoolCallable {
public:
	//可调用对象, 调用这个对象获取AssignParamT实例
	using GetValCallableT = krnlib::function<AssignParamT()>;
	//可调用对象, 如果真时调用的对象, 接收一个AssignParamT类型的参数, 会自动赋值
	using TrueCallbackT = krnlib::function<void(AssignParamT)>;
	//可调用对象, 如果假时调用的对象
	using FalseCallbackT = krnlib::function<void()>;

	BoolCallable(bool bval, const GetValCallableT& get_val_call):
		bval_(bval),
		get_val_call_(get_val_call) {}
	~BoolCallable() {}

	// 如果真, 调用传入的callback参数, 并自动传入需要的参数
	BoolCallable& TrueIf(const TrueCallbackT& callback) {
		if (bval_)
			callback(get_val_call_());
		return *this;
	}

	// 如果假, 调用传入的callback参数
	BoolCallable& FalseIf(const FalseCallbackT& callback) {
		if (!bval_)
			callback();
		return *this;
	}

	operator bool() {
		return bval_;
	}

private:
	bool bval_;
	GetValCallableT get_val_call_;
};
}