#ifndef CBL_CORE_SET_H_
#define CBL_CORE_SET_H_

#include <cable/core/object.h>

typedef const struct CblSet CblSet;
typedef struct CblSet CblMutableSet;
CblClass * const CBL_SET_CLASS;

typedef const void *(*CblSetOwnCallback)(CblSet *set, const void *element);
typedef void (*CblSetDisownCallback)(CblSet *set, const void *element);
typedef CblCmp (*CblSetCompareCallback)(const void *lhs, const void *rhs);
typedef size_t (*CblSetHashCallback)(const void *element);
typedef CblString *(*CblSetStringCallback)(const void *element);
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

CblCmp cblSetCompare(CblSet *lhs, CblSet *rhs);

size_t cblSetGetLength(CblSet *set);

const void *cblSetGet(CblSet *set, const void *element);

const void *cblSetGetTransfer(CblSet *set, const void *element);

bool cblSetForeach(CblSet *set, CblSetForeachFunction foreachFunction, void *userData);

void cblSetSet(CblMutableSet *set, const void *element);

void cblSetSetTransfer(CblMutableSet *set, const void *element);

void cblSetRemove(CblMutableSet *set, const void *element);

void cblSetEmpty(CblMutableSet *set);

#endif // CBL_CORE_SET_H_