#include <cable/core/test.h>
#include <cable/core/array.h>
#include <cable/core/string.h>
#include <cable/core/allocator.h>
#include <cable/core/stream.h>

struct CblTest {
    CblConcreteObject isa;
    CblTestMethod method;
    CblString *description;
    CblMutableArray *conditions;
    int failCount;
};

static void finalizeTest(CblTest *test) {
    cblDisown(test->description);
    cblDisown(test->conditions);
}

static CblClass TEST_CLASS = {
        .name = "CblTest",
        .finalizeCallback = (CblObjectFinalizeCallback)finalizeTest,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

const CblClass * const CBL_TEST_CLASS = &TEST_CLASS;

static CblTest *cblTestPrivateNew(CblAllocator *alloc, CblTestRunner *runner, CblTestMethod method, const char *cdescription) {
    cblReturnUnless(runner && method && cdescription, NULL);
    CblMutableArray *conditions = cblMutableArrayNew(alloc, NULL);
    cblReturnUnless(conditions, NULL);
    CblString *description = cblStringNewWithCString(alloc, cdescription);
    if (!description) {
        cblDisown(conditions);
        return NULL;
    }
    CblTest *test = cblAllocatorAllocate(alloc, sizeof(CblTest));
    if (!test) {
        cblDisown(conditions);
        cblDisown(description);
        return NULL;
    }
    cblInitialize(test, alloc, CBL_TEST_CLASS);
    test->description = description;
    test->conditions = conditions;
    test->method = method;
    test->failCount = 0;
    return test;
}

void cblTestEquals(CblTest *it, const char *cdescription, CblObject *expected, CblObject *actual) {
    cblTestTrue(it, cdescription, cblCompare(expected, actual) == 0);
}

void cblTestNotEquals(CblTest *it, const char *cdescription, CblObject *expected, CblObject *actual) {
    cblTestFalse(it, cdescription, cblCompare(expected, actual) == 0);
}

void cblTestTrue(CblTest *it, const char *cdescription, bool condition) {
    cblBailUnless(it && cdescription);
    if (!condition) {
        it->failCount++;
    }
    CblAllocator *alloc = cblGetAllocator(it);
    CblString *description = cblStringNewWithCFormat(alloc, condition ? "✔ it %s" : "✘ it %s", cdescription);
    cblBailUnless(description);
    cblArrayAppend(it->conditions, description);
    cblDisown(description);
}

void cblTestFalse(CblTest *it, const char *cdescription, bool condition) {
    cblTestTrue(it, cdescription, !condition);
}

struct CblTestRunner {
    CblConcreteObject isa;
    CblTestMethod setup;
    CblTestMethod teardown;
    CblMutableArray *tests;
    int failCount;
};

static void finalizeTestRunner(CblTestRunner *runner) {
    cblDisown(runner->tests);
}

static CblClass TEST_RUNNER_CLASS = {
        .name = "CblTest",
        .finalizeCallback = (CblObjectFinalizeCallback)finalizeTestRunner,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

const CblClass * const CBL_TEST_RUNNER_CLASS = &TEST_RUNNER_CLASS;

CblTestRunner *cblTestRunnerNew(CblAllocator *alloc) {
    CblMutableArray *tests = cblMutableArrayNew(alloc, NULL);
    cblReturnUnless(tests, NULL);
    CblTestRunner *runner = cblAllocatorAllocate(alloc, sizeof(CblTestRunner));
    if (!runner) {
        cblDisown(tests);
        return NULL;
    }
    cblInitialize(runner, alloc, CBL_TEST_RUNNER_CLASS);
    runner->tests = tests;
    runner->failCount = 0;
    return runner;
}

void cblTestRunnerSetup(CblTestRunner *runner, CblTestMethod setup) {
    cblBailUnless(runner);
    runner->setup = setup;
}

void cblTestRunnerTeardown(CblTestRunner *runner, CblTestMethod teardown) {
    cblBailUnless(runner);
    runner->teardown = teardown;
}

void cblTestRunnerAdd(CblTestRunner *runner, const char *cdescription, CblTestMethod method) {
    cblBailUnless(runner && method && cdescription);
    CblTest *test = cblTestPrivateNew(cblGetAllocator(runner), runner, method, cdescription);
    cblBailUnless(test);
    cblArrayAppend(runner->tests, test);
    cblDisown(test);
}

typedef struct RunTestData RunTestData;
struct RunTestData {
    CblTestRunner *runner;
    CblOutputStream *stream;
    CblError **error;
};

static bool outputDescription(CblArray *array, CblString *description, size_t index, RunTestData *data) {
    CblTestRunner *runner = data->runner;
    CblOutputStream *stream = data->stream;
    CblError **error = data->error;
    CblAllocator *alloc = cblGetAllocator(runner);
    cblStreamWriteCString(alloc, stream, "  ", error);
    cblReturnIf(error && *error, true);
    cblStringOutput(alloc, description, stream, CBL_STRING_ENCODING_UTF8, error);
    cblReturnIf(error && *error, true);
    cblStreamWriteCString(alloc, stream, "\n", error);
    cblReturnIf(error && *error, true);
    return false;
}

static bool runTest(CblArray *array, CblTest *test, size_t index, RunTestData *data) {
    CblTestRunner *runner = data->runner;
    CblOutputStream *stream = data->stream;
    CblError **error = data->error;
    CblAllocator *alloc = cblGetAllocator(runner);
    CblTestMethod setup = runner->setup;
    if (stream) {
        cblStringOutput(alloc, test->description, stream, CBL_STRING_ENCODING_UTF8, error);
        cblReturnIf(error && *error, true);
        cblStreamWriteCString(alloc, stream, "\n", error);
        cblReturnIf(error && *error, true);
    }
    if (setup) {
        setup(test);
    }
    test->method(test);
    CblTestMethod teardown = runner->teardown;
    if (teardown) {
        runner->teardown(test);
    }
    if (stream) {
        cblReturnIf(cblArrayForeach(test->conditions, (CblArrayForeachFunction) outputDescription, data), true);
        if (index < cblArrayGetLength(array) - 1) {
            cblStreamWriteCString(alloc, stream, "\n", error);
            cblReturnIf(error && *error, true);
        }
    }
    if (test->failCount) {
        runner->failCount++;
    }
    return false;
}

int cblTestRunnerRun(CblTestRunner *runner) {
    return cblTestRunnerRunLog(runner, NULL, NULL);
}

int cblTestRunnerRunLog(CblTestRunner *runner, CblOutputStream *stream, CblError **error) {
    cblReturnUnless(runner, -1);
    RunTestData data = {runner, stream, error};
    cblArrayForeach(runner->tests, (CblArrayForeachFunction)runTest, &data);
    return runner->failCount;
}