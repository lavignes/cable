#ifndef CBL_CORE_SET_H_
#define CBL_CORE_SET_H_

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef const struct CblSet CblSet;
typedef struct CblSet CblMutableSet;
CblClass * const CBL_SET_CLASS;

typedef const void *(*CblSetOwnCallback)(CblSet *set, const void *element);
typedef void (*CblSetDisownCallback)(CblSet *set, const void *element);
typedef int (*CblSetCompareCallback)(const void *lhs, const void *rhs);
typedef size_t (*CblSetHashCallback)(const void *element);
typedef CblString *(*CblSetStringCallback)(CblAllocator *alloc, const void *element);
typedef bool (*CblSetForeachFunction)(CblSet *set, const void *element, void *userData);

typedef struct CblSetContext {
    CblSetOwnCallback ownCallback;
    CblSetDisownCallback disownCallback;
    CblSetCompareCallback compareCallback;
    CblSetHashCallback hashCallback;
    CblSetStringCallback stringCallback;
} CblSetContext;

const CblSetContext * const CBL_SET_CONTEXT_OBJECTS;
const CblSetContext * const CBL_SET_CONTEXT_ALLOCS;

CblSet *cblSetNew(CblAllocator *alloc, const CblSetContext *context, const void **elements, size_t length);

CblSet *cblSetNewCopy(CblAllocator *alloc, CblSet *set);

CblMutableSet *cblMutableSetNew(CblAllocator *alloc, const CblSetContext *context);

CblMutableSet *cblMutableSetNewWithElements(CblAllocator *alloc,
                                            const CblSetContext *context,
                                            const void **elements,
                                            size_t length);

CblMutableSet *cblMutableSetNewCopy(CblAllocator *alloc, CblSet *set);

size_t cblSetGetLength(CblSet *set);

const void *cblSetGet(CblSet *set, const void *element);

bool cblSetForeach(CblSet *set, CblSetForeachFunction foreachFunction, void *userData);

void cblSetSet(CblMutableSet *set, const void *element);

void cblSetRemove(CblMutableSet *set, const void *element);

void cblSetEmpty(CblMutableSet *set);

CBL_EXTERN_END

#endif // CBL_CORE_SET_H_