#pragma once
#ifdef WINNT
#include <ntifs.h>
#endif
#include <memory>
#include <stdexcept>
#include "krnlib/algorithm.hpp"
#include "krnlib/type_traits_extension.hpp"

namespace krnlib {
namespace details {
#pragma region 内部用结构
constexpr POOL_TYPE kAllocPoolType = PagedPool;
constexpr ULONG kAllocPoolTag = 'mMuF';
template<class T>
struct NewArrayMemory {
    size_t count;
    T objs[1];
};
#pragma endregion
}

#pragma region  普通内存操作函数
inline void* Malloc(size_t size) {
#ifdef WINNT
    auto ptr = ExAllocatePoolWithTag(details::kAllocPoolType, size, details::kAllocPoolTag);
#else
    auto ptr = malloc(size);
#endif // !WINNT
    KRNLIB_ASSERT(ptr != nullptr);
    return ptr;
}

inline void Free(void* ptr) {
#ifdef WINNT
    ExFreePoolWithTag(ptr, details::kAllocPoolTag);
#else
    free(ptr);
#endif // !WINNT
}


template<class T>
inline T* Allocate(size_t count) {
    return (T*)Malloc(std::_Get_size_of_n<sizeof(T)>(count));
}

inline void Deallocate(void* ptr) {
    Free(ptr);
}

template<class T, class... ArgsT>
inline T* Construct(T* ptr, ArgsT&&... args) {
    return ::new(std::_Voidify_iter(ptr)) T(std::forward<ArgsT>(args)...);
}

template<class T>
inline void Destroy(T* ptr) {
    ptr->~T();
}
#pragma endregion

#pragma region New
template<class T, class... ArgsT>
inline T* New(ArgsT&&... args) {
    auto ptr = Allocate<T>(1);
    RtlZeroMemory(ptr, sizeof(T));
    return Construct(ptr, std::forward<ArgsT>(args)...);
}

template<class ElemT, class... CtorArgsT, std::enable_if_t<!std::is_array_v<ElemT>, int> = 0>
inline ElemT* NewArrayImplicit(size_t count, CtorArgsT&&... ctor_args) {
    using NewArrayMemoryT = details::NewArrayMemory<ElemT>;
    constexpr size_t ctor_args_count = sizeof...(ctor_args);
    AssertCheckVariableTypesImplicit<ElemT, CtorArgsT...>();

    KRNLIB_ASSERT(ctor_args_count <= count);
    if (count == 0) count = 1;

    constexpr size_t alloc_size = std::_Get_size_of_n<sizeof(ElemT)>(count - 1) + sizeof(NewArrayMemoryT);
    NewArrayMemoryT* array_mem = (NewArrayMemoryT*)Malloc(alloc_size);
    RtlZeroMemory(array_mem, alloc_size);
    array_mem->count = count;

    if constexpr (ctor_args_count > 0) {
        TraverseVariable([&](ElemT&& elem, size_t cur_idx) {
            RtlMoveMemory(&array_mem->objs[cur_idx], std::addressof(elem), sizeof(ElemT));
            RtlZeroMemory(std::addressof(elem), sizeof(ElemT));
            }, 0, count,
            std::forward<CtorArgsT>(ctor_args)...);
    }
    for (size_t i = ctor_args_count; i < count; i++)
        Construct<ElemT>(&array_mem->objs[i]);

    return (ElemT*)array_mem->objs;
}

template<class ElemT, class... CtorElemsT, std::enable_if_t<!std::is_array_v<ElemT>, int> = 0>
inline ElemT* NewArray(size_t count, CtorElemsT&&... ctor_elems) {
    AssertCheckVariableTypes<ElemT, CtorElemsT...>();
    return NewArrayImplicit<ElemT>(count, std::forward<CtorElemsT>(ctor_elems)...);
}
#pragma endregion

#pragma region delete
template<class T>
inline void Delete(T* ptr) {
    Destroy(ptr);
    Deallocate(ptr);
}

template<class ElemT, std::enable_if_t<!std::is_array_v<ElemT>, int> = 0>
inline void DeleteArray(ElemT* ptr) {
    using NewArrayMemoryT = details::NewArrayMemory<ElemT>;

    NewArrayMemoryT* array_mem = (NewArrayMemoryT*)((char*)ptr - sizeof(size_t));
    for (size_t i = 0; i < array_mem->count; i++)
        Destroy(&array_mem->objs[i]);
    Deallocate(array_mem);
}
#pragma endregion

namespace details {
#pragma region 内存分配器
template <class T>
class allocator {
public:
    static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements "
        "because allocator<const T> is ill-formed.");

    using _From_primary = allocator;

    using value_type = T;

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
    using pointer _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS = T*;
    using const_pointer _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS = const T*;

    using reference _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS = T&;
    using const_reference _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS = const T&;
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal _CXX20_DEPRECATE_IS_ALWAYS_EQUAL = std::true_type;

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
    template <class _Other>
    struct _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS rebind {
        using other = allocator<_Other>;
    };

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD T* address(T& val) const noexcept {
        return std::addressof(val);
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD const T* address(const T& val) const noexcept {
        return std::addressof(val);
    }
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

    constexpr allocator() noexcept {}

    constexpr allocator(const allocator&) noexcept = default;
    template <class _Other>
    constexpr allocator(const allocator<_Other>&) noexcept {}
    _CONSTEXPR20 ~allocator() = default;
    _CONSTEXPR20 allocator& operator=(const allocator&) = default;

    _CONSTEXPR20 void deallocate(T* const ptr, const size_t) {
        _STL_ASSERT(ptr != nullptr, "null pointer cannot point to a block of non-zero size");
        Deallocate(ptr);
    }

    _CONSTEXPR20 void deallocate(T* const ptr) {
        this->deallocate(ptr, 0);
    }

    _NODISCARD_RAW_PTR_ALLOC _CONSTEXPR20 __declspec(allocator) T* allocate(_CRT_GUARDOVERFLOW const size_t count) {
        static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
        return Allocate<T>(count);
    }

#if _HAS_CXX23
    _NODISCARD_RAW_PTR_ALLOC constexpr allocation_result<T*> allocate_at_least(
        _CRT_GUARDOVERFLOW const size_t count) {
        return { this->allocate(count), count };
    }
#endif // _HAS_CXX23

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD_RAW_PTR_ALLOC __declspec(allocator) T* allocate(
        _CRT_GUARDOVERFLOW const size_t count, const void*) {
        return this->allocate(count);
    }

    template <class U, class... ArgsT>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void construct(U* const ptr, ArgsT&&... args) {
        Construct<U>(ptr, std::forward<ArgsT>(args)...);
    }

    template <class U>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void destroy(U* const ptr) {
        ptr->~U();
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD size_t max_size() const noexcept {
        return static_cast<size_t>(-1) / sizeof(T);
    }
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

    static constexpr size_t _Minimum_asan_allocation_alignment = std::_Asan_granularity;
    };
#pragma endregion

#pragma region 内存析构器
template <class T>
struct default_delete { // default deleter for unique_ptr
    constexpr default_delete() noexcept = default;

    template <class NewT, std::enable_if_t<std::is_convertible_v<NewT*, T*>, int> = 0>
    _CONSTEXPR23 default_delete(const default_delete<NewT>&) noexcept {}

    _CONSTEXPR23 void operator()(T* ptr) const noexcept /* strengthened */ { // delete a pointer
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        Delete<T>(ptr);
    }
};

template <class T>
struct default_delete<T[]> { // default deleter for unique_ptr to array of unknown size
    constexpr default_delete() noexcept = default;

    template <class NewT, std::enable_if_t<std::is_convertible_v<NewT(*)[], T(*)[]>, int> = 0>
    _CONSTEXPR23 default_delete(const default_delete<NewT[]>&) noexcept {}

    template <class U, std::enable_if_t<std::is_convertible_v<U(*)[], T(*)[]>, int> = 0>
    _CONSTEXPR23 void operator()(U* ptr) const noexcept /* strengthened */ { // delete a pointer
        static_assert(0 < sizeof(U), "can't delete an incomplete type");
        DeleteArray<U>(ptr);
    }
};
#pragma endregion
}

template<class T>
using default_allocator = details::allocator<T>;
template<class T>
using default_delete = details::default_delete<T>;
}
