#pragma once
#include <type_traits>

#define KRNLIB_SIMU_ENUM_ELEM(name) \
template <class T> \
class name \
{ \
public: \
	name(const name&) = delete; \
	template<class T1> \
	name(name<T1>&& rvalue) : val_(std::move(rvalue.val_)) { \
		static_assert(std::is_convertible_v<T1, T>, "T1 must be T, or it can be implicitly converted to T!"); \
	} \
	name(T lvalue) : val_(lvalue) { } \
	~name() {} \
	T val_; \
};


namespace krnlib {
namespace details {
}
}