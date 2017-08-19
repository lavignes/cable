#ifndef CBL_CORE_STRING_H_
#define CBL_CORE_STRING_H_

#include <stdarg.h>
#include <string.h>

#include <cable/core/object.h>

typedef const struct CblString CblString;
typedef struct CblString CblMutableString;
CblClass * const CBL_STRING_CLASS;

typedef bool (*CblStringForeachFunction)(CblString *string, char32_t uniChar, size_t index, void *userData);

#define CBL_STR(bytes) cblStringNewNoCopy(NULL, (const uint8_t *)(bytes), strlen(bytes))

CblString *cblStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length);

CblString *cblStringNewNoCopy(CblAllocator *alloc, const uint8_t *bytes, size_t length);

CblString *cblStringNewFromCString(CblAllocator *alloc, const char *string);

CblString *cblStringNewInterp(CblAllocator *alloc, const char *interp, ...);

CblString *cblStringNewInterpTransfer(CblAllocator *alloc, const char *interp, ...);

CblString *cblStringNewInterpList(CblAllocator *alloc, const char *interp, va_list args);

CblString *cblStringNewInterpListTransfer(CblAllocator *alloc, const char *interp, va_list args);

CblString *cblStringNewCopy(CblAllocator *alloc, CblString *string);

CblMutableString *cblMutableStringNew(CblAllocator *alloc);

CblMutableString *cblMutableStringNewWithBytes(CblAllocator *alloc, const uint8_t *bytes, size_t length);

CblMutableString *cblMutableStringNewFromCString(CblAllocator *alloc, const char *string);

CblMutableString *cblMutableStringNewCopy(CblAllocator *alloc, CblString *string);

CblMutableString *cblMutableStringNewInterp(CblAllocator *alloc, const char *interp, ...);

CblMutableString *cblMutableStringNewInterpTransfer(CblAllocator *alloc, const char *interp, ...);

CblMutableString *cblMutableStringNewInterpList(CblAllocator *alloc, const char *interp, va_list args);

CblMutableString *cblMutableStringNewInterpListTransfer(CblAllocator *alloc, const char *interp, va_list args);

CblCmp cblStringCompare(CblString *lhs, CblString *rhs);

size_t cblStringGetLength(CblString *string);

char32_t cblStringGet(CblString *string, size_t index);

bool cblStringForeach(CblString *string, CblStringForeachFunction foreachFunction, void *userData);

void cblStringLog(CblString *string);

void cblStringLogTransfer(CblString *string);

void cblStringAppend(CblMutableString *string, CblString *append);

void cblStringAppendTransfer(CblMutableString *string, CblString *append);

void cblStringAppendBytes(CblMutableString *string, const uint8_t *bytes, size_t length);

void cblStringAppendCString(CblMutableString *string, const char *cstring);

#endif // CBL_CORE_STRING_H_