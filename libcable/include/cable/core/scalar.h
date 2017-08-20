#ifndef CBL_CORE_SCALAR_H_
#define CBL_CORE_SCALAR_H_

#include <cable/core/object.h>

typedef struct CblScalar CblScalar;
CblClass * const CBL_SCALAR_CLASS;

CblScalar *cblScalarNewBool(CblAllocator *alloc, bool value);

bool cblScalarBoolValue(CblScalar *scalar);

CblScalar *cblScalarNewFloat(CblAllocator *alloc, float value);

float cblScalarFloatValue(CblScalar *scalar);

CblScalar *cblScalarNewDouble(CblAllocator *alloc, double value);

double cblScalarDoubleValue(CblScalar *scalar);

CblScalar *cblScalarNewInt(CblAllocator *alloc, int value);

int cblScalarIntValue(CblScalar *scalar);

CblScalar *cblScalarNewUInt(CblAllocator *alloc, unsigned int value);

unsigned int cblScalarUIntValue(CblScalar *scalar);

CblScalar *cblScalarNewInt8(CblAllocator *alloc, int8_t value);

int8_t cblScalarInt8Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt8(CblAllocator *alloc, uint8_t value);

uint8_t cblScalarUInt8Value(CblScalar *scalar);

CblScalar *cblScalarNewInt16(CblAllocator *alloc, int16_t value);

int16_t cblScalarInt16Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt16(CblAllocator *alloc, uint16_t value);

uint16_t cblScalarUInt16Value(CblScalar *scalar);

CblScalar *cblScalarNewInt32(CblAllocator *alloc, int32_t value);

int32_t cblScalarInt32Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt32(CblAllocator *alloc, uint32_t value);

uint32_t cblScalarUInt32Value(CblScalar *scalar);

CblScalar *cblScalarNewInt64(CblAllocator *alloc, int64_t value);

int64_t cblScalarInt64Value(CblScalar *scalar);

CblScalar *cblScalarNewUInt64(CblAllocator *alloc, uint64_t value);

uint64_t cblScalarUInt64Value(CblScalar *scalar);

CblScalar *cblScalarNewSize(CblAllocator *alloc, ssize_t value);

ssize_t cblScalarSizeValue(CblScalar *scalar);

CblScalar *cblScalarNewUSize(CblAllocator *alloc, size_t value);

size_t cblScalarUSizeValue(CblScalar *scalar);

CblScalar *cblScalarNewIntPointer(CblAllocator *alloc, intptr_t value);

intptr_t cblScalarIntPointerValue(CblScalar *scalar);

CblScalar *cblScalarNewUIntPointer(CblAllocator *alloc, uintptr_t value);

uintptr_t cblScalarUIntPointerValue(CblScalar *scalar);

#endif // CBL_CORE_SCALAR_H_