#include <cable/test.h>
#include <cable/core/map.h>
#include <cable/core/string.h>
#include <cable/core.h>

void cblTestRun(const char *name, CblTest test, CblObject *cblTestEnv) {
    CblMutableArray *testEnvs = (CblMutableArray *)cblTestEnv;
    CblMutableMap *testEnv = cblMutableMapNew(NULL, NULL, NULL);
    cblMapSetTransfer(testEnv, CBL_STR("TestName"), CBL_STR(name));
    cblMapSetTransfer(testEnv, CBL_STR("Failures"), cblMutableArrayNew(NULL, NULL));
    if (CblTestSetup) {
        CblTestSetup(testEnv);
    }
    test(testEnv);
    if (CblTestTeardown) {
        CblTestTeardown(testEnv);
    }
    cblArrayAppendTransfer(testEnvs, testEnv);
}

int cblTestRunAll(const CblTest tests[]) {
    CblMutableArray *testEnvs = cblMutableArrayNew(NULL, NULL);
    int failCount = 0;
    for (size_t i = 0; tests[i] != NULL; ++i) {
        tests[i](testEnvs);
        CblMap *testEnv = cblArrayGet(testEnvs, i);
        CblArray *failures = cblMapGetTransfer(testEnv, CBL_STR("Failures"));
        failCount += cblArrayGetLength(failures);
    }
    if (CblTestReport) {
        CblTestReport(testEnvs);
    }
    cblDisown(testEnvs);
    cblAllocatorPrintLiveAllocationRecords(NULL, stdout);
    return failCount;
}

void cblTestEnvIsTrue(CblObject *cblTestEnv, const char *repr, bool test) {
    CblMap *envMap = (CblMap *)cblTestEnv;
    if (!test) {
        CblMutableString *failure = cblMutableStringNewFromCString(NULL, repr);
        CblMutableArray *failures = (CblMutableArray *)cblMapGetTransfer(envMap, CBL_STR("Failures"));
        cblArrayAppendTransfer(failures, failure);
    }
}

void cblTestEnvIsFalse(CblObject *cblTestEnv, const char *repr, bool test) {
    return cblTestEnvIsTrue(cblTestEnv, repr, !test);
}

void cblTestEnvIsEqual(CblObject *cblTestEnv,
                       const char *reprExpected,
                       const char *reprActual,
                       CblObject *expected,
                       CblObject *actual)
{
    CblMap *envMap = (CblMap *)cblTestEnv;
    if (cblCompare(expected, actual) != CBL_CMP_EQUAL) {
        CblMutableString *failure = cblMutableStringNewFromCString(NULL, reprExpected);
        cblStringAppendCString(failure, " != ");
        cblStringAppendCString(failure, reprActual);
        CblMutableArray *failures = (CblMutableArray *)cblMapGetTransfer(envMap, CBL_STR("Failures"));
        cblArrayAppendTransfer(failures, failure);
    }
}

void cblTestEnvIsNotEqual(CblObject *cblTestEnv,
                          const char *reprExpected,
                          const char *reprActual,
                          CblObject *expected,
                          CblObject *actual)
{
    CblMap *envMap = (CblMap *)cblTestEnv;
    if (cblCompare(expected, actual) == CBL_CMP_EQUAL) {
        CblMutableString *failure = cblMutableStringNewFromCString(NULL, reprExpected);
        cblStringAppendCString(failure, " == ");
        cblStringAppendCString(failure, reprActual);
        CblMutableArray *failures = (CblMutableArray *)cblMapGetTransfer(envMap, CBL_STR("Failures"));
        cblArrayAppendTransfer(failures, failure);
    }
}