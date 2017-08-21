#ifndef CBL_CORE_STREAM_H_
#define CBL_CORE_STREAM_H_

#include <stdio.h>

#include <cable/core/object.h>

CBL_EXTERN_BEGIN

typedef const struct CblData CblData;
typedef struct CblData CblMutableData;
typedef struct CblError CblError;

typedef struct CblStream CblStream;
CblClass * const CBL_STREAM_CLASS;

void cblStreamClose(CblAllocator *alloc, CblStream *stream, CblError **error);

void cblStreamFlush(CblAllocator *alloc, CblStream *stream, CblError **error);

typedef struct CblStream CblInputStream;

CblInputStream *cblInputStreamNew(CblAllocator *alloc, const char *location, CblError **error);

CblInputStream *cblInputStreamNewWithCStream(CblAllocator *alloc, FILE *cstream);

CblInputStream *cblInputStreamNewWithData(CblAllocator *alloc, CblData *data);

size_t cblStreamReadBytes(CblAllocator *alloc, CblInputStream *stream, uint8_t *buffer, size_t length, CblError **error);

typedef struct CblStream CblOutputStream;

CblOutputStream *cblOutputStreamNew(CblAllocator *alloc, const char *location, bool append, CblError **error);

CblOutputStream *cblOutputStreamNewWithCStream(CblAllocator *alloc, FILE *cstream);

CblOutputStream *cblOutputStreamNewWithData(CblAllocator *alloc, CblMutableData *data, bool append);

size_t cblStreamWriteCString(CblAllocator *alloc, CblOutputStream *stream, const char *cstring, CblError **error);

size_t cblStreamWriteCFormat(CblAllocator *alloc, CblOutputStream *stream, const char *format, CblError **error, ...);

size_t cblStreamWriteCFormatList(CblAllocator *alloc, CblOutputStream *stream, const char *format, CblError **error, va_list args);

size_t cblStreamWriteBytes(CblAllocator *alloc, CblOutputStream *stream, const uint8_t *bytes, size_t length, CblError **error);

CBL_EXTERN_END

#endif // CBL_CORE_ERROR_H_