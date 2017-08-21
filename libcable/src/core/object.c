#include <cable/core/allocator.h>
#include <cable/core/string.h>

#define GET(obj) ((CblConcreteObject *)(obj))

void cblOwn(CblObject *obj) {
    cblBailUnless(obj);
    atomic_fetch_add(&GET(obj)->rc, 1);
}

void cblDisown(CblObject *obj) {
    cblBailUnless(obj);
    if (atomic_fetch_add(&GET(obj)->rc, -1) == 1) {
        if (GET(obj)->isa->finalizeCallback) {
            GET(obj)->isa->finalizeCallback(obj);
        }
        // TODO: Clobber a pointer (alloc?) in the object to mark it unusable
        cblAllocatorDeallocate(GET(obj)->alloc, (void *)obj);
    }
}

size_t cblGetRefCount(CblObject *obj) {
    cblReturnUnless(obj, 0);
    return atomic_load(&GET(obj)->rc);
}

size_t cblGetHash(CblObject *obj) {
    cblReturnUnless(obj, 0);
    if (!GET(obj)->isa->hashCallback) {
        return (size_t)(uintptr_t)obj;
    }
    return GET(obj)->isa->hashCallback(obj);
}

bool cblEquals(CblObject *lhs, CblObject *rhs) {
    return cblCompare(lhs, rhs) == 0;
}

int cblCompare(CblObject *lhs, CblObject *rhs) {
    cblReturnUnless(lhs && rhs, -1);
    cblReturnIf(lhs == rhs, 0);
    if (GET(lhs)->isa != GET(rhs)->isa) {
        return -1;
    }
    CblObjectCompareCallback compare = GET(lhs)->isa->compareCallback;
    cblReturnUnless(compare, -1);
    return compare(lhs, rhs);
}

CblObject *cblOwnInOwner(CblObject *owner, CblObject *obj) {
    cblOwn(obj);
    return obj;
}

void cblDisownInOwner(CblObject *owner, CblObject *obj) {
    cblDisown(obj);
}

void cblDeallocateInOwner(CblObject *owner, const void *element) {
    cblAllocatorDeallocate(GET(owner)->alloc, (void *)element);
}

CblClass *cblGetClass(CblObject *obj) {
    return GET(obj)->isa;
}

CblAllocator *cblGetAllocator(CblObject *obj) {
    cblReturnUnless(obj, NULL);
    return GET(obj)->alloc;
}

CblString *cblGetString(CblAllocator *alloc, CblObject *obj) {
    cblReturnUnless(obj, cblStringNewWithCString(alloc, "(null)"));
    if (!alloc) {
        alloc = GET(obj)->alloc;
    }
    CblObjectStringCallback string = GET(obj)->isa->stringCallback;
    if (string) {
        return string(alloc, obj);
    }
    return cblStringNewWithCFormat(alloc, "(%s)", GET(obj)->isa->name);
}

void cblInitialize(CblObject *obj, CblAllocator *alloc, CblClass *cls) {
    GET(obj)->isa = cls;
    GET(obj)->alloc = alloc ?: CBL_ALLOCATOR_DEFAULT;
    atomic_store(&GET(obj)->rc, 1);
}
