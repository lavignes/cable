#ifndef CBL_CORE_DATA_H_
#define CBL_CORE_DATA_H_

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef const struct CblData CblData;
typedef struct CblData CblMutableData;
CblClass * const CBL_DATA_CLASS;

CblData *cblDataNew(CblAllocator *alloc, const uint8_t *bytes, size_t length);

CblData *cblDataNewCopy(CblAllocator *alloc, CblData *data);

CblMutableData *cblMutableDataNew(CblAllocator *alloc, size_t length);

CblMutableData *cblMutableDataNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length);

CblMutableData *cblMutableDataNewCopy(CblAllocator *alloc, CblData *data);

size_t cblDataGetSize(CblData *data);

size_t cblDataGetLength(CblData *data);

void cblDataGetBytes(CblData *data, uint8_t *buffer, CblRange range);

const uint8_t *cblDataGetBytePointer(CblData *data);

uint8_t *cblDataGetMutableBytePointer(CblMutableData *data);

void cblDataSetSize(CblMutableData *data, size_t size);

void cblDataSetLength(CblMutableData *data, size_t length);

void cblDataAppendBytes(CblMutableData *data, const uint8_t *bytes, size_t length);

void cblDataReplaceBytes(CblMutableData *data, CblRange range, const uint8_t *bytes, size_t length);

void cblDataAppendData(CblMutableData *data, CblData *append);

void cblDataReplaceData(CblMutableData *data, CblRange range, CblData *append);

CBL_EXTERN_END

#endif // CBL_CORE_DATA_H_