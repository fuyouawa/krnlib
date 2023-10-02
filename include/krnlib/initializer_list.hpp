#pragma once
#include <type_traits>

namespace krnlib {
/*
* 模拟std::initializer_list
*/
template<class T>
class initializer_list {
public:
    template<class... ArgsT>
    initializer_list(ArgsT&&... args) : size_(sizeof...(args)) {
        buffer_ = new T[sizeof...(args)]{std::forward<ArgsT>(args)...};
    }

    ~initializer_list() {
        delete[] buffer_;
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