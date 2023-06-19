/*
* 因为在驱动开发中std::initializer_list在涉及到会发生隐式转换的初始化时会报错(c++这个糟老头子肯定悄咪咪夹杂私货了)
* 所以我自己实现了一个使用方法和std::initializer_list几乎一样的容器
* 但是缺点就是不能像他那样在编译期就确定好栈中的大小, 而是在运行时动态在堆中分配内存
*/

#pragma once
#include <type_traits>
#include <utility>
#include <krnlib/memory.hpp>

namespace krnlib
{
namespace detail
{
/*
* InitListWrapper是利用递归将可变模板展开, 并将参数构造成对象保存入缓冲区中
*
*   T                 要将参数列表构造成的对象
*   CurArgsSeek       辅助函数, 标识当前是参数列表中的第几个参数
*   Args              参数列表
*/
template<class T, size_t cur_args_idx, class... Args>
class InitListWrapper;
template<class T, size_t cur_args_idx>
class InitListWrapper<T, cur_args_idx> {
public:
    constexpr InitListWrapper(T* buffer) noexcept {}
};
template<class T, size_t cur_args_idx, class ThisArgT, class...RestArgsT>
class InitListWrapper<T, cur_args_idx, ThisArgT, RestArgsT...> : public InitListWrapper<T, cur_args_idx + 1, RestArgsT...>
{
public:
    using InheritedT = InitListWrapper<T, cur_args_idx + 1, RestArgsT...>;

    constexpr InitListWrapper(T* buffer, ThisArgT&& this_arg, RestArgsT&&... rest_args) :
        InheritedT(buffer, std::forward<RestArgsT>(rest_args)...)
    {
        /* 使用total的内存, 将当前参数total_begin构造成T对象*/
        Construct(&buffer[cur_args_idx], std::forward<ThisArgT>(this_arg));
    }
};
}
/*
* 模拟std::initializer_list
*/
template<class T>
class initializer_list
{
public:
    /*
    * 接收一个可变参数模板, 根据模板大小分配内存, 然后传入InitListWrapper
    * InitListWrapper内部将args展开, 并根据参数索引按照顺序保存入buffer_中(方便迭代)
    */
    template<class... ArgsT>
    initializer_list(ArgsT&&... args) :
        size_(sizeof...(args))
    {
        buffer_ = Allocate<T>(size_);
        using WrapperT = detail::InitListWrapper<T, 0, std::_Unrefwrap_t<ArgsT>...>;
        WrapperT(buffer_, std::forward<ArgsT>(args)...);
    }

    ~initializer_list() {
        Deallocate(buffer_);
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