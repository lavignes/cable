#include <cable/core/allocator.h>
#include <cable/core/string.h>

#define GET(obj) ((CblConcreteObject *)obj)

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
    cblReturnUnless(obj, CBL_NOT_FOUND);
    return atomic_load(&GET(obj)->rc);
}

size_t cblGetHash(CblObject *obj) {
    cblReturnUnless(obj, 0);
    if (!GET(obj)->isa->hashCallback) {
        return (size_t)(uintptr_t)obj;
    }
    return GET(obj)->isa->hashCallback(obj);
}

CblCmp cblCompare(CblObject *lhs, CblObject *rhs) {
    cblReturnUnless(lhs && rhs, CBL_CMP_GREATER);
    cblReturnIf(lhs == rhs, CBL_CMP_EQUAL);
    if (GET(lhs)->isa != GET(rhs)->isa) {
        return CBL_CMP_GREATER;
    }
    cblReturnUnless(GET(lhs)->isa->compareCallback, CBL_CMP_GREATER);
    return GET(lhs)->isa->compareCallback(lhs, rhs);
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

CblString *cblGetString(CblObject *obj) {
    cblReturnUnless(obj, CBL_STR("(null)"));
    if (GET(obj)->isa->stringCallback) {
        return GET(obj)->isa->stringCallback(obj);
    }
    CblMutableString *string = cblMutableStringNewFromCString(NULL, "(");
    cblStringAppendCString(string, GET(obj)->isa->name);
    cblStringAppendCString(string, ")");
    return string;
}

CblString *cblTransferString(CblObject *obj) {
    cblReturnUnless(obj, CBL_STR("(null)"));
    CblString *string = cblGetString(obj);
    cblDisown(obj);
    return string;
}

CblString *cblGetClassName(CblClass *cls) {
    return CBL_STR(cls->name);
}

void cblInitialize(CblObject *obj, CblAllocator *alloc, CblClass *cls) {
    GET(obj)->isa = cls;
    GET(obj)->alloc = alloc ?: CBL_ALLOCATOR_DEFAULT;
    atomic_store(&GET(obj)->rc, 1);
}
