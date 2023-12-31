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
*/
template<typename T>
[[nodiscard]] inline T* Allocate(size_t count) {
    return (T*)operator new(sizeof(T) * count);
}

/**
 * @brief 释放内存
 * @param ptr 要释放的内存
*/
inline void Deallocate(void* ptr) {
    operator delete(ptr);
}

/**
 * @brief 对一块内存调用析构函数
 * @tparam T 对象类型
*/
template<typename T>
inline void Destroy(T* ptr) {
    ptr->~T();
}
}