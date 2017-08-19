#include <string.h>

#include <cable/core/string.h>
#include <cable/core/data.h>
#include <cable/core/allocator.h>

static const size_t DEFAULT_FORMAT_BUFFER_SIZE = 16;

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

static CblMutableString *createInterpolatedString(CblAllocator *alloc,
                                                  const char *interp,
                                                  bool transfer,
                                                  va_list args) {
    // TODO: This is a crummy implementation of string interpolation
    cblReturnUnless(interp, NULL);
    CblMutableData *buffer = cblMutableDataNew(alloc, DEFAULT_FORMAT_BUFFER_SIZE);
    cblReturnUnless(buffer, NULL);
    cblDataSetLength(buffer, 0);
    uint8_t *copyStart = (uint8_t *)interp;
    for (uint8_t *iter = copyStart; *iter; iter = (uint8_t *)utf8Next(iter)) {
        // Next thing is a specifier
        if (*iter == '%') {
            // Flush buffer up to this point :)
            if (copyStart < iter) {
                cblDataAppendBytes(buffer, copyStart, (size_t)(iter - copyStart));
            }
            CblObject *obj = va_arg(args, CblObject *);
            CblString *repr = cblGetString(obj);
            if (transfer) {
                cblDisown(obj);
            }
            cblDataAppendBytes(buffer, cblDataGetBytePointer(repr->buffer), cblDataGetLength(repr->buffer));
            cblDisown(repr);
            copyStart = iter + 1;
        } else if (*(iter + 1) == 0 && copyStart < iter) {
            // If we are about to hit the end of the string and haven't flushed
            cblDataAppendBytes(buffer, copyStart, (size_t)(iter - copyStart + 1));
            copyStart = iter;
        }
    }
    CblMutableString *string = cblMutableStringNewWithBytes(alloc,
                                                            cblDataGetBytePointer(buffer),
                                                            cblDataGetLength(buffer));
    cblDisown(buffer);
    return string;
}

static void finalize(CblString *data) {
    cblDisown(data->buffer);
}

static CblString *stringCallback(CblString *string) {
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

static CblClass STRING_CLASS = {
        .name = "CblString",
        .finalizeCallback = (CblObjectFinalizeCallback)finalize,
        .hashCallback = (CblObjectHashCallback)hashCallback,
        .compareCallback = (CblObjectCompareCallback)cblStringCompare,
        .stringCallback = (CblObjectStringCallback)stringCallback
};

CblClass * const CBL_STRING_CLASS = &STRING_CLASS;

CblString *cblStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length) {
    return cblMutableStringNewWithBytes(alloc, bytes, length);
}

CblString *cblStringNewNoCopy(CblAllocator *alloc, const uint8_t *bytes, size_t length) {
    // TODO: String internment?
    return cblMutableStringNewWithBytes(alloc, bytes, length);
}

CblString *cblStringNewFromCString(CblAllocator *alloc, const char *string) {
    return cblMutableStringNewFromCString(alloc, string);
}

CblString *cblStringNewInterp(CblAllocator *alloc, const char *interp, ...) {
    va_list args;
    va_start(args, interp);
    CblString *string = cblMutableStringNewInterpList(alloc, interp, args);
    va_end(args);
    return string;
}

CblString *cblStringNewInterpTransfer(CblAllocator *alloc, const char *interp, ...) {
    va_list args;
    va_start(args, interp);
    CblString *string = cblMutableStringNewInterpListTransfer(alloc, interp, args);
    va_end(args);
    return string;
}

CblString *cblStringNewInterpList(CblAllocator *alloc, const char *interp, va_list args) {
    return cblMutableStringNewInterpList(alloc, interp, args);
}

CblString *cblStringNewInterpListTransfer(CblAllocator *alloc, const char *interp, va_list args) {
    return cblMutableStringNewInterpListTransfer(alloc, interp, args);
}

CblString *cblStringNewCopy(CblAllocator *alloc, CblString *string) {
    return cblMutableStringNewCopy(alloc, string);
}

CblMutableString *cblMutableStringNew(CblAllocator *alloc) {
    return cblMutableStringNewWithBytes(alloc, NULL, 0);
}

CblMutableString *cblMutableStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length) {
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
    return cblMutableStringNewWithBytes(alloc, (const uint8_t *)string, strlen(string));
}

CblMutableString *cblMutableStringNewCopy(CblAllocator *alloc, CblString *string) {
    cblReturnUnless(string, NULL);
    CblData *data = string->buffer;
    return cblMutableStringNewWithBytes(alloc, cblDataGetBytePointer(data), cblDataGetLength(data));
}

CblMutableString *cblMutableStringNewInterp(CblAllocator *alloc, const char *interp, ...) {
    va_list args;
    va_start(args, interp);
    CblMutableString *string = createInterpolatedString(alloc, interp, false, args);
    va_end(args);
    return string;
}

CblMutableString *cblMutableStringNewInterpTransfer(CblAllocator *alloc, const char *interp, ...) {
    va_list args;
    va_start(args, interp);
    CblMutableString *string = createInterpolatedString(alloc, interp, true, args);
    va_end(args);
    return string;
}

CblMutableString *cblMutableStringNewInterpList(CblAllocator *alloc, const char *interp, va_list args) {
    return createInterpolatedString(alloc, interp, false, args);
}

CblMutableString *cblMutableStringNewInterpListTransfer(CblAllocator *alloc, const char *interp, va_list args) {
    return createInterpolatedString(alloc, interp, true, args);
}

CblCmp cblStringCompare(CblString *lhs, CblString *rhs) {
    cblReturnUnless(lhs && rhs, false);
    return cblDataCompare(lhs->buffer, rhs->buffer);
}

size_t cblStringGetLength(CblString *string) {
    cblReturnUnless(string, 0);
    return string->length;
}

char32_t cblStringGet(CblString *string, size_t index) {
    // TODO: Unimplemented
    return 0;
}

bool cblStringForeach(CblString *string, CblStringForeachFunction foreachFunction, void *userData) {
    // TODO: Unimplemented
    return false;
}

void cblStringLog(CblString *string) {
    cblBailUnless(string);
    printf("%s\n", cblDataGetBytePointer(string->buffer));
}

void cblStringLogTransfer(CblString *string) {
    cblBailUnless(string);
    cblStringLog(string);
    cblDisown(string);
}

void cblStringAppend(CblMutableString *string, CblString *append) {
    cblBailUnless(string && append);
    cblDataAppendData(string->buffer, append->buffer);
    string->length += append->length;
}

void cblStringAppendTransfer(CblMutableString *string, CblString *append) {
    cblStringAppend(string, append);
    cblDisown(append);
}

void cblStringAppendBytes(CblMutableString *string, const uint8_t *bytes, size_t length) {
    cblBailUnless(string && bytes);
    cblDataAppendBytes(string->buffer, bytes, length);
    string->length += length;
}

void cblStringAppendCString(CblMutableString *string, const char *cstring) {
    cblBailUnless(string && cstring);
    size_t length = strlen(cstring);
    cblDataAppendBytes(string->buffer, (const uint8_t *)cstring, length);
    string->length += utf8Strnlen((const uint8_t *)cstring, length);
}