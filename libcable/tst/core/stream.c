#include <cable/test.h>

int main() {
    CblTestRunner *runner = cblTestRunnerNew(NULL);

    int status = cblTestRunnerRun(runner);
    cblDisown(runner);
    return status;
}