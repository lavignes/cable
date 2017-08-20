#ifndef CBL_CORE_STRING_H_
#define CBL_CORE_STRING_H_

#include <stdarg.h>
#include <string.h>

#include <cable/core/object.h>
#include <cable/core/data.h>

typedef enum CblStringEncoding CblStringEncoding;
enum CblStringEncoding {
    CBL_STRING_ENCODING_UTF8
};

typedef const struct CblString CblString;
typedef struct CblString CblMutableString;
CblClass * const CBL_STRING_CLASS;

typedef bool (*CblStringForeachFunction)(CblString *string, char32_t uniChar, size_t index, void *userData);

#define CBL_STR(bytes) cblStringNewWithBytes(NULL, (const uint8_t *)(bytes), strlen(bytes), CBL_STRING_ENCODING_UTF8)

CblString *cblStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length, CblStringEncoding encoding);

CblString *cblStringNewFromCString(CblAllocator *alloc, const char *string);

CblString *cblStringNewFromCFormat(CblAllocator *alloc, const char *format, ...);

CblString *cblStringNewFromCFormatList(CblAllocator *alloc, const char *format, va_list args);

CblString *cblStringNewCopy(CblAllocator *alloc, CblString *string);

CblMutableString *cblMutableStringNew(CblAllocator *alloc);

CblMutableString *cblMutableStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length, CblStringEncoding encoding);

CblMutableString *cblMutableStringNewFromCString(CblAllocator *alloc, const char *string);

CblMutableString *cblMutableStringNewFromCFormat(CblAllocator *alloc, const char *format, ...);

CblMutableString *cblMutableStringNewFromCFormatList(CblAllocator *alloc, const char *format, va_list args);

CblMutableString *cblMutableStringNewCopy(CblAllocator *alloc, CblString *string);

CblData *cblStringGetData(CblAllocator *alloc, CblString *string, CblStringEncoding encoding);

CblMutableData *cblStringGetMutableData(CblAllocator *alloc, CblString *string, CblStringEncoding encoding);

CblCmp cblStringCompare(CblString *lhs, CblString *rhs);

size_t cblStringGetLength(CblString *string);

size_t cblStringGetSize(CblString *string);

char32_t cblStringGet(CblString *string, size_t index);

bool cblStringForeach(CblString *string, CblStringForeachFunction foreachFunction, void *userData);

void cblStringEmpty(CblMutableString *string);

void cblStringAppend(CblMutableString *string, CblString *append);

void cblStringAppendBytes(CblMutableString *string, const uint8_t *bytes, size_t length);

void cblStringAppendCString(CblMutableString *string, const char *cstring);

void cblStringAppendCFormat(CblMutableString *string, const char *format, ...);

void cblStringAppendCFormatList(CblMutableString *string, const char *format, va_list args);

#endif // CBL_CORE_STRING_H_