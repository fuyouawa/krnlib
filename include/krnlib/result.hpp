#pragma once
#include "krnlib/stl_container.hpp"
#include "krnlib/functional.hpp"

namespace krnlib {
struct Empty{};

template<class T, std::enable_if_t<!std::is_array_v<T>, int> = 0>
class Ok {
public:
	template<class... ArgsT>
	static constexpr bool va_param_limit = !is_pure_first_type_same_v<Ok, ArgsT...>;

	Ok(const Ok&) = delete;
	Ok& operator=(const Ok&) = delete;
	Ok& operator=(Ok&&) = delete;

	Ok(Ok&& right) noexcept(std::is_nothrow_move_constructible_v<T>) : val_(std::move(right.val_)) {}

	template<class... ArgsT, std::enable_if_t<va_param_limit<ArgsT...>, int> = 0>
	Ok(ArgsT&&... args) : val_(std::forward<ArgsT>(args)...) {}

	T val_;
};

template<class T, std::enable_if_t<!std::is_array_v<T>, int> = 0>
class Err {
public:
	template<class... ArgsT>
	static constexpr bool va_param_limit = !is_pure_first_type_same_v<Err, ArgsT...>;

	Err(const Err&) = delete;
	Err& operator=(const Err&) = delete;
	Err& operator=(Err&&) = delete;

	Err(Err&& right) noexcept(std::is_nothrow_move_constructible_v<T>) : val_(std::move(right.val_)) {}

	template<class... ArgsT, std::enable_if_t<va_param_limit<ArgsT...>, int> = 0>
	Err(ArgsT&&... args) : val_(std::forward<ArgsT>(args)...) {}

	T val_;
};


template<class T, class E>
class Result
{
public:
	Result(Ok<T>&& ok):
		ok_ptr_(krnlib::make_unique<Ok<T>>(std::move(ok))),
		err_ptr_(nullptr) {}
	Result(Err<E>&& err) :
		err_ptr_(krnlib::make_unique<Err<E>>(std::move(err))),
		ok_ptr_(nullptr) {}
	~Result() {}

	bool IsOk() const noexcept {
		return static_cast<bool>(ok_ptr_);
	}

	bool IsErr() const noexcept {
		return !IsOk();
	}

	T& OkVal() noexcept {
		return ok_ptr_->val_;
	}

	E& ErrVal() noexcept {
		return err_ptr_->val_;
	}

	T& UnwrapOrElse(const krnlib::function<void(E&)>& op) {
		if (IsOk())
			return OkVal();
		else
			op(ErrVal());
	}

	T& Expect(const char* msg) {
		if (IsOk())
			return OkVal();
		else
			std::_Xruntime_error(msg);
	}

	template<std::enable_if_t<krnlib::is_charptr_v<E>, int> = 0>
	T& Unwrap() {
		if (IsOk())
			return OkVal();
		else
			std::_Xruntime_error(ErrVal());
	}

private:
	krnlib::unique_ptr<Ok<T>> ok_ptr_;
	krnlib::unique_ptr<Err<E>> err_ptr_;
};
}