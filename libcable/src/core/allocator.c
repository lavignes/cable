#include <string.h>

#include <cable/core/allocator.h>

struct CblAllocator {
    CblConcreteObject isa;
    CblAllocatorContext context;
    atomic_uint allocationBalance;
};

static void *mallocAllocate(CblAllocator *alloc, size_t size, void *userData) {
    void *ptr = calloc(1, size);
    if (ptr) {
        cblOwn(alloc);
    }
    return ptr;
}

static void mallocFree(CblAllocator *alloc, void *ptr, void *userData) {
    if (ptr && cblGetAllocator(alloc)) {
        cblDisown(alloc);
    }
    free(ptr);
}

static void *mallocRealloc(CblAllocator *alloc, void *ptr, size_t newSize, void *userData) {
    void *newPtr = realloc(ptr, newSize);
    if (cblGetAllocator(alloc)) {
        if (!ptr && newPtr) {
            cblOwn(alloc);
        } else if (ptr && newSize == 0) {
            cblDisown(alloc);
        }
    }
    return newPtr;
}

static CblClass ALLOCATOR_CLASS = {
        .name = "CblAllocator",
        .finalizeCallback = NULL,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

CblClass * const CBL_ALLOCATOR_CLASS = &ALLOCATOR_CLASS;

static CblAllocator MALLOC_ALLOCATOR = {
        .isa = {
                .isa = &ALLOCATOR_CLASS,
                .alloc = NULL,
        },
        .context = {
                .userData = NULL,
                .allocate = mallocAllocate,
                .reallocate = mallocRealloc,
                .deallocate = mallocFree,
        },
        .allocationBalance = ATOMIC_VAR_INIT(0),
};

CblAllocator * const CBL_ALLOCATOR_MALLOC = &MALLOC_ALLOCATOR;
CblAllocator * const CBL_ALLOCATOR_DEFAULT = &MALLOC_ALLOCATOR;

CblAllocator *cblAllocatorNew(CblAllocator *alloc, const CblAllocatorContext *context) {
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    if (!context) {
        context = &alloc->context;
    }
    CblAllocator *allocator = cblAllocatorAllocate(alloc, sizeof(CblAllocator));
    cblReturnUnless(allocator, NULL);
    cblInitialize(allocator, alloc, &ALLOCATOR_CLASS);
    allocator->context = *context;
    atomic_store(&allocator->allocationBalance, 0);
    return allocator;
}

void *cblAllocatorAllocate(CblAllocator *alloc, size_t size) {
    cblReturnUnless(size > 0, NULL);
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    atomic_fetch_add(&alloc->allocationBalance, 1);
    return alloc->context.allocate(alloc, size, alloc->context.userData);
}

void *cblAllocatorReallocate(CblAllocator *alloc, void *ptr, size_t size) {
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    if (size == 0) {
        cblAllocatorDeallocate(alloc, ptr);
        return NULL;
    }
    cblReturnUnless(ptr, cblAllocatorAllocate(alloc, size));
    return alloc->context.reallocate(alloc, ptr, size, alloc->context.userData);
}

void cblAllocatorDeallocate(CblAllocator *alloc, void *ptr) {
    cblBailUnless(ptr);
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    atomic_fetch_add(&alloc->allocationBalance, -1);
    alloc->context.deallocate(alloc, ptr, alloc->context.userData);
}

size_t cblAllocatorGetAllocationBalance(CblAllocator *alloc) {
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    return atomic_load(&alloc->allocationBalance);
}
