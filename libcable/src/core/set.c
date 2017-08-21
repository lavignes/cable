#include <cable/core/set.h>
#include <cable/core/allocator.h>
#include <cable/core/data.h>
#include <cable/core/string.h>

static const size_t DEFAULT_SIZE = 16;

static void *DUMMY = (void *)CBL_NOT_FOUND;

typedef struct Bucket {
    size_t hashCode;
    const void *element;
} Bucket;

static CblSetContext OBJECT_CONTEXT = {
        .ownCallback = (CblSetOwnCallback)cblOwnInOwner,
        .disownCallback = (CblSetDisownCallback)cblDisownInOwner,
        .compareCallback = cblCompare,
        .hashCallback = cblGetHash,
        .stringCallback = cblGetString,
};

const CblSetContext * const CBL_SET_CONTEXT_OBJECTS = &OBJECT_CONTEXT;

static CblSetContext ALLOC_CONTEXT = {
        .ownCallback = NULL,
        .disownCallback = (CblSetDisownCallback)cblDeallocateInOwner,
        .compareCallback = NULL,
        .hashCallback = NULL,
        .stringCallback = NULL,
};

const CblSetContext * const CBL_SET_CONTEXT_ALLOCS = &ALLOC_CONTEXT;

struct CblSet {
    CblConcreteObject isa;
    CblSetContext context;
    CblMutableData *buffer;
    size_t length;
    size_t loadFactor;
};

static void finalize(CblMutableSet *set) {
    cblSetEmpty(set);
    cblDisown(set->buffer);
}

static inline size_t getDataVirtualLength(CblData *data) {
    return cblDataGetLength(data) / sizeof(Bucket);
}

static CblString *stringCallback(CblAllocator *alloc, CblMutableSet *set) {
    cblReturnUnless(set, NULL);
    const CblSetContext *context = &set->context;
    cblReturnUnless(context->stringCallback, NULL);
    CblMutableString *string = cblMutableStringNewWithCString(alloc, "{");
    size_t size = getDataVirtualLength(set->buffer);
    const Bucket *buckets = (Bucket *)cblDataGetBytePointer(set->buffer);
    const void *element;
    for (size_t i = 0, j = 0; i < size; ++i) {
        element = buckets[i].element;
        if (!element || element == DUMMY) {
            continue;
        }
        CblString *temp = context->stringCallback(alloc, element);
        cblStringAppend(string, temp);
        cblDisown(temp);
        if (j < set->length - 1) {
            cblStringAppendCString(string, ", ");
        }
        j++;
    }
    cblStringAppendCString(string, "}");
    return string;
}

static CblClass SET_CLASS = {
        .name = "CblSet",
        .finalizeCallback = (CblObjectFinalizeCallback)finalize,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = (CblObjectStringCallback)stringCallback,
};

const CblClass * const CBL_SET_CLASS = &SET_CLASS;

static inline Bucket *getBucketForElement(const CblSetContext *context,
                                             CblMutableData *buffer,
                                             const void *element,
                                             size_t *hashCode)
{
    cblReturnUnless(context && buffer && element, NULL);
    size_t code = context->hashCallback ? context->hashCallback(element) : (size_t)(uintptr_t)element;
    if (hashCode) {
        *hashCode = code;
    }
    size_t size = getDataVirtualLength(buffer);
    size_t index = code % size;
    Bucket *buckets = (Bucket *)cblDataGetMutableBytePointer(buffer);
    Bucket *bucket = &buckets[index];
    while (bucket->element) {
        if (bucket->element != DUMMY && code == bucket->hashCode) {
            if (!context->compareCallback) {
                if (element == bucket->element) {
                    return bucket;
                }
            } else if (context->compareCallback(element, bucket->element) == 0) {
                return bucket;
            }
        }
        index++;
        if (index == size) {
            size = 0;
        }
        bucket = &buckets[index];
    }
    return bucket;
}

static inline void expandSet(CblMutableSet *set) {
    const CblSetContext *context = &set->context;
    CblMutableData *newBuffer = cblMutableDataNew(cblGetAllocator(set), cblDataGetSize(set->buffer) * 2);
    size_t size = getDataVirtualLength(set->buffer);
    const Bucket *buckets = (Bucket *)cblDataGetBytePointer(set->buffer);
    const void *element;
    for (size_t i = 0; i < size; ++i) {
        element = buckets[i].element;
        if (!element || element == DUMMY) {
            continue;
        }
        size_t hashCode;
        Bucket *bucket = getBucketForElement(context, newBuffer, element, &hashCode);
        bucket->element = element;
        bucket->hashCode = hashCode;
    }
    cblDisown(set->buffer);
    set->buffer = newBuffer;
    set->loadFactor = ((set->length * 2) >> 2) * 3;
}

CblSet *cblSetNew(CblAllocator *alloc, const CblSetContext *context, const void **elements, size_t length) {
    return cblMutableSetNewWithElements(alloc, context, elements, length);
}

CblSet *cblSetNewCopy(CblAllocator *alloc, CblSet *set) {
    return cblMutableSetNewCopy(alloc, set);
}

CblMutableSet *cblMutableSetNew(CblAllocator *alloc, const CblSetContext *context) {
    return cblMutableSetNewWithElements(alloc, context, NULL, 0);
}

CblMutableSet *cblMutableSetNewWithElements(CblAllocator *alloc,
                                            const CblSetContext *context,
                                            const void **elements,
                                            size_t length)
{
    if (!context) {
        context = CBL_SET_CONTEXT_OBJECTS;
    }
    if (!elements && length < DEFAULT_SIZE) {
        length = DEFAULT_SIZE;
    }
    CblMutableData *buffer = cblMutableDataNewWithBytes(alloc, (uint8_t *) elements, sizeof(void *) * length);
    cblReturnUnless(buffer, NULL);
    CblMutableSet *set = cblAllocatorAllocate(alloc, sizeof(CblMutableSet));
    if (!set) {
        cblAllocatorDeallocate(alloc, buffer);
        return NULL;
    }
    cblInitialize(set, alloc, &SET_CLASS);
    set->context = *context;
    set->buffer = buffer;
    set->length = 0;
    set->loadFactor = (length >> 2) * 3;
    if (elements) {
        for (size_t i = 0; i < length; ++i) {
            cblSetSet(set, elements[i]);
        }
    }
    return set;
}

CblMutableSet *cblMutableSetNewCopy(CblAllocator *alloc, CblSet *set) {
    cblReturnUnless(set, NULL);
    CblMutableSet *newSet = cblMutableSetNew(alloc, &set->context);
    cblReturnUnless(newSet, NULL);
    size_t size = getDataVirtualLength(set->buffer);
    const Bucket *buckets = (Bucket *)cblDataGetBytePointer(set->buffer);
    const void *element;
    for (size_t i = 0; i < size; ++i) {
        element = buckets[i].element;
        if (!element || element == DUMMY) {
            continue;
        }
        cblSetSet(newSet, element);
    }
    return newSet;
}

size_t cblSetGetLength(CblSet *set) {
    cblReturnUnless(set, 0);
    return set->length;
}

const void *cblSetGet(CblSet *set, const void *element) {
    cblReturnUnless(set, NULL);
    const Bucket *bucket = getBucketForElement(&set->context, set->buffer, element, NULL);
    cblReturnUnless(bucket, NULL);
    return bucket->element;
}

bool cblSetForeach(CblSet *set, CblSetForeachFunction foreachFunction, void *userData) {
    cblReturnUnless(set && foreachFunction, false);
    size_t size = getDataVirtualLength(set->buffer);
    const Bucket *buckets = (Bucket *)cblDataGetBytePointer(set->buffer);
    const void *element;
    for (size_t i = 0; i < size; ++i) {
        element = buckets[i].element;
        if (!element || element == DUMMY) {
            continue;
        }
        if (foreachFunction(set, element, userData)) {
            return true;
        }
    }
    return false;
}

void cblSetSet(CblMutableSet *set, const void *element) {
    cblBailUnless(set && element);
    size_t hashCode;
    const CblSetContext *context = &set->context;
    Bucket *bucket = getBucketForElement(context, set->buffer, element, &hashCode);
    if (!bucket->element) {
        set->length++;
    }
    if (context->disownCallback && bucket->element) {
        context->disownCallback(set, bucket->element);
    }
    if (context->ownCallback) {
        context->ownCallback(set, element);
    }
    bucket->element = element;
    bucket->hashCode = hashCode;
    if (set->length >= set->loadFactor) {
        expandSet(set);
    }
}

void cblSetRemove(CblMutableSet *set, const void *element) {
    cblBailUnless(set);
    const CblSetContext *context = &set->context;
    Bucket *bucket = getBucketForElement(context, set->buffer, element, NULL);
    cblBailUnless(bucket->element);
    if (context->disownCallback) {
        context->disownCallback(set, bucket->element);
    }
    bucket->element = DUMMY;
    set->length--;
}

void cblSetEmpty(CblMutableSet *set) {
    cblBailUnless(set);
    const CblSetContext *context = &set->context;
    size_t size = getDataVirtualLength(set->buffer);
    Bucket *buckets = (Bucket *)cblDataGetMutableBytePointer(set->buffer);
    Bucket *bucket;
    for (size_t i = 0; i < size; ++i) {
        bucket = &buckets[i];
        if (!bucket->element) {
            continue;
        }
        if (context->disownCallback && bucket->element != DUMMY) {
            context->disownCallback(set, bucket->element);
        }
        bucket->element = DUMMY;
    }
    set->length = 0;
}