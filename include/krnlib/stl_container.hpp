
#pragma once
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
template<class T>
using list = std::list<T, krnlib::allocator<T>>;
template<class T>
using vector = std::vector<T, krnlib::allocator<T>>;
template <class KeyT, class ValueT>
using map = std::map<KeyT, ValueT, std::less<KeyT>, krnlib::allocator<std::pair<const KeyT, ValueT>>>;

template <class KeyT, class ValueT>
using multimap = std::multimap<KeyT, ValueT, std::less<KeyT>, krnlib::allocator<std::pair<const KeyT, ValueT>>>;
template <class T>
using deque = std::deque<T, krnlib::allocator<T>>;
template <class T>
using stack = std::stack<T, deque<T>>;
template <class T>
using set = std::set<T, std::less<T>, krnlib::allocator<T>>;
template <class T>
using multiset = std::multiset<T, std::less<T>, krnlib::allocator<T>>;
}