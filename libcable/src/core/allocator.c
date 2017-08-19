#include <string.h>

#include <backtrace.h>

#include <cable/core/allocator.h>

typedef struct CallFrame CallFrame;
struct CallFrame {
    CallFrame *next;
    char *fileName;
    char *function;
    size_t lineNumber;
};

typedef struct Record Record;
struct Record {
    Record *next;
    Record *prev;
    CblAllocator *alloc;
    size_t size;
    CallFrame *head;
    CallFrame *tail;
};

struct CblAllocator {
    CblConcreteObject isa;
    CblAllocatorContext context;
    atomic_uint allocationBalance;
    Record *head;
    Record *tail;
};

static int traceAllocationRecord(Record *record,
                                    uintptr_t pc,
                                    const char *fileName,
                                    int lineNumber,
                                    const char *function)
{
    cblReturnUnless(fileName && lineNumber && function, 0);
    size_t fileNameSize = strlen(fileName) + 1;
    size_t functionSize = strlen(function) + 2 + 1;
    CblAllocator *alloc = record->alloc;
    CblAllocatorContext *context = &alloc->context;
    CallFrame *frame = context->allocate(alloc, sizeof(CallFrame), context->userData);
    frame->fileName = context->allocate(alloc, fileNameSize, context->userData);
    frame->function = context->allocate(alloc, functionSize, context->userData);
    frame->lineNumber = (size_t)lineNumber;
    char *baseName = strrchr(fileName, '/') + 1;
    if (baseName != NULL) {
        fileNameSize =  baseName - fileName;
        fileName = baseName;
    }
    memmove(frame->fileName, fileName, fileNameSize);
    memmove(frame->function, function, functionSize);
    if (!record->tail) {
        record->head = frame;
        record->tail = frame;
    } else {
        record->tail->next = frame;
        record->tail = frame;
    }
    return 0;
}

static void initAllocationRecord(Record *record, CblAllocator *alloc, size_t size) {
    static struct backtrace_state *bs = NULL;
    if (!bs) {
        bs = backtrace_create_state(NULL, 1, NULL, NULL);
    }
    record->next = NULL;
    record->prev = NULL;
    record->alloc = alloc;
    record->size = size;
    record->head = NULL;
    record->tail = NULL;
    backtrace_full(bs, 2, (void *)traceAllocationRecord, NULL, record);
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
    CblAllocatorContext *context = &alloc->context;
    CallFrame *frame = record->head;
    while (frame) {
        CallFrame *next = frame->next;
        context->deallocate(alloc, frame->fileName, context->userData);
        context->deallocate(alloc, frame->function, context->userData);
        context->deallocate(alloc, frame, context->userData);
        frame = next;
    }
    record->head = NULL;
    record->tail = NULL;
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

void cblAllocatorPrintAllocationRecord(const void *ptr, FILE *file) {
    cblBailUnless(ptr && file);
    Record *record = ((Record *)ptr) - 1;
    CblConcreteObject *candidateObject = (CblConcreteObject *)ptr;
    if (candidateObject->alloc == record->alloc && candidateObject->isa) {
        fprintf(file, "[%s (%zu bytes)]\n", candidateObject->isa->name, record->size);
    } else {
        fprintf(file, "[%zu bytes]\n", record->size);
    }
    CallFrame *frame = record->head;
    while (frame) {
        fprintf(file, "  %-38s %s:%zu\n", frame->function, frame->fileName, frame->lineNumber);
        frame = frame->next;
    }
    fprintf(file, "\n");
}

void cblAllocatorPrintLiveAllocationRecords(CblAllocator *alloc, FILE *file) {
    cblBailUnless(file);
    if (!alloc) {
        alloc = CBL_ALLOCATOR_DEFAULT;
    }
    Record *record = alloc->head;
    while (record) {
        cblAllocatorPrintAllocationRecord(record + 1, file);
        record = record->next;
    }
}