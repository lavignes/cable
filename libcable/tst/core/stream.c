#include <cable/test.h>

int main() {
    autodisown CblTestRunner *runner = cblTestRunnerNew(NULL);

    return cblTestRunnerRun(runner);
}