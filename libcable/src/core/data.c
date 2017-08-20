#include <string.h>

#include <cable/core/data.h>
#include <cable/core/allocator.h>

struct CblData {
    CblConcreteObject isa;
    size_t size;
    size_t length;
    uint8_t *buffer;
};

static void finalize(CblData *data) {
    cblAllocatorDeallocate(cblGetAllocator(data), data->buffer);
}

static int dataCompare(CblData *lhs, CblData *rhs) {
    cblReturnUnless(lhs && rhs, -1);
    return memcmp(lhs->buffer, rhs->buffer, cblUSizeMin(lhs->length, rhs->length));
}

static CblClass DATA_CLASS = {
        .name = "CblData",
        .finalizeCallback = (CblObjectFinalizeCallback)finalize,
        .hashCallback = NULL,
        .compareCallback = (CblObjectCompareCallback)dataCompare,
        .stringCallback = NULL,
};

CblClass * const CBL_DATA_CLASS = &DATA_CLASS;

CblData *cblDataNew(CblAllocator *alloc, const uint8_t *bytes, size_t length) {
    return cblMutableDataNewWithBytes(alloc, bytes, length);
}

CblData *cblDataNewCopy(CblAllocator *alloc, CblData *data) {
    return cblMutableDataNewCopy(alloc, data);
}

CblMutableData *cblMutableDataNew(CblAllocator *alloc, size_t length) {
    uint8_t *buffer = cblAllocatorAllocate(alloc, length);
    cblReturnUnless(buffer || length == 0, NULL);
    CblMutableData *data = cblAllocatorAllocate(alloc, sizeof(CblData));
    if (!data) {
        cblAllocatorDeallocate(alloc, buffer);
        return NULL;
    }
    cblInitialize(data, alloc, &DATA_CLASS);
    data->buffer = buffer;
    data->size = length;
    data->length = length;
    return data;
}

CblMutableData *cblMutableDataNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length) {
    CblMutableData *data = cblMutableDataNew(alloc, length);
    cblReturnUnless(data, NULL);
    cblDataReplaceBytes(data, cblRangeNew(0, length), bytes, length);
    return data;
}

CblMutableData *cblMutableDataNewCopy(CblAllocator *alloc, CblData *data) {
    return cblMutableDataNewWithBytes(alloc, data->buffer, data->length);
}

size_t cblDataGetSize(CblData *data) {
    cblReturnUnless(data, 0);
    return data->size;
}

size_t cblDataGetLength(CblData *data) {
    cblReturnUnless(data, 0);
    return data->length;
}

void cblDataGetBytes(CblData *data, uint8_t *buffer, CblRange range) {
    cblBailUnless(data && buffer);
    memmove(buffer, &data->buffer[range.location], range.length);
}

const uint8_t *cblDataGetBytePointer(CblData *data) {
    cblReturnUnless(data, NULL);
    return data->buffer;
}

uint8_t *cblDataGetMutableBytePointer(CblMutableData *data) {
    cblReturnUnless(data, NULL);
    return data->buffer;
}

void cblDataSetSize(CblMutableData *data, size_t size) {
    cblBailUnless(data);
    void *newBuffer = cblAllocatorReallocate(cblGetAllocator(data), data->buffer, size);
    cblBailUnless(newBuffer);
    data->buffer = newBuffer;
    data->size = size;
    data->length = cblUSizeMin(data->length, size);
}

void cblDataSetLength(CblMutableData *data, size_t length) {
    cblBailUnless(data);
    data->length = cblUSizeMin(length, data->size);
}

void cblDataAppendBytes(CblMutableData *data, const uint8_t *bytes, size_t length) {
    cblBailUnless(data);
    cblDataReplaceBytes(data, cblRangeNew(data->length, 0), bytes, length);
}

void cblDataReplaceBytes(CblMutableData *data, CblRange range, const uint8_t *bytes, size_t length) {
    cblBailUnless(data);
    size_t newLength = data->length - range.length + length;
    if (data->size < newLength) {
        size_t newSize = data->size + (length - range.length);
        cblDataSetSize(data, newSize);
        cblBailUnless(data->size == newSize);
    }
    if (length != range.length && range.location + range.length < data->length) {
        memmove(&data->buffer[range.location + length],
                &data->buffer[range.location + range.length],
                (data->length - range.location - range.length));
    }
    if (length > 0) {
        if (bytes) {
            memmove(&data->buffer[range.location], bytes, length);
        } else {
            memset(&data->buffer[range.location], 0, length);
        }
    }
    data->length = newLength;
}

void cblDataAppendData(CblMutableData *data, CblData *append) {
    cblBailUnless(data && append);
    cblDataReplaceBytes(data, cblRangeNew(data->length, 0), append->buffer, append->length);
}

void cblDataReplaceData(CblMutableData *data, CblRange range, CblData *append) {
    cblBailUnless(data && append);
    cblDataReplaceBytes(data, range, append->buffer, append->length);
}