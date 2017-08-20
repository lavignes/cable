#include <cable/core/test.h>

static void alwaysFails(CblTest *it) {
    cblTestTrue(it, "always fails", false);
}

static void alwaysPasses(CblTest *it) {
    cblTestTrue(it, "always passes", true);
}

static void emptyRunner(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestTrue(it, "should return 0", cblTestRunnerRun(runner) == 0);
    cblDisown(runner);
}

static void oneFailingTest(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "a failing test", alwaysFails);

    cblTestTrue(it, "should return 1", cblTestRunnerRun(runner) == 1);
    cblDisown(runner);
}

static void twoFailingTests(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "a failing test", alwaysFails);
    cblTestRunnerAdd(runner, "another failing test", alwaysFails);

    cblTestTrue(it, "should return 2", cblTestRunnerRun(runner) == 2);
    cblDisown(runner);
}

static void onePassingTest(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "a passing test", alwaysPasses);

    cblTestTrue(it, "should return 0", cblTestRunnerRun(runner) == 0);
    cblDisown(runner);
}

static void twoPassingTests(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "a passing test", alwaysPasses);
    cblTestRunnerAdd(runner, "another passing test", alwaysPasses);

    cblTestTrue(it, "should return 0", cblTestRunnerRun(runner) == 0);
    cblDisown(runner);
}

int main() {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "An empty test runner", emptyRunner);
    cblTestRunnerAdd(runner, "A test runner with one failing test", oneFailingTest);
    cblTestRunnerAdd(runner, "A test runner with two failing tests", twoFailingTests);
    cblTestRunnerAdd(runner, "A test runner with one passing test", onePassingTest);
    cblTestRunnerAdd(runner, "A test runner with two passing tests", twoPassingTests);

    CblOutputStream *stream = cblOutputStreamNewFromCStream(NULL, stderr);
    int status = cblTestRunnerRunLog(runner, stream, NULL);
    cblDisown(stream);

    cblDisown(runner);
    return status;
}