/*
* 因为在驱动开发中std::initializer_list在涉及到会发生隐式转换的初始化时会报错(c++这个糟老头子肯定悄咪咪夹杂私货了)
* 所以我自己实现了一个使用方法和std::initializer_list几乎一样的容器
* 但是缺点就是不能像他那样在编译期就确定好栈中的大小, 而是在运行时动态在堆中分配内存
*/

#pragma once
#include <type_traits>
#include <utility>
#include "krnlib/memory.hpp"

namespace krnlib {
/*
* 模拟std::initializer_list
*/
template<class T>
class initializer_list {
public:
    template<class... ArgsT>
    initializer_list(ArgsT&&... args) : size_(sizeof...(args)) {
        buffer_ = NewArrayImplicit<T>(size_, std::forward<ArgsT>(args)...);
    }

    ~initializer_list() {
        DeleteArray(buffer_);
    }

    T* begin() const noexcept {
        return buffer_;
    }
    T* end() const noexcept {
        return buffer_ + size_;
    }
    size_t size() const noexcept {
        return size_;
    }

private:
    T* buffer_;
    size_t size_;
};
}