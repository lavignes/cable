#include <string.h>

#include <cable/core/string.h>
#include <cable/core/allocator.h>

static const int CONTINUATION_BYTES[256] = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

static CBL_INLINE const uint8_t *utf8Next(const uint8_t *bytes) {
    return bytes + CONTINUATION_BYTES[*bytes];
}

static CBL_INLINE size_t utf8Strnlen(const uint8_t *bytes, size_t size) {
    cblReturnUnless(bytes && size > 0, 0);
    size_t length = 0;
    const uint8_t *start = bytes;
    bytes = utf8Next(bytes);
    while ((size_t)(bytes - start) < size && *bytes) {
        bytes = utf8Next(bytes);
        ++length;
    }
    if ((size_t)(bytes - start) <= size) {
        ++length;
    }
    return length;
}

struct CblString {
    CblConcreteObject isa;
    CblMutableData *buffer;
    size_t length;
    size_t hash;
};

static void finalize(CblString *data) {
    cblDisown(data->buffer);
}

static CblString *stringCallback(CblAllocator *alloc, CblString *string) {
    cblOwn(string);
    return string;
}

static size_t hashCallback(CblMutableString *string) {
    if (!string->hash) {
        size_t hash = 5381;
        const size_t length = cblDataGetLength(string->buffer);
        const uint8_t *buffer = cblDataGetBytePointer(string->buffer);
        for (size_t i = 0; i < length; ++i) {
            hash = (hash << 5) + hash + buffer[i];
        }
        if (hash == 0) {
            hash = 1;
        }
        string->hash = hash;
    }
    return string->hash;
}

static int stringCompare(CblString *lhs, CblString *rhs) {
    cblReturnUnless(lhs && rhs, -1);
    return cblCompare(lhs->buffer, rhs->buffer);
}

static CblClass STRING_CLASS = {
        .name = "CblString",
        .finalizeCallback = (CblObjectFinalizeCallback)finalize,
        .hashCallback = (CblObjectHashCallback)hashCallback,
        .compareCallback = (CblObjectCompareCallback)stringCompare,
        .stringCallback = (CblObjectStringCallback)stringCallback
};

CblClass * const CBL_STRING_CLASS = &STRING_CLASS;

CblString *cblStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length, CblStringEncoding encoding) {
    return cblMutableStringNewWithBytes(alloc, bytes, length, encoding);
}

CblString *cblStringNewFromCString(CblAllocator *alloc, const char *string) {
    return cblMutableStringNewFromCString(alloc, string);
}

CblString *cblStringNewFromCFormat(CblAllocator *alloc, const char *format, ...) {
    va_list args;
    va_start(args, format);
    CblString *string = cblMutableStringNewFromCFormatList(alloc, format, args);
    va_end(args);
    return string;
}

CblString *cblStringNewFromCFormatList(CblAllocator *alloc, const char *format, va_list args) {
    return cblMutableStringNewFromCFormatList(alloc, format, args);
}

CblString *cblStringNewCopy(CblAllocator *alloc, CblString *string) {
    return cblMutableStringNewCopy(alloc, string);
}

CblMutableString *cblMutableStringNew(CblAllocator *alloc) {
    return cblMutableStringNewWithBytes(alloc, NULL, 0, CBL_STRING_ENCODING_UTF8);
}

CblMutableString *cblMutableStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length, CblStringEncoding encoding) {
    CblMutableData *buffer = cblMutableDataNewWithBytes(alloc, bytes, length);
    cblReturnUnless(buffer, NULL);
    CblMutableString *string = cblAllocatorAllocate(alloc, sizeof(CblMutableString));
    if (!string) {
        cblAllocatorDeallocate(alloc, buffer);
        return NULL;
    }
    cblInitialize(string, alloc, &STRING_CLASS);
    string->buffer = buffer;
    string->length = utf8Strnlen(bytes, length);
    string->hash = 0;
    return string;
}

CblMutableString *cblMutableStringNewFromCString(CblAllocator *alloc, const char *string) {
    return cblMutableStringNewWithBytes(alloc, (const uint8_t *)string, strlen(string), CBL_STRING_ENCODING_UTF8);
}

CblMutableString *cblMutableStringNewFromCFormat(CblAllocator *alloc, const char *format, ...) {
    va_list args;
    va_start(args, format);
    CblMutableString *string = cblMutableStringNewFromCFormatList(alloc, format, args);
    va_end(args);
    return string;
}

CblMutableString *cblMutableStringNewFromCFormatList(CblAllocator *alloc, const char *format, va_list args) {
    va_list argscopy;
    va_copy(argscopy, args);
    int count = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    cblReturnUnless(count > 0, NULL);
    size_t size = (size_t)count;
    char *bytes = cblAllocatorAllocate(alloc, size + 1);
    cblReturnUnless(bytes, NULL);
    vsnprintf(bytes, size + 1, format, args);
    CblMutableString *string = cblMutableStringNewWithBytes(alloc, (uint8_t *)bytes, size, CBL_STRING_ENCODING_UTF8);
    cblAllocatorDeallocate(alloc, bytes);
    cblReturnUnless(string, NULL);
    return string;
}

CblMutableString *cblMutableStringNewCopy(CblAllocator *alloc, CblString *string) {
    cblReturnUnless(string, NULL);
    CblData *data = string->buffer;
    return cblMutableStringNewWithBytes(alloc, cblDataGetBytePointer(data), cblDataGetLength(data), CBL_STRING_ENCODING_UTF8);
}

CblData *cblStringGetData(CblAllocator *alloc, CblString *string, CblStringEncoding encoding) {
    return cblStringGetMutableData(alloc, string, encoding);
}

CblMutableData *cblStringGetMutableData(CblAllocator *alloc, CblString *string, CblStringEncoding encoding) {
    cblReturnUnless(string, NULL);
    return cblMutableDataNewCopy(alloc, string->buffer);
}

size_t cblStringOutput(CblAllocator *alloc, CblString *string, CblOutputStream *stream, CblStringEncoding encoding, CblError **error) {
    cblReturnUnless(string && stream, 0);
    CblData *data = string->buffer;
    size_t length = cblDataGetLength(data);
    const uint8_t *bytes = cblDataGetBytePointer(data);
    return cblStreamWriteBytes(alloc, stream, bytes, length, error);
}

char *cblStringGetCString(CblAllocator *alloc, CblString *string) {
    CblData *data = string->buffer;
    size_t length = cblDataGetLength(data);
    uint8_t *bytes = cblAllocatorAllocate(alloc, length);
    cblReturnUnless(bytes, NULL);
    return memcpy(bytes, cblDataGetBytePointer(data), length);
}

size_t cblStringGetLength(CblString *string) {
    cblReturnUnless(string, 0);
    return string->length;
}

size_t cblStringGetSize(CblString *string) {
    cblReturnUnless(string, 0);
    return cblDataGetLength(string->buffer);
}

char32_t cblStringGet(CblString *string, size_t index) {
    // TODO: Unimplemented
    return 0;
}

bool cblStringForeach(CblString *string, CblStringForeachFunction foreachFunction, void *userData) {
    // TODO: Unimplemented
    return false;
}

void cblStringEmpty(CblMutableString *string) {
    cblBailUnless(string);
    cblDataSetLength(string->buffer, 0);
    string->length = 0;
    string->hash = 0;
}

void cblStringAppend(CblMutableString *string, CblString *append) {
    cblBailUnless(string && append);
    cblDataAppendData(string->buffer, append->buffer);
    string->length += append->length;
    string->hash = 0;
}

void cblStringAppendBytes(CblMutableString *string, const uint8_t *bytes, size_t length) {
    cblBailUnless(string && bytes);
    cblDataAppendBytes(string->buffer, bytes, length);
    string->length += length;
    string->hash = 0;
}

void cblStringAppendCString(CblMutableString *string, const char *cstring) {
    cblBailUnless(string && cstring);
    size_t length = strlen(cstring);
    cblDataAppendBytes(string->buffer, (const uint8_t *)cstring, length);
    string->length += utf8Strnlen((const uint8_t *)cstring, length);
    string->hash = 0;
}

void cblStringAppendCFormat(CblMutableString *string, const char *format, ...) {
    va_list args;
    va_start(args, format);
    cblStringAppendCFormatList(string, format, args);
    va_end(args);
}

void cblStringAppendCFormatList(CblMutableString *string, const char *format, va_list args) {
    cblBailUnless(string && format);
    CblString *temp = cblStringNewFromCFormatList(cblGetAllocator(string), format, args);
    cblStringAppend(string, temp);
    cblDisown(temp);
}