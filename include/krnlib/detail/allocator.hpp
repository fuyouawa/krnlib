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

    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal _CXX20_DEPRECATE_IS_ALWAYS_EQUAL = std::true_type;

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

    static constexpr size_t _Minimum_asan_allocation_alignment = std::_Asan_granularity;
};
}