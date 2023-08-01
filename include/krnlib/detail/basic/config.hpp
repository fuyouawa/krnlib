#pragma once
#ifdef WINNT
#include <ntifs.h>
#endif

namespace krnlib {
namespace details {
#ifdef WINNT
    // 内存分配类型
    constexpr POOL_TYPE kAllocPoolType = PagedPool;
    // 内存分配Tag
    constexpr ULONG kAllocPoolTag = 'mMuF';
#endif // WINNT
}
}