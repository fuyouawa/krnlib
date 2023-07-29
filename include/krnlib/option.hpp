#pragma once
#include "krnlib/functional.hpp"
#include "krnlib/stl_container.hpp"

namespace krnlib {
template<class T, std::enable_if_t<!std::is_array_v<T>, int> = 0>
class Some {
public:
	template<class... ArgsT>
	static constexpr bool va_param_limit = !is_pure_first_type_same_v<Some, ArgsT...>;

	Some(const Some&) = delete;
	Some& operator=(const Some&) = delete;
	Some& operator=(Some&&) = delete;

	Some(Some&& right) noexcept(std::is_nothrow_move_constructible_v<T>) : val_(std::move(right.val_)) {}

	template<class... ArgsT, std::enable_if_t<va_param_limit<ArgsT...>, int> = 0>
	Some(ArgsT&&... args) : val_(std::forward<ArgsT>(args)...) {}

	T val_;
};

struct None {};

template<class T>
class Option
{
public:
	Option(Some<T>&& some) : some_ptr_(krnlib::make_unique<Some<T>>(std::move(some))) {}
	Option(None) : some_ptr_(nullptr) {}
	~Option() {}

	bool IsSome() const noexcept {
		return static_cast<bool>(some_ptr_);
	}

	bool IsNone() const noexcept {
		return !IsSome();
	}

	T& SomeVal() {
		return some_ptr_->val_;
	}

private:
	krnlib::unique_ptr<Some<T>> some_ptr_;
};
}