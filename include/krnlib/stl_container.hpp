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
KRNLIB_BEGIN_NAMESPACE

template<class T, class AllocT = allocator<T>>
using list = std::list<T, AllocT>;

template<class T, class AllocT = allocator<T>>
using vector = std::vector<T, AllocT>;

template <class KeyT, class ValueT, class AllocT = allocator<std::pair<const KeyT, ValueT>>>
using map = std::map<KeyT, ValueT, std::less<KeyT>, AllocT>;

template <class KeyT, class ValueT, class AllocT = allocator<std::pair<const KeyT, ValueT>>>
using multimap = std::multimap<KeyT, ValueT, std::less<KeyT>, AllocT>;

template <class T, class AllocT = allocator<T>>
using deque = std::deque<T, AllocT>;

template <class T, class AllocT = allocator<T>>
using stack = std::stack<T, deque<T, AllocT>>;

template <class T, class AllocT = allocator<T>>
using set = std::set<T, std::less<T>, AllocT>;

template <class T, class AllocT = allocator<T>>
using multiset = std::multiset<T, std::less<T>, AllocT>;

KRNLIB_END_NAMESPACE