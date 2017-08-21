#include <cable/core.h>

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

    cblTestRunnerAdd(runner, "A failing test", alwaysFails);

    cblTestTrue(it, "should return 1", cblTestRunnerRun(runner) == 1);
    cblDisown(runner);
}

static void twoFailingTests(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "A failing test", alwaysFails);
    cblTestRunnerAdd(runner, "Another failing test", alwaysFails);

    cblTestTrue(it, "should return 2", cblTestRunnerRun(runner) == 2);
    cblDisown(runner);
}

static void onePassingTest(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "A passing test", alwaysPasses);

    cblTestTrue(it, "should return 0", cblTestRunnerRun(runner) == 0);
    cblDisown(runner);
}

static void twoPassingTests(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "A passing test", alwaysPasses);
    cblTestRunnerAdd(runner, "Another passing test", alwaysPasses);

    cblTestTrue(it, "should return 0", cblTestRunnerRun(runner) == 0);
    cblDisown(runner);
}

static void failingSetupAndTeardown(CblTest *it) {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerSetup(runner, alwaysFails);
    cblTestRunnerAdd(runner, "A passing test", alwaysPasses);
    cblTestRunnerAdd(runner, "A failing test", alwaysFails);
    cblTestRunnerTeardown(runner, alwaysFails);

    CblMutableData *data = cblMutableDataNew(NULL, 0);
    CblOutputStream *stream = cblOutputStreamNewWithData(NULL, data, false);

    cblTestTrue(it, "should return 2", cblTestRunnerRunLog(runner, stream, NULL) == 2);
    cblDisown(runner);
    cblDisown(stream);

    CblString *log = cblStringNewWithData(NULL, data, CBL_STRING_ENCODING_UTF8);
    cblDisown(data);

    CblString *expectedLog = cblStringNewWithCString(NULL,
                                                     "A passing test\n"
                                                             "  ✘ it always fails\n"
                                                             "  ✔ it always passes\n"
                                                             "  ✘ it always fails\n"
                                                             "\n"
                                                             "A failing test\n"
                                                             "  ✘ it always fails\n"
                                                             "  ✘ it always fails\n"
                                                             "  ✘ it always fails\n"
    );

    cblTestEquals(it, "will have failing setup and teardown in its logs", expectedLog, log);
    cblDisown(log);
    cblDisown(expectedLog);
}

int main() {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    cblTestRunnerAdd(runner, "An empty test runner", emptyRunner);
    cblTestRunnerAdd(runner, "A test runner with one failing test", oneFailingTest);
    cblTestRunnerAdd(runner, "A test runner with two failing tests", twoFailingTests);
    cblTestRunnerAdd(runner, "A test runner with one passing test", onePassingTest);
    cblTestRunnerAdd(runner, "A test runner with two passing tests", twoPassingTests);
    cblTestRunnerAdd(runner, "A test runner with a failing setup and teardown", failingSetupAndTeardown);

    CblOutputStream *stream = cblOutputStreamNewWithCStream(NULL, stderr);
    int status = cblTestRunnerRunLog(runner, stream, NULL);
    cblDisown(stream);

    cblDisown(runner);
    return status;
}