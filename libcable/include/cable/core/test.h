#ifndef CBL_TEST_H_
#define CBL_TEST_H_

#include <cable/core/object.h>
#include <cable/core/error.h>
#include <cable/core/stream.h>

CBL_EXTERN_BEGIN

typedef struct CblTest CblTest;
CblClass * const CBL_TEST_CLASS;

typedef void (*CblTestMethod)(CblTest *it);

void cblTestEquals(CblTest *it, const char *cdescription, CblObject *expected, CblObject *actual);

void cblTestNotEquals(CblTest *it, const char *cdescription, CblObject *expected, CblObject *actual);

void cblTestTrue(CblTest *it, const char *cdescription, bool condition);

void cblTestFalse(CblTest *it, const char *cdescription, bool condition);

typedef struct CblTestRunner CblTestRunner;
CblClass * const CBL_TEST_RUNNER_CLASS;

CblTestRunner *cblTestRunnerNew(CblAllocator *alloc);

void cblTestRunnerSetup(CblTestRunner *runner, CblTestMethod setup);

void cblTestRunnerTeardown(CblTestRunner *runner, CblTestMethod teardown);

void cblTestRunnerAdd(CblTestRunner *runner, const char *cdescription, CblTestMethod method);

int cblTestRunnerRun(CblTestRunner *runner);

int cblTestRunnerRunLog(CblTestRunner *runner, CblOutputStream *stream, CblError **error);

CBL_EXTERN_END

#endif // CBL_TEST_H_