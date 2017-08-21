#include <errno.h>
#include <string.h>

#include <cable/core/stream.h>
#include <cable/core/allocator.h>
#include <cable/core/string.h>
#include <cable/core/error.h>
#include <cable/core/data.h>

typedef enum Species Species;
enum Species {
    ISTREAM,
    OSTREAM,
    IDATA,
    ODATA,
};

struct CblStream {
    CblConcreteObject isa;
    Species species;
    union {
        FILE *cstream;
        struct {
            union {
                CblObject *proxy;
                CblData *idata;
                CblMutableData *odata;
            };
            size_t index;
        };
    };
};

static void finalizeCallback(CblStream *stream) {
    switch (stream->species) {
        case IDATA:
        case ODATA:
            cblDisown(stream->proxy);
            break;
        default:
            break;
    }
}

static CblClass STREAM_CLASS = {
        .name = "CblStream",
        .finalizeCallback = (CblObjectFinalizeCallback)finalizeCallback,
        .hashCallback = NULL,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

CblClass * const CBL_STREAM_CLASS = &STREAM_CLASS;

static inline FILE *openStream(CblAllocator *alloc, const char *location, const char *mode, CblError **error) {
    cblReturnUnless(location, NULL);
    FILE *cstream = fopen(location, "rb");
    int err = errno;
    if (!cstream) {
        if (error) {
            *error = cblErrorNewWithErrno(alloc, err);
        }
        return NULL;
    }
    return cstream;
}

void cblStreamClose(CblAllocator *alloc, CblStream *stream, CblError **error) {
    cblBailUnless(stream);
    int code = 0;
    switch (stream->species) {
        case ISTREAM:
        case OSTREAM:
            code = fclose(stream->cstream);
        default:
            break;
    }
    if (code != 0 && error) {
        *error = cblErrorNewWithErrno(alloc, errno);
    }
}

void cblStreamFlush(CblAllocator *alloc, CblStream *stream, CblError **error) {
    cblBailUnless(stream);
    int code = 0;
    switch (stream->species) {
        case ISTREAM:
        case OSTREAM:
            code = fflush(stream->cstream);
        default:
            break;
    }
    if (code != 0 && error) {
        *error = cblErrorNewWithErrno(alloc, errno);
    }
}

CblInputStream *cblInputStreamNew(CblAllocator *alloc, const char *location, CblError **error) {
    FILE *cstream = openStream(alloc, location, "rb", error);
    cblReturnUnless(cstream, NULL);
    CblInputStream *stream = cblInputStreamNewWithCStream(alloc, cstream);
    if (!stream) {
        fclose(cstream);
    }
    return stream;
}

CblInputStream *cblInputStreamNewWithCStream(CblAllocator *alloc, FILE *cstream) {
    cblReturnUnless(cstream, NULL);
    CblInputStream *stream = cblAllocatorAllocate(alloc, sizeof(CblStream));
    cblReturnUnless(stream, NULL);
    cblInitialize(stream, alloc, CBL_STREAM_CLASS);
    stream->species = ISTREAM;
    stream->cstream = cstream;
    return stream;
}

CblInputStream *cblInputStreamNewWithData(CblAllocator *alloc, CblData *data) {
    cblReturnUnless(data, NULL);
    CblInputStream *stream = cblAllocatorAllocate(alloc, sizeof(CblStream));
    cblReturnUnless(stream, NULL);
    cblInitialize(stream, alloc, CBL_STREAM_CLASS);
    cblOwn(data);
    stream->species = IDATA;
    stream->proxy = data;
    stream->index = 0;
    return stream;
}

size_t cblStreamReadBytes(CblAllocator *alloc, CblInputStream *stream, uint8_t *buffer, size_t length, CblError **error) {
    cblReturnUnless(stream && buffer && length, 0);
    switch (stream->species) {
        case ISTREAM: {
            FILE *cstream = stream->cstream;
            clearerr(cstream);
            size_t count = fread(buffer, 1, length, cstream);
            int err = errno;
            if (ferror(cstream)) {
                if (error) {
                    *error = cblErrorNewWithErrno(alloc, err);
                }
                return 0;
            }
            return count;
        }

        case IDATA: {
            CblData *data = stream->idata;
            size_t dataLength = cblDataGetLength(data);
            if (stream->index >= dataLength) {
                if (error) {
                    *error = cblErrorNewWithErrno(alloc, EOVERFLOW);
                }
                return 0;
            }
            size_t count = cblUSizeMin(dataLength - length, dataLength - stream->index);
            cblDataGetBytes(data, buffer, cblRangeNew(stream->index, count));
            stream->index += count;
            return count;
        }

        default:
            break;
    }
    return 0;
}

CblOutputStream *cblOutputStreamNew(CblAllocator *alloc, const char *location, bool append, CblError **error) {
    FILE *cstream = openStream(alloc, location, append ? "wba" : "wb", error);
    cblReturnUnless(cstream, NULL);
    CblOutputStream *stream = cblOutputStreamNewWithCStream(alloc, cstream);
    if (!stream) {
        fclose(cstream);
    }
    return stream;
}

CblOutputStream *cblOutputStreamNewWithCStream(CblAllocator *alloc, FILE *cstream) {
    cblReturnUnless(cstream, NULL);
    CblOutputStream *stream = cblAllocatorAllocate(alloc, sizeof(CblStream));
    cblReturnUnless(stream, NULL);
    cblInitialize(stream, alloc, CBL_STREAM_CLASS);
    stream->species = OSTREAM;
    stream->cstream = cstream;
    return stream;
}

CblOutputStream *cblOutputStreamNewWithData(CblAllocator *alloc, CblMutableData *data, bool append) {
    cblReturnUnless(data, NULL);
    CblOutputStream *stream = cblAllocatorAllocate(alloc, sizeof(CblStream));
    cblReturnUnless(stream, NULL);
    cblInitialize(stream, alloc, CBL_STREAM_CLASS);
    cblOwn(data);
    stream->species = ODATA;
    stream->odata = data;
    if (!append) {
        cblDataSetLength(data, 0);
    }
    return stream;
}

size_t cblStreamWriteCString(CblAllocator *alloc, CblOutputStream *stream, const char *cstring, CblError **error) {
    return cblStreamWriteBytes(alloc, stream, (const uint8_t *)cstring, strlen(cstring), error);
}

size_t cblStreamWriteCFormat(CblAllocator *alloc, CblOutputStream *stream, const char *format, CblError **error, ...) {
    va_list args;
    va_start(args, error);
    size_t written = cblStreamWriteCFormatList(alloc, stream, format, error, args);
    va_end(args);
    return written;
}

size_t cblStreamWriteCFormatList(CblAllocator *alloc, CblOutputStream *stream, const char *format, CblError **error, va_list args) {
    cblReturnUnless(stream && format, 0);
    CblString *string = cblStringNewWithCFormatList(alloc, format, args);
    cblReturnUnless(string, 0);
    size_t written = cblStringOutput(alloc, string, stream, CBL_STRING_ENCODING_UTF8, error);
    cblDisown(string);
    return written;
}

size_t cblStreamWriteBytes(CblAllocator *alloc,
                           CblOutputStream *stream,
                           const uint8_t *bytes,
                           size_t length,
                           CblError **error)
{
    cblReturnUnless(stream && bytes && length, 0);
    switch (stream->species) {
        case OSTREAM: {
            FILE *cstream = stream->cstream;
            clearerr(cstream);
            size_t count = fwrite(bytes, 1, length, cstream);
            int err = errno;
            if (ferror(cstream)) {
                if (error) {
                    *error = cblErrorNewWithErrno(alloc, err);
                }
                return 0;
            }
            return count;
        }

        case ODATA: {
            CblMutableData *data = stream->odata;
            size_t oldLength = cblDataGetLength(data);
            cblDataAppendBytes(data, bytes, length);
            return cblDataGetLength(data) - oldLength;
        }

        default:
            break;
    }
    return 0;
}
