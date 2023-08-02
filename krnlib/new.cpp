#include <new>
#include <ntifs.h>

// 内存分配类型
constexpr POOL_TYPE kAllocPoolType = PagedPool;
// 内存分配Tag
constexpr ULONG kAllocPoolTag = 'mMuF';

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size) {
	auto ptr = ExAllocatePoolWithTag(kAllocPoolType, size, kAllocPoolTag);
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
		ExFreePoolWithTag(ptr, kAllocPoolTag);
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