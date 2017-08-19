#include <errno.h>

#include <cable/core/error.h>
#include <cable/core/string.h>
#include <cable/core/allocator.h>

struct CblError {
    CblConcreteObject isa;
    CblString *domain;
    int code;
    CblString *reason;
};

static void finalizeCallback(CblError *error) {
    cblBailUnless(error);
    cblDisown(error->domain);
    cblDisown(error->reason);
}

static CblString *stringCallback(CblError *error) {
    cblReturnUnless(error, NULL);
    CblMutableString *string = cblMutableStringNewCopy(NULL, error->domain);
    cblStringAppendCFormat(string, ": (%d)", error->code);
    if (error->reason) {
        cblStringAppendCString(string, " ");
        cblStringAppend(string, error->reason);
    }
    return string;
}

static CblClass ERROR_CLASS = {
        .name = "CblError",
        .finalizeCallback = (CblObjectFinalizeCallback)finalizeCallback,
        .hashCallback = NULL,
        .compareCallback = (CblObjectCompareCallback)cblErrorCompare,
        .stringCallback = (CblObjectStringCallback)stringCallback,
};

CblClass * const CBL_ERROR_CLASS = &ERROR_CLASS;

CblError *cblErrorNew(CblAllocator *alloc, CblString *domain, int code) {
    return cblErrorNewWithReason(alloc, domain, code, NULL);
}

CblError *cblErrorNewFromErrno(CblAllocator *alloc, int code) {
    CblString *reason = cblStringNewFromCString(alloc, strerror(code));
    cblReturnUnless(reason, NULL);
    return cblErrorNewWithReasonTransfer(alloc, NULL, code, reason);
}

CblError *cblErrorNewTransfer(CblAllocator *alloc, CblString *domain, int code) {
    return cblErrorNewWithReasonTransfer(alloc, domain, code, NULL);
}

CblError *cblErrorNewWithReason(CblAllocator *alloc, CblString *domain, int code, CblString *reason) {
    CblError *error = cblAllocatorAllocate(alloc, sizeof(CblError));
    cblReturnUnless(error, NULL);
    cblInitialize(error, alloc, &ERROR_CLASS);
    error->domain = domain ?: cblStringNewFromCString(alloc, "Error");
    error->code = code;
    error->reason = reason;
    return error;
}

CblError *cblErrorNewWithReasonTransfer(CblAllocator *alloc, CblString *domain, int code, CblString *reason) {
    CblError *error = cblErrorNewWithReason(alloc, domain, code, reason);
    cblDisown(domain);
    cblDisown(reason);
    return error;
}

CblCmp cblErrorCompare(CblError *lhs, CblError *rhs) {
    cblReturnUnless(lhs && rhs, CBL_CMP_GREATER);
    CblCmp domainCmp = cblStringCompare(lhs->domain, rhs->domain);
    if (domainCmp != CBL_CMP_EQUAL) {
        return CBL_CMP_GREATER;
    }
    if (lhs->code != rhs->code) {
        return CBL_CMP_GREATER;
    }
    return CBL_CMP_EQUAL;
}

CblString *cblErrorGetDomain(CblError *error) {
    cblReturnUnless(error, NULL);
    return error->domain;
}

int cblErrorGetCode(CblError *error) {
    cblReturnUnless(error, 0);
    return error->code;
}

CblString *cblErrorGetReason(CblError *error) {
    cblReturnUnless(error, NULL);
    return error->reason;
}