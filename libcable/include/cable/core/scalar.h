#ifndef CBL_CORE_SCALAR_H_
#define CBL_CORE_SCALAR_H_

#include <cable/core/object.h>

typedef struct CblScalar CblScalar;
CblClass * const CBL_SCALAR_CLASS;

CblScalar *cblScalarNewBool(CblAllocator *alloc, bool value);

#define CBL_BOOL(value) cblScalarNewBool(NULL, (value))

bool cblScalarBoolValue(CblScalar *scalar);

CblScalar *cblScalarNewFloat(CblAllocator *alloc, float value);

#define CBL_FLOAT(value) cblScalarNewFloat(NULL, (value))

float cblScalarFloatValue(CblScalar *scalar);

CblScalar *cblScalarNewDouble(CblAllocator *alloc, double value);

#define CBL_DOUBLE(value) cblScalarNewDouble(NULL, (value))

double cblScalarDoubleValue(CblScalar *scalar);

CblScalar *cblScalarNewInt(CblAllocator *alloc, int value);

#define CBL_INT(value) cblScalarNewInt(NULL, (value))

int cblScalarIntValue(CblScalar *scalar);

CblScalar *cblScalarNewUInt(CblAllocator *alloc, unsigned int value);

#define CBL_UINT(value) cblScalarNewUInt(NULL, (value))

unsigned int cblScalarUIntValue(CblScalar *scalar);

CblScalar *cblScalarNewInt8(CblAllocator *alloc, int8_t value);

#define CBL_INT8(value) cblScalarNewInt8(NULL, (value))

int8_t cblScalarInt8Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt8(CblAllocator *alloc, uint8_t value);

#define CBL_UINT8(value) cblScalarNewUInt8(NULL, (value))

uint8_t cblScalarUInt8Value(CblScalar *scalar);

CblScalar *cblScalarNewInt16(CblAllocator *alloc, int16_t value);

#define CBL_INT16(value) cblScalarNewInt16(NULL, (value))

int16_t cblScalarInt16Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt16(CblAllocator *alloc, uint16_t value);

#define CBL_UINT16(value) cblScalarNewUInt16(NULL, (value))

uint16_t cblScalarUInt16Value(CblScalar *scalar);

CblScalar *cblScalarNewInt32(CblAllocator *alloc, int32_t value);

#define CBL_INT32(value) cblScalarNewInt32(NULL, (value))

int32_t cblScalarInt32Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt32(CblAllocator *alloc, uint32_t value);

#define CBL_UINT32(value) cblScalarNewUInt32(NULL, (value))

uint32_t cblScalarUInt32Value(CblScalar *scalar);

CblScalar *cblScalarNewInt64(CblAllocator *alloc, int64_t value);

#define CBL_INT64(value) cblScalarNewInt64(NULL, (value))

int64_t cblScalarInt64Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt64(CblAllocator *alloc, uint64_t value);

#define CBL_UINT64(value) cblScalarNewUInt64(NULL, (value))

uint64_t cblScalarUInt64Value(CblScalar *scalar);

CblScalar *cblScalarNewSize(CblAllocator *alloc, ssize_t value);

#define CBL_SIZE(value) cblScalarNewSize(NULL, (value))

ssize_t cblScalarSizeValue(CblScalar *scalar);

CblScalar *cblScalarNewUSize(CblAllocator *alloc, size_t value);

#define CBL_USIZE(value) cblScalarNewUSize(NULL, (value))

size_t cblScalarUSizeValue(CblScalar *scalar);

CblScalar *cblScalarNewIntPointer(CblAllocator *alloc, intptr_t value);

#define CBL_INTPOINTER(value) cblScalarNewIntPointer(NULL, (value))

intptr_t cblScalarIntPointerValue(CblScalar *scalar);

CblScalar *cblScalarNewUIntPointer(CblAllocator *alloc, uintptr_t value);

#define CBL_UINTPOINTER(value) cblScalarNewUIntPointer(NULL, value)

uintptr_t cblScalarUIntPointerValue(CblScalar *scalar);

CblCmp cblScalarCompare(CblScalar *lhs, CblScalar *rhs);

#endif // CBL_CORE_SCALAR_H_