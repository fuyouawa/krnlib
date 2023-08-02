#pragma once
#include "krnlib/memory.hpp"
#include "krnlib/detail/basic/algorithm.hpp"

namespace krnlib {
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
        DEBUG_ASSERT(ptr != nullptr, "null pointer cannot point to a block of non-zero size");
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
}