#pragma once
#include <type_traits>
#include <krnlib/detail/algorithm.hpp>

KRNLIB_BEGIN_NAMESPACE

namespace details {
/* ---------------------------------------------------------------------------------------------
* 1: 将可调用对象封装为一个类, 方便操作
* 2: 使用类型擦除, 使代码简洁更明了
--------------------------------------------------------------------------------------------- */
template <class RetT, class... ArgsT>
class __declspec(novtable) CallableObjBase {
public:
	/*
	* 注: 以下说的"自身", 都是子类(实现类)向上转型后的
	* 使用类型擦除将细节隐藏起来, 实际调用时执行的是子类对应的函数
	*/

	// 分配一个新的对象, 获取自身的复制
	virtual CallableObjBase* GetCopy() const = 0;
	// 调用可调用对象
	virtual RetT DoCall(ArgsT&&...) = 0;

	CallableObjBase() = default;
	CallableObjBase(const CallableObjBase&) = delete;
	CallableObjBase& operator=(const CallableObjBase&) = delete;
};

/* ---------------------------------------------------------------------------------------------
* 这个是FuncBase的实现类, 类型擦除所隐藏的部分
--------------------------------------------------------------------------------------------- */
template <class CallableT, class RetT, class... ArgsT>
class CallableObjImpl : public CallableObjBase<RetT, ArgsT...> {
public:
	using InheritedT = CallableObjBase<RetT, ArgsT...>;

	/*
	* 自动根据FuncTTemp的类型执行赋值或者右值引用
	* 使用enable_if_t确保FuncTTemp不是自身的类型, 否则会陷入无限递归
	*/
	template<class NewCallableT, std::enable_if_t<!std::is_same_v<std::decay_t<CallableObjImpl>, std::decay_t<NewCallableT>>, int> = 0>
	CallableObjImpl(NewCallableT&& new_call) : call_(std::forward<NewCallableT>(new_call)) {}

private:
	InheritedT* GetCopy() const override {
		/*
		* 先分配一块自身大小的内存
		* 然后对内存执行构造函数, 拷贝内部的可调用对象
		* 最后返回父类进行类型擦除
		*/
		return new CallableObjImpl(call_);
	}

	RetT DoCall(ArgsT&&... args) override {
		return std::invoke(call_, std::forward<ArgsT>(args)...);
	}

	CallableT call_;
};


/* ---------------------------------------------------------------------------------------------
* 此类的作用是
* 1. 将可调用对象的操作再封装一层
* 2. 使得最外层的类只需要提供一个模板, 自动将模板中的返回值类型和参数列表提取出来
--------------------------------------------------------------------------------------------- */
template <class RetT, class... ArgsT>
class FuncBaseImpl {
public:
	using CallableObj = CallableObjBase<RetT, ArgsT...>;

	FuncBaseImpl() : callable_obj_ptr_(nullptr) {}
	~FuncBaseImpl() { Tidy(); }

	// 调用内部的可调用对象
	RetT operator()(ArgsT... args) const {
		return callable_obj_ptr_->DoCall(std::forward<ArgsT>(args)...);
	}

protected:
	// 判断是否是可调用对象, 并且传入的NewCallableT和声明的调用方式相同
	template <class NewCallableT, class function>
	using EnableIfAllowableCallT = std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::_Remove_cvref_t<NewCallableT>, function>>,
		std::_Is_invocable_r<RetT, std::decay_t<NewCallableT>, ArgsT...>>,
		int>;
	/*
	* EnableIfAllowableCallT原理:
	* 1: std::negation<std::is_same<std::_Remove_cvref_t<CallableT>, function>>
	*	解析: 首先通过std::_Remove_cvref_t<CallableT>去除CallableT的一些干扰属性然后使用std::is_same与function判断, 最后使用std::negation将结果取反
	*	作用: 防止CallableT与最外层的function相等, 导致拷贝构造函数时发生无限递归
	*
	* 2: std::_Is_invocable_r<RetT, std::decay_t<CallableT>&, ArgsT...>>
	*	解析: 首先通过std::decay_t<CallableT>去除CallableT干扰的属性, 然后使用std::_Is_invocable_r检测可调用对象是否可以按照传入的返回值和参数列表进行调用
	*	作用: 检测给定的CallableT是否可以按照预期调用
	*
	* 3: 最后通过std::conjunction_v将 1 & 2
	*/


	// 重置为参数的复制
	void ResetCopy(const FuncBaseImpl& right) {
		if (!right.Empty())
			callable_obj_ptr_ = right.callable_obj_ptr_->GetCopy();
	}

	// 重置为参数的引用
	void ResetMove(FuncBaseImpl&& right) noexcept {
		if (!right.Empty()) {
			callable_obj_ptr_ = right.callable_obj_ptr_;
			right.callable_obj_ptr_ = nullptr;
		}
	}

	// 重置内部的可调用对象
	template <class CallableT>
	void AutoReset(CallableT&& call) {
		// 获取CallableObjBase的实现类
		using ImplT = CallableObjImpl<std::decay_t<CallableT>, RetT, ArgsT...>;
		// 分配内存, 并进行向上转型(类型擦除)
		callable_obj_ptr_ = (CallableObj*)new ImplT(std::forward<CallableT>(call));
	}

	bool Empty() const noexcept {
		return callable_obj_ptr_ == nullptr;
	}

	// 交换
	void Swap(FuncBaseImpl& right) noexcept {
		CallableObj* temp = callable_obj_ptr_;
		callable_obj_ptr_ = right.callable_obj_ptr_;
		right.callable_obj_ptr_ = temp;
	}

private:
	// 释放内存
	void Tidy() {
		if (callable_obj_ptr_) {
			delete callable_obj_ptr_;
			callable_obj_ptr_ = nullptr;
		}
	}

	CallableObj* callable_obj_ptr_;
};

/*
* 使用SFINAE, 根据T的类型选择对应的实现路径
* 如果无法转换成RetT(ArgsT...), 则会触发编译期错误
*/
template <class T>
struct GetFunctionImpl {
	static_assert(std::_Always_false<T>, "function only accepts function types as template arguments.");
};

template <class RetT, class... ArgsT>
struct GetFunctionImpl<RetT(ArgsT...)> {
	using Type = FuncBaseImpl<RetT, ArgsT...>;
};
}


template<class CallableT>
class function : public details::GetFunctionImpl<CallableT>::Type {
public:
	using InheritedT = typename details::GetFunctionImpl<CallableT>::Type;

	function() noexcept {}
	function(nullptr_t) noexcept {}
	function(const function& right) {
		this->ResetCopy(right);
	}
	function(function&& right) noexcept {
		this->ResetMove(std::move(right));
	}

	/*
	* 注: 必须得使用NewCallableT, 而不能使用已有的CallableT, 这是因为声明的CallableT多少都会和实际情况有所差异.
	* 当然具体的NewCallableT是否能够等同于CallableT来调用, 由子类内部的EnableIfAllowableCallT进行判断
	*/
	// 为可调用对象分配内存
	template <class NewCallableT, typename InheritedT::template EnableIfAllowableCallT<NewCallableT, function> = 0>
	function(NewCallableT&& new_call) {
		this->AutoReset(std::forward<NewCallableT>(new_call));
	}

	function& operator=(const function& right) {
		/*
		* 调用拷贝构造函数, 分配一个临时变量, 将里面的内容和自身交换
		* 这样做的好处是:
		* 1. 可以调用已有的拷贝构造函数, 拷贝数据, 减少重复代码
		* 2. 使用Swap将原有的数据交换入拷贝构造函数里, 作用域离开后就会自动调用析构函数, 释放原有的数据
		*/
		function(right).Swap(*this);
		return *this;
	}

	function& operator=(function&& right) noexcept {
		if (this != std::addressof(right)) {
			function(std::move(right)).Swap(*this);
		}
		return *this;
	}

	// 为可调用对象赋值操作
	template <class NewCallableT, typename InheritedT::template EnableIfAllowableCallT<NewCallableT, function> = 0>
	function& operator=(NewCallableT&& new_call) {
		function(std::forward<NewCallableT>(new_call)).Swap(*this);
		return *this;
	}
	~function() {}
};

KRNLIB_END_NAMESPACE