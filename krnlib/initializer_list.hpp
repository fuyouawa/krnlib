#pragma once
#include <type_traits>
#include <utility>
#include "krnlib/memory.hpp"

namespace krnlib {
/*
* Ä£Äâstd::initializer_list
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