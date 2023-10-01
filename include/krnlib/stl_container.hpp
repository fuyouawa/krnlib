#pragma once
#if __cplusplus >= 202002L
#pragma message("WARNING: There's no need to use this hpp file in C++20. You can directly use the standard library!")
#endif
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include "krnlib/detail/allocator.hpp"

/*
* STL容器驱动版
*/
namespace krnlib {
template<class T, class AllocT = krnlib::allocator<T>>
using list = std::list<T, AllocT>;

template<class T, class AllocT = krnlib::allocator<T>>
using vector = std::vector<T, AllocT>;

template <class KeyT, class ValueT, class AllocT = krnlib::allocator<std::pair<const KeyT, ValueT>>>
using map = std::map<KeyT, ValueT, std::less<KeyT>, AllocT>;

template <class KeyT, class ValueT, class AllocT = krnlib::allocator<std::pair<const KeyT, ValueT>>>
using multimap = std::multimap<KeyT, ValueT, std::less<KeyT>, AllocT>;

template <class T, class AllocT = krnlib::allocator<T>>
using deque = std::deque<T, AllocT>;

template <class T, class AllocT = krnlib::allocator<T>>
using stack = std::stack<T, deque<T, AllocT>>;

template <class T, class AllocT = krnlib::allocator<T>>
using set = std::set<T, std::less<T>, AllocT>;

template <class T, class AllocT = krnlib::allocator<T>>
using multiset = std::multiset<T, std::less<T>, AllocT>;
}