#ifndef CBL_CORE_ALLOCATOR_H_
#define CBL_CORE_ALLOCATOR_H_

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef struct CblAllocator CblAllocator;

/**
 * A class for allocators.
 */
CblClass * const CBL_ALLOCATOR_CLASS;

/**
 * The default allocator.
 */
CblAllocator * const CBL_ALLOCATOR_DEFAULT;

/**
 * An allocator that uses system malloc.
 */
CblAllocator * const CBL_ALLOCATOR_MALLOC;

/**
 * A hook executed when an allocator must perform an allocation.
 * @param alloc The allocator.
 * @param size The size (in bytes) of the allocation.
 * @param userData The user data of the allocator context.
 * @return A pointer. If the allocation failed, return null.
 */
typedef void *(*CblAllocatorAllocateHook)(CblAllocator *alloc, size_t size, void *userData);

/**
 * A hook executed when an allocator must perform a re-allocation.
 * @param alloc The allocator.
 * @param ptr The pointer to re-allocate. A null pointer should be treated as an allocation.
 * @param newSize The newSize (in bytes) of the allocation. A size of 0 should de-allocate the pointer.
 * @param userData The user data of the allocator context.
 * @return The re-allocated pointer. May be null if the pointer has been de-allocated, or the re-allocation fails.
 */
typedef void *(*CblAllocatorReallocateHook)(CblAllocator *alloc, void *ptr, size_t newSize, void *userData);

/**
 * A hook executed when an allocator must perform a de-allocation.
 * @param alloc The allocator.
 * @param ptr The pointer to de-allocate. A null pointer should be ignored.
 * @param userData the user data of the allocator context.
 */
typedef void (*CblAllocatorDeallocateHook)(CblAllocator *alloc, void *ptr, void *userData);

/**
 * A list of hooks that can be implemented to create custom allocators.
 */
typedef struct CblAllocatorContext {
    void *userData;
    CblAllocatorAllocateHook allocate;
    CblAllocatorReallocateHook reallocate;
    CblAllocatorDeallocateHook deallocate;
} CblAllocatorContext;

CblAllocator *cblAllocatorNew(CblAllocator *alloc, const CblAllocatorContext *context);

void *cblAllocatorAllocate(CblAllocator *alloc, size_t size);

void *cblAllocatorReallocate(CblAllocator *alloc, void *ptr, size_t size);

void cblAllocatorDeallocate(CblAllocator *alloc, void *ptr);

size_t cblAllocatorGetAllocationBalance(CblAllocator *alloc);

/**
 * Returns true if a pointer if currently allocated by this allocator.
 * @param alloc An allocator.
 * @param ptr A pointer to test.
 * @return true if the pointer is allocated.
 */
bool cblAllocatorHasAllocationRecord(CblAllocator *alloc, const void *ptr);

CBL_EXTERN_END

#endif // CBL_CORE_ALLOCATOR_H_