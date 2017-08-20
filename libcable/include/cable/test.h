#ifndef CBL_TEST_H_
#define CBL_TEST_H_

#include <cable/core/object.h>
#include <cable/core/error.h>

CBL_EXTERN_BEGIN

typedef struct CblTest CblTest;
CblClass * const CBL_TEST_CLASS;

typedef void (*CblTestMethod)(CblTest *test, CblError **error);

void cblTestEquals(CblTest *test, CblObject *expected, CblObject *actual);

void cblTestNotEquals(CblTest *test, CblObject *expected, CblObject *actual);

void cblTestTrue(CblTest *test, bool condition);

void cblTestFalse(CblTest *test, bool condition);

void cblTestSetUserData(CblTest *test, void *userData);

void *cblTestGetUserData(CblTest *test);

typedef struct CblTestRunner CblTestRunner;
CblClass * const CBL_TEST_RUNNER_CLASS;

CblTestRunner *cblTestRunnerNew(CblAllocator *alloc);

void cblTestRunnerSetup(CblTestMethod setup);

void cblTestRunnerAdd(CblTestMethod method, const char *cdescription);

int cblTestRunnerRun(CblTestRunner *runner);

CBL_EXTERN_END

#endif // CBL_TEST_H_