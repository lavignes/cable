#ifndef CBL_CORE_ERROR_H_
#define CBL_CORE_ERROR_H_

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef struct CblError CblError;
CblClass * const CBL_ERROR_CLASS;

CblError *cblErrorNew(CblAllocator *alloc, CblString *domain, int code);

CblError *cblErrorNewFromErrno(CblAllocator *alloc, int code);

CblError *cblErrorNewTransfer(CblAllocator *alloc, CblString *domain, int code);

CblError *cblErrorNewWithReason(CblAllocator *alloc, CblString *domain, int code, CblString *reason);

CblError *cblErrorNewWithReasonTransfer(CblAllocator *alloc, CblString *domain, int code, CblString *reason);

CblCmp cblErrorCompare(CblError *lhs, CblError *rhs);

CblString *cblErrorGetDomain(CblError *error);

int cblErrorGetCode(CblError *error);

CblString *cblErrorGetReason(CblError *error);

CBL_EXTERN_END

#endif // CBL_CORE_ERROR_H_