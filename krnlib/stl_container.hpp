#pragma once
#include <list>
#include <vector>
#include <string>
#include "krnlib/memory.hpp"

namespace krnlib {
template<class T>
using list = std::list<T, krnlib::default_allocator<T>>;
template<class T>
using vector = std::vector<T, krnlib::default_allocator<T>>;
using string = std::basic_string<char, std::char_traits<char>, krnlib::default_allocator<char>>;
template <class T>
using unique_ptr = std::unique_ptr<T, krnlib::default_delete<T>>;
template <class T>
using shared_ptr = std::shared_ptr<T>;

#pragma region make_unique
template <class T, class... ArgsT, std::enable_if_t<!std::is_array_v<T>, int> = 0>
_NODISCARD_SMART_PTR_ALLOC _CONSTEXPR23 unique_ptr<T> make_unique(ArgsT&&... args) {
    return unique_ptr<T>(New<T>(std::forward<ArgsT>(args)...));
}

template <class T, std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> == 0, int> = 0>
_NODISCARD_SMART_PTR_ALLOC _CONSTEXPR23 unique_ptr<T> make_unique(const size_t size) {
    using ElemT = std::remove_extent_t<T>;
    return unique_ptr<T>(NewArray<ElemT>(size));
}

_EXPORT_STD template <class T, class... ArgsT, std::enable_if_t<std::extent_v<T> != 0, int> = 0>
void make_unique(ArgsT&&...) = delete;
#pragma endregion

#pragma region make_share
template <class T, class... ArgsT>
_NODISCARD_SMART_PTR_ALLOC
#if _HAS_CXX20
std::enable_if_t<!std::is_array_v<T>, shared_ptr<T>>
#else // _HAS_CXX20
shared_ptr<T>
#endif // _HAS_CXX20
make_shared(ArgsT&&... args) { // make a shared_ptr to non-array object
    return std::allocate_shared<T, krnlib::default_allocator>(krnlib::default_allocator(), std::forward<ArgsT>(args)...);
}
#pragma endregion
}