#include <new>
#include "krnlib/detail/basic/config.hpp"

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size) {
	auto ptr = ExAllocatePoolWithTag(krnlib::details::kAllocPoolType, size, krnlib::details::kAllocPoolTag);
	if (!ptr)
		throw std::bad_alloc();
	return ptr;
}

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size) {
	return operator new(size);
}

void __CRTDECL operator delete(void* ptr) noexcept {
	if (ptr)
		ExFreePoolWithTag(ptr, krnlib::details::kAllocPoolTag);
}

void __CRTDECL operator delete[](void* ptr) noexcept {
	operator delete(ptr);
}

void __CRTDECL operator delete(void* ptr, size_t size) noexcept {
	operator delete(ptr);
}

void __CRTDECL operator delete[](void* ptr, size_t size) noexcept {
	operator delete(ptr);
}