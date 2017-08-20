#include <cable/test.h>
#include <cable/core.h>

static CblClass TEST_CLASS = {
        .name = "CblTest",
        .finalizeCallback = NULL,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

const CblClass * const CBL_TEST_CLASS = &TEST_CLASS;

void cblTestEquals(CblTest *test, CblObject *expected, CblObject *actual) {

}

void cblTestNotEquals(CblTest *test, CblObject *expected, CblObject *actual) {

}

void cblTestTrue(CblTest *test, bool condition) {

}

void cblTestFalse(CblTest *test, bool condition) {

}

void cblTestSetUserData(CblTest *test, void *userData) {

}

void *cblTestGetUserData(CblTest *test) {
    return NULL;
}

CblTestRunner *cblTestRunnerNew(CblAllocator *alloc) {
    return (CblTestRunner *)cblStringNewFromCString(alloc, "test string");
}

void cblTestRunnerSetup(CblTestMethod setup) {

}

void cblTestRunnerAdd(CblTestMethod method, const char *cdescription) {

}

int cblTestRunnerRun(CblTestRunner *runner) {
    return 0;
}