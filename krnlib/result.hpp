#pragma once
#include "krnlib/functional.hpp"
#include "krnlib/stl_container.hpp"

namespace krnlib {
struct Empty{};

template<class T, std::enable_if_t<!std::is_array_v<T>, int> = 0>
class Ok {
public:
	static constexpr bool is_empty_v = std::is_same_v<T, Empty>;
	template<class... ArgsT>
	static constexpr bool va_param_limit = !is_empty_v && !is_pure_first_type_same_v<Ok, ArgsT...>;

	Ok(const Ok&) = delete;
	Ok& operator=(const Ok&) = delete;
	Ok& operator=(Ok&&) = delete;

	Ok(Ok&& right) noexcept(std::is_nothrow_move_constructible_v<T>) : val_(std::move(right.val_)) {}

	template<class... ArgsT, std::enable_if_t<va_param_limit<ArgsT...>, int> = 0>
	Ok(ArgsT&&... args) : val_(std::forward<ArgsT>(args)...) {}

	Ok() : val_() { static_assert(is_empty_v, "To call this parameterless constructor, the type must be Empty!"); }

	T val_;
};

template<class T, std::enable_if_t<!std::is_array_v<T>, int> = 0>
class Err {
public:
	static constexpr bool is_empty_v = std::is_same_v<T, Empty>;
	template<class... ArgsT>
	static constexpr bool va_param_limit = !is_empty_v && !is_pure_first_type_same_v<Err, ArgsT...>;

	Err(const Err&) = delete;
	Err& operator=(const Err&) = delete;
	Err& operator=(Err&&) = delete;

	Err(Err&& right) noexcept(std::is_nothrow_move_constructible_v<T>) : val_(std::move(right.val_)) {}

	template<class... ArgsT, std::enable_if_t<va_param_limit<ArgsT...>, int> = 0>
	Err(ArgsT&&... args) : val_(std::forward<ArgsT>(args)...) {}

	Err() : val_() { static_assert(is_empty_v, "To call this parameterless constructor, the type must be Empty!"); }

	T val_;
};

using OkEmp = Ok<Empty>;
using ErrEmp = Err<Empty>;


template<class T, class E>
class Result
{
public:
	using OkT = Ok<T>;
	using ErrT = Err<E>;

	Result(OkT&& ok) {
		ok_ptr_ = make_unique<OkT>(std::move(ok));
		err_ptr_ = nullptr;
	}
	Result(ErrT&& err) {
		err_ptr_ = make_unique<ErrT>(std::move(err));
		ok_ptr_ = nullptr;
	}
	~Result() {}

	bool Ok() {
		return static_cast<bool>(ok_ptr_);
	}

	bool Err() {
		return static_cast<bool>(err_ptr_);
	}

	std::add_lvalue_reference_t<T> OkVal() {
		static_assert(!OkT::is_empty_v, "The type of 'Ok' is 'Empty', so you cannot retrieve its value!");
		return ok_ptr_->val_;
	}

	std::add_lvalue_reference_t<E> ErrVal() {
		static_assert(!ErrT::is_empty_v, "The type of 'Err' is 'Empty', so you cannot retrieve its value!");
		return err_ptr_->val_;
	}

private:
	krnlib::unique_ptr<OkT> ok_ptr_;
	krnlib::unique_ptr<ErrT> err_ptr_;
};
}