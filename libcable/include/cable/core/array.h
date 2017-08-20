#ifndef CBL_CORE_ARRAY_H_
#define CBL_CORE_ARRAY_H_

#include <stdarg.h>

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef const struct CblArray CblArray;
typedef struct CblArray CblMutableArray;
CblClass * const CBL_ARRAY_CLASS;

typedef const void *(*CblArrayOwnCallback)(CblArray *array, const void *element);
typedef void (*CblArrayDisownCallback)(CblArray *array, const void *element);
typedef int (*CblArrayCompareCallback)(const void *lhs, const void *rhs);
typedef CblString *(*CblArrayStringCallback)(CblAllocator *alloc, const void *element);
typedef bool (*CblArrayForeachFunction)(CblArray *array, const void *element, size_t index, void *userData);

typedef struct CblArrayContext {
    CblArrayOwnCallback ownCallback;
    CblArrayDisownCallback disownCallback;
    CblArrayCompareCallback compareCallback;
    CblArrayStringCallback stringCallback;
} CblArrayContext;

const CblArrayContext * const CBL_ARRAY_CONTEXT_OBJECTS;
const CblArrayContext * const CBL_ARRAY_CONTEXT_ALLOCS;

CblArray *cblArrayNew(CblAllocator *alloc, const CblArrayContext *context, const void **elements, size_t length);

CblArray *cblArrayNewCopy(CblAllocator *alloc, CblArray *array);

CblArray *cblArrayNewArgs(CblAllocator *alloc, const CblArrayContext *context, ...);

CblArray *cblArrayNewArgsList(CblAllocator *alloc, const CblArrayContext *context, va_list args);

CblMutableArray *cblMutableArrayNew(CblAllocator *alloc, const CblArrayContext *context);

CblMutableArray *cblMutableArrayNewWithElements(CblAllocator *alloc,
                                                const CblArrayContext *context,
                                                const void **elements,
                                                size_t length);

CblMutableArray *cblMutableArrayNewCopy(CblAllocator *alloc, CblArray *array);

CblMutableArray *cblMutableArrayNewArgs(CblAllocator *alloc, const CblArrayContext *context, ...);

CblMutableArray *cblMutableArrayNewArgsList(CblAllocator *alloc, const CblArrayContext *context, va_list args);

size_t cblArrayGetLength(CblArray *array);

const void *cblArrayGet(CblArray *array, size_t index);

bool cblArrayForeach(CblArray *array, CblArrayForeachFunction foreachFunction, void *userData);

const void *cblArraySet(CblMutableArray *array, size_t index, const void *value);

size_t cblArrayInsert(CblMutableArray *array, size_t index, const void *value);

void cblArrayRemove(CblMutableArray *array, size_t index);

void cblArrayEmpty(CblMutableArray *array);

size_t cblArrayAppend(CblMutableArray *array, const void *value);

CBL_EXTERN_END

#endif // CBL_CORE_ARRAY_H_