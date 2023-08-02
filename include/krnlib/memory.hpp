#pragma once
#ifdef WINNT
#include <ntifs.h>
#endif
#include <memory>

namespace krnlib {
/**
 * @brief 分配指定数量的对象内存(sizeof(T) * count)
 * @tparam T 对象类型
 * @param count 数量
 * @return 内存
*/
template<class T>
inline T* Allocate(size_t count) {
    return (T*)operator new(std::_Get_size_of_n<sizeof(T)>(count));
}

/**
 * @brief 释放内存
 * @param ptr 要释放的内存
*/
inline void Deallocate(void* ptr) {
    operator delete(ptr);
}

/**
 * @brief 对一块内存调用构造函数
 * @tparam T 对象类型
 * @param ptr 目标内存
 * @param args 构造函数参数
 * @return 目标内存
*/
template<class T, class... ArgsT>
inline T* Construct(T* ptr, ArgsT&&... args) {
    return ::new(std::_Voidify_iter(ptr)) T(std::forward<ArgsT>(args)...);
}

/**
 * @brief 对一块内存调用析构函数
 * @tparam T 对象类型
 * @param ptr 目标内存
*/
template<class T>
inline void Destroy(T* ptr) {
    ptr->~T();
}
}