#include <string.h>

#include <cable/core/allocator.h>

typedef struct Record Record;
struct Record {
    Record *next;
    Record *prev;
    CblAllocator *alloc;
    size_t size;
};

struct CblAllocator {
    CblConcreteObject isa;
    CblAllocatorContext context;
    atomic_uint allocationBalance;
    Record *head;
    Record *tail;
};

static void initAllocationRecord(Record *record, CblAllocator *alloc, size_t size) {
    record->next = NULL;
    record->prev = NULL;
    record->alloc = alloc;
    record->size = size;
    if (!alloc->tail) {
        alloc->head = record;
        alloc->tail = record;
    } else {
        alloc->tail->next = record;
        record->prev = alloc->tail;
        alloc->tail = record;
    }
}

static void cleanupAllocationRecord(Record *record) {
    CblAllocator *alloc = record->alloc;
    if (alloc->head == record && alloc->tail == record) {
        alloc->head = NULL;
        alloc->tail = NULL;
    } else if (alloc->head == record) {
        alloc->head = record->next;
        alloc->head->prev = NULL;
    } else if (alloc->tail == record) {
        alloc->tail = record->prev;
        alloc->tail->next = NULL;
    } else {
        Record *after = record->next;
        Record *before = record->prev;
        after->prev = before;
        before->next = after;
    }
}

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
        .head = NULL,
        .tail = NULL
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
    Record *record = alloc->context.allocate(alloc, sizeof(Record) + size, alloc->context.userData);
    initAllocationRecord(record, alloc, size);
    return record + 1;
}

void *cblAllocatorReallocate(CblAllocator *alloc, void *ptr, size_t size) {
    if (size == 0) {
        cblAllocatorDeallocate(alloc, ptr);
        return NULL;
    }
    cblReturnUnless(ptr, cblAllocatorAllocate(alloc, size));
    if (alloc == NULL) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    Record *record = ((Record *)ptr) - 1;
    cleanupAllocationRecord(record);
    record = alloc->context.reallocate(alloc, record, sizeof(Record) + size, alloc->context.userData);
    initAllocationRecord(record, alloc, size);
    return record + 1;
}

void cblAllocatorDeallocate(CblAllocator *alloc, void *ptr) {
    cblBailUnless(ptr);
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    atomic_fetch_add(&alloc->allocationBalance, -1);
    Record *record = ((Record *)ptr) - 1;
    cleanupAllocationRecord(record);
    alloc->context.deallocate(alloc, record, alloc->context.userData);
}

size_t cblAllocatorGetAllocationBalance(CblAllocator *alloc) {
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    return atomic_load(&alloc->allocationBalance);
}

bool cblAllocatorHasAllocationRecord(CblAllocator *alloc, const void *ptr) {
    cblReturnUnless(ptr, false);
    Record *record = ((Record *)ptr) - 1;
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    return record->alloc == alloc;
}