#include <cable/test.h>
#include <cable/core.h>

CBL_TEST_REPORT {
    cblStringLogTransfer(cblGetString(NULL, cblTestEnv));
}

CBL_TEST(defaultAllocatorIsMalloc) {
    // The NULL allocator is the default/malloc allocator
    size_t bal = cblAllocatorGetAllocationBalance(NULL);
    CblAllocator *allocator = cblAllocatorNew(NULL, NULL);
    cblTestIsTrue(cblAllocatorHasAllocationRecord(NULL, allocator));
    cblTestIsEqual(CBL_ALLOCATOR_DEFAULT, cblGetAllocator(allocator));
    cblTestIsEqual(CBL_ALLOCATOR_MALLOC, cblGetAllocator(allocator));
    cblTestIsTrue(cblAllocatorGetAllocationBalance(NULL) == bal + 1);
    cblDisown(allocator);
    cblTestIsTrue(cblAllocatorGetAllocationBalance(NULL) == bal);
}

CBL_TEST(allocatorsAreRetainedByTheirAllocations) {
    CblAllocator *allocator = cblAllocatorNew(NULL, NULL);
    cblTestIsTrue(cblGetRefCount(allocator) == 1);
    CblScalar *num = cblScalarNewBool(allocator, true);
    cblTestIsTrue(cblGetRefCount(allocator) == 2);
    cblDisown(allocator);
    cblTestIsTrue(cblAllocatorHasAllocationRecord(NULL, allocator));
    cblDisown(num);
    cblTestIsFalse(cblAllocatorHasAllocationRecord(NULL, allocator));
}

CBL_TEST_RUN(
        defaultAllocatorIsMalloc,
        allocatorsAreRetainedByTheirAllocations
);
