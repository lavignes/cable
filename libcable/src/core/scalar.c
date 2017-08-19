#include <math.h>
#include <stdarg.h>

#include <cable/core/allocator.h>
#include <cable/core/scalar.h>
#include <cable/core/string.h>

typedef enum Species {
    BOOL,
    DOUBLE,
    SIZE,
    USIZE,
} Species;

struct CblScalar {
    CblConcreteObject isa;
    Species species;
    union {
        bool boolValue;
        double doubleValue;
        ssize_t sizeValue;
        size_t uSizeValue;
    } as;
};

static size_t hashCallback(CblScalar *scalar) {
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue? 1 : 0;

        case DOUBLE:
            return (size_t)scalar->as.doubleValue;

        case SIZE:
            return (size_t)scalar->as.sizeValue;

        case USIZE:
            return scalar->as.uSizeValue;
    }
    return false;
}

static CblString *stringCallback(CblScalar *scalar) {
    switch (scalar->species) {
        case BOOL:
            return CBL_STR(scalar->as.boolValue? "1" : "0");

        case DOUBLE:
            return cblStringNewFromCFormat(NULL, "%g", scalar->as.doubleValue);

        case SIZE:
            return cblStringNewFromCFormat(NULL, "%zd", scalar->as.doubleValue);

        case USIZE:
            return cblStringNewFromCFormat(NULL, "%zu", scalar->as.doubleValue);
    }
    return CBL_STR("(null)");
}

static CblClass SCALAR_CLASS = {
        .name = "CblScalar",
        .finalizeCallback = NULL,
        .hashCallback = (CblObjectHashCallback)hashCallback,
        .compareCallback = (CblObjectCompareCallback)cblScalarCompare,
        .stringCallback = (CblObjectStringCallback)stringCallback
};

CblClass * const CBL_SCALAR_CLASS = &SCALAR_CLASS;

static CblScalar *createRawNumber(CblAllocator *alloc, Species species, ...) {
    CblScalar *scalar = cblAllocatorAllocate(alloc, sizeof(CblScalar));
    cblReturnUnless(scalar, NULL);
    va_list args;
    va_start(args, species);
    switch (species) {
        case BOOL:
            // bool is promoted to int to preserve alignment on systems that use small booleans
            scalar->as.boolValue = (bool)va_arg(args, int);
            break;

        case DOUBLE:
            scalar->as.doubleValue = va_arg(args, double);
            break;

        case SIZE:
            scalar->as.sizeValue = va_arg(args, ssize_t);
            break;

        case USIZE:
            scalar->as.uSizeValue = va_arg(args, size_t);
            break;
    }
    va_end(args);
    scalar->species = species;
    cblInitialize(scalar, alloc, CBL_SCALAR_CLASS);
    return scalar;
}

CblScalar *cblScalarNewBool(CblAllocator *alloc, bool value) {
    return createRawNumber(alloc, BOOL, value);
}

bool cblScalarBoolValue(CblScalar *scalar) {
    cblReturnUnless(scalar, false);
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue;

        case DOUBLE:
            if (!isfinite(scalar->as.doubleValue)) {
                return false;
            }
            return scalar->as.doubleValue != 0.0;

        case SIZE:
            return scalar->as.sizeValue != 0;

        case USIZE:
            return scalar->as.uSizeValue != 0;
    }
    return false;
}

CblScalar *cblScalarNewFloat(CblAllocator *alloc, float value) {
    return createRawNumber(alloc, DOUBLE, (double)value);
}

float cblScalarFloatValue(CblScalar *scalar) {
    cblReturnUnless(scalar, 0.0f);
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue? 1.0f : 0.0f;

        case DOUBLE:
            return (float)scalar->as.doubleValue;

        case SIZE:
            return scalar->as.sizeValue;

        case USIZE:
            return scalar->as.uSizeValue;
    }
    return 0.0f;
}

CblScalar *cblScalarNewDouble(CblAllocator *alloc, double value) {
    return createRawNumber(alloc, DOUBLE, value);
}

double cblScalarDoubleValue(CblScalar *scalar) {
    cblReturnUnless(scalar, 0.0);
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue? 1.0 : 0.0;

        case DOUBLE:
            return scalar->as.doubleValue;

        case SIZE:
            return scalar->as.sizeValue;

        case USIZE:
            return scalar->as.uSizeValue;
    }
    return 0.0;
}

CblScalar *cblScalarNewInt(CblAllocator *alloc, int value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

int cblScalarIntValue(CblScalar *scalar) {
    return (int)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUInt(CblAllocator *alloc, unsigned int value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

unsigned int cblScalarUIntValue(CblScalar *scalar) {
    return (unsigned int)cblScalarUSizeValue(scalar);
}

CblScalar *cblScalarNewInt8(CblAllocator *alloc, int8_t value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

int8_t cblScalarInt8Value(CblScalar *scalar) {
    return (int8_t)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUInt8(CblAllocator *alloc, uint8_t value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

uint8_t cblScalarUInt8Value(CblScalar *scalar) {
    return (uint8_t)cblScalarUSizeValue(scalar);
}

CblScalar *cblScalarNewInt16(CblAllocator *alloc, int16_t value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

int16_t cblScalarInt16Value(CblScalar *scalar) {
    return (int16_t)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUInt16(CblAllocator *alloc, uint16_t value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

uint16_t cblScalarUInt16Value(CblScalar *scalar) {
    return (uint16_t)cblScalarUSizeValue(scalar);
}

CblScalar *cblScalarNewInt32(CblAllocator *alloc, int32_t value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

int32_t cblScalarInt32Value(CblScalar *scalar) {
    return (int32_t)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUInt32(CblAllocator *alloc, uint32_t value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

uint32_t cblScalarUInt32Value(CblScalar *scalar) {
    return (uint32_t)cblScalarUSizeValue(scalar);
}

CblScalar *cblScalarNewInt64(CblAllocator *alloc, int64_t value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

int64_t cblScalarInt64Value(CblScalar *scalar) {
    return (int64_t)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUInt64(CblAllocator *alloc, uint64_t value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

uint64_t cblScalarUInt64Value(CblScalar *scalar) {
    return (uint64_t)cblScalarUSizeValue(scalar);
}

CblScalar *cblScalarNewSize(CblAllocator *alloc, ssize_t value) {
    return createRawNumber(alloc, SIZE, value);
}

ssize_t cblScalarSizeValue(CblScalar *scalar) {
    cblReturnUnless(scalar, 0);
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue? 1 : 0;

        case DOUBLE:
            return (ssize_t)scalar->as.doubleValue;

        case SIZE:
            return scalar->as.sizeValue;

        case USIZE:
            return (ssize_t)scalar->as.uSizeValue;
    }
    return 0;
}

CblScalar *cblScalarNewUSize(CblAllocator *alloc, size_t value) {
    return createRawNumber(alloc, USIZE, value);
}

size_t cblScalarUSizeValue(CblScalar *scalar) {
    cblReturnUnless(scalar, 0);
    switch (scalar->species) {
        case BOOL:
            return scalar->as.boolValue? 1 : 0;

        case DOUBLE:
            return (size_t)scalar->as.doubleValue;

        case SIZE:
            return (size_t)scalar->as.sizeValue;

        case USIZE:
            return scalar->as.uSizeValue;
    }
    return 0;
}

CblScalar *cblScalarNewIntPointer(CblAllocator *alloc, intptr_t value) {
    return createRawNumber(alloc, SIZE, (ssize_t)value);
}

intptr_t cblScalarIntPointerValue(CblScalar *scalar) {
    return (intptr_t)cblScalarSizeValue(scalar);
}

CblScalar *cblScalarNewUIntPointer(CblAllocator *alloc, uintptr_t value) {
    return createRawNumber(alloc, USIZE, (size_t)value);
}

uintptr_t cblScalarUIntPointerValue(CblScalar *scalar) {
    return (uintptr_t)cblScalarUSizeValue(scalar);
}

CblCmp cblScalarCompare(CblScalar *lhs, CblScalar *rhs) {
    cblReturnUnless(lhs && rhs, CBL_CMP_GREATER);
    switch (lhs->species) {
        case BOOL:
            cblReturnUnless(rhs->species == BOOL, false);
            if (lhs->as.boolValue != rhs->as.boolValue) {
                return CBL_CMP_GREATER;
            }
            break;

        case DOUBLE:
            cblReturnUnless(rhs->species == DOUBLE, false);
            {
                double res = lhs->as.doubleValue - rhs->as.doubleValue;
                if (res > 0) {
                    return CBL_CMP_GREATER;
                }
                if (res < 0) {
                    return CBL_CMP_LESSER;
                }
            }
            break;

        case SIZE:
            if (lhs->as.sizeValue > rhs->as.sizeValue) {
                return CBL_CMP_GREATER;
            } else if (lhs->as.sizeValue < rhs->as.sizeValue) {
                return CBL_CMP_LESSER;
            }
            break;

        case USIZE:
            if (lhs->as.uSizeValue > rhs->as.uSizeValue) {
                return CBL_CMP_GREATER;
            }
            if (lhs->as.uSizeValue < rhs->as.uSizeValue) {
                return CBL_CMP_LESSER;
            }
            break;
    }
    return CBL_CMP_EQUAL;
}