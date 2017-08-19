#ifndef CBL_CORE_OBJECT_H_
#define CBL_CORE_OBJECT_H_

#include <stdatomic.h>

#include <cable/porting.h>

CBL_EXTERN_BEGIN

typedef const void CblObject;
typedef const struct CblClass CblClass;
typedef struct CblAllocator CblAllocator;
typedef const struct CblString CblString;

typedef void (*CblObjectFinalizeCallback)(CblObject *obj);
typedef size_t (*CblObjectHashCallback)(CblObject *obj);
typedef CblCmp (*CblObjectCompareCallback)(CblObject *lhs, CblObject *rhs);
typedef CblString *(*CblObjectStringCallback)(CblObject *obj);

struct CblClass {
    const char *name;
    CblObjectFinalizeCallback finalizeCallback;
    CblObjectHashCallback hashCallback;
    CblObjectCompareCallback compareCallback;
    CblObjectStringCallback stringCallback;
};

typedef struct CblConcreteObject CblConcreteObject;
struct CblConcreteObject {
    CblClass *isa;
    CblAllocator *alloc;
    // TODO: move refcounts to a thread-safe hash table. This will enable weak references and improve locality.
    atomic_size_t rc;
};

void cblInitialize(CblObject *obj, CblAllocator *alloc, CblClass *cls);

CblClass *cblGetClass(CblObject *obj);

CblString *cblGetClassName(CblClass *cls);

void cblOwn(CblObject *obj);

void cblDisown(CblObject *obj);

size_t cblGetRefCount(CblObject *obj);

size_t cblGetHash(CblObject *obj);

CblCmp cblCompare(CblObject *lhs, CblObject *rhs);

CblObject *cblOwnInOwner(CblObject *owner, CblObject *obj);

void cblDisownInOwner(CblObject *owner, CblObject *obj);

void cblDeallocateInOwner(CblObject *owner, const void *element);

CblAllocator *cblGetAllocator(CblObject *obj);

CblString *cblGetString(CblObject *obj);

CblString *cblTransferString(CblObject *obj);

CBL_EXTERN_END

#endif // CBL_CORE_OBJECT_H_