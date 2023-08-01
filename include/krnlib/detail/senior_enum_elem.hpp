#pragma once
#include "krnlib/functional.hpp"
#include <functional>
#include "krnlib/detail/basic/algorithm.hpp"

namespace krnlib {
namespace details {

template<class Tag>
class SeniorEnumElemValue
{
public:
	SeniorEnumElemValue(const SeniorEnumElemValue&) = delete;
	SeniorEnumElemValue& operator=(const SeniorEnumElemValue&) = delete;
	SeniorEnumElemValue& operator=(SeniorEnumElemValue&&) = delete;

	SeniorEnumElemValue(SeniorEnumElemValue&& right) noexcept {
		ptr_ = right.ptr_;
		type_ = right.type_;
		deleter_ptr_ = std::move(right.deleter_ptr_);
		right.ptr_ = nullptr;
	}

	template<class T, class DecayT=std::decay_t<T>, std::enable_if_t<!std::is_same_v<DecayT, SeniorEnumElemValue>, int> = 0>
	SeniorEnumElemValue(T&& val) {
		if constexpr (std::is_integral_v<DecayT>)
			int_ = static_cast<uint64_t>(val);
		else if constexpr (std::is_pointer_v<DecayT>)
			ptr_ = (void*)val;
		else {
			ptr_ = (void*)new DecayT(std::forward<T>(val));
			deleter_ptr_ = [](void* ptr) {
				delete (DecayT*)ptr;
			};
		}
		type_ = GetDecayValType<T>();
	}

	~SeniorEnumElemValue() {
		switch (type_) {
		case krnlib::kIntegralT:
		case krnlib::kFloatingPointT:
		case krnlib::kPointerNotStrT:
		case krnlib::kCharptrT:
		case krnlib::kWCharptrT:
		case krnlib::kArrayT:
			break;
		default:
			if (ptr_) {
				deleter_ptr_(ptr_);
			}
			break;
		}
		ptr_ = nullptr;
		type_ = ValType::kUnknowT;
	}

	template<class T, class DecayT=std::decay_t<T>>
	T Get() const noexcept {
		if constexpr (std::is_integral_v<DecayT>)
			return static_cast<T>(int_);
		else if constexpr (std::is_pointer_v<DecayT>)
			return static_cast<T>(ptr_);
		else
			return *static_cast<T*>(ptr_);
	}

	template<class T>
	bool IsType() const noexcept {
		return GetDecayValType<T>() == type_;
	}

private:
	ValType type_;
	krnlib::function<void(void*)> deleter_ptr_;
	union{
		void* ptr_;
		uint64_t int_;
		const char* charptr_;
		const wchar_t* wcharptr_;
		char buf_[8];
	};
};

template<class T, class ValueT>
class SeniorEnumElemWrapper
{
public:
	SeniorEnumElemWrapper() : value_ptr_(nullptr) {}
	SeniorEnumElemWrapper(ValueT&& right) :
		value_ptr_(new ValueT(std::move(right))) {
		DEBUG_ASSERT(value_ptr_->IsType<T>());
	}
	~SeniorEnumElemWrapper() {
		delete value_ptr_;
	}

	bool IsValid() const noexcept {
		return value_ptr_ != nullptr;
	}

	T GetVal() const noexcept {
		return value_ptr_->Get<T>();
	}

private:
	ValueT* value_ptr_;
};

}
}