#ifndef CBL_TEST_H_
#define CBL_TEST_H_

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef void (*CblTest)(CblObject *cblTestEnv);

CblTest CblTestSetup;
CblTest CblTestTeardown;
CblTest CblTestReport;

#define CBL_TEST(name)                                       \
void cblTest_##name (CblObject *cblTestEnv);                 \
void name (CblObject *cblTestEnv) {                          \
    cblTestRun(#name, cblTest_##name, cblTestEnv);           \
}                                                            \
void cblTest_##name (CblObject *cblTestEnv)                  \

#define CBL_TEST_RUN(...)                                    \
int main() {                                                 \
    const CblTest tests[] = {__VA_ARGS__, NULL};             \
    int result = cblTestRunAll(tests);                       \
    cblAllocatorPrintLiveAllocationRecords(NULL, stderr);    \
    return result;                                           \
}                                                            \

#define CBL_TEST_SETUP                                       \
void cblTestSetup_impl(CblObject *cblTestEnv);               \
CblTest CblTestSetup = cblTestSetup_impl;                    \
void cblTestSetup_impl(CblObject *cblTestEnv)                \

#define CBL_TEST_TEARDOWN                                    \
void cblTestTeardown_impl(CblObject *cblTestEnv);            \
CblTest CblTestTeardown = cblTestTeardown_impl;              \
void cblTestTeardown_impl(CblObject *cblTestEnv)             \

#define CBL_TEST_REPORT                                      \
void cblTestReport_impl(CblObject *cblTestEnv);              \
CblTest CblTestReport = cblTestReport_impl;                  \
void cblTestReport_impl(CblObject *cblTestEnv)               \

void cblTestRun(const char *name, CblTest test, CblObject *env);
int cblTestRunAll(const CblTest tests[]);

void cblTestEnvIsTrue(CblObject *cblTestEnv, const char *repr, bool test);
#define cblTestIsTrue(test) cblTestEnvIsTrue(cblTestEnv, #test, (test))

void cblTestEnvIsFalse(CblObject *cblTestEnv, const char *repr, bool test);
#define cblTestIsFalse(test) cblTestEnvIsFalse(cblTestEnv, #test, (test))

void cblTestEnvIsEqual(CblObject *cblTestEnv,
                       const char *reprExpected,
                       const char *reprActual,
                       CblObject *expected,
                       CblObject *actual);

void cblTestEnvIsNotEqual(CblObject *cblTestEnv,
                       const char *reprExpected,
                       const char *reprActual,
                       CblObject *expected,
                       CblObject *actual);

#define cblTestIsEqual(expected, actual) cblTestEnvIsEqual(cblTestEnv, #expected, #actual, (expected), (actual))
#define cblTestIsNotEqual(expected, actual) cblTestEnvIsNotEqual(cblTestEnv, #expected, #actual, (expected), (actual))

CBL_EXTERN_END

#endif // CBL_TEST_H_