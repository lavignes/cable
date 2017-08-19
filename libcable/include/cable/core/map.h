#ifndef CBL_CORE_MAP_H_
#define CBL_CORE_MAP_H_

#include <cable/core/object.h>

typedef const struct CblMap CblMap;
typedef struct CblMap CblMutableMap;
CblClass * const CBL_MAP_CLASS;

typedef const void *(*CblMapOwnCallback)(CblMap *map, const void *element);
typedef void (*CblMapDisownCallback)(CblMap *map, const void *element);
typedef CblCmp (*CblMapCompareCallback)(const void *lhs, const void *rhs);
typedef size_t (*CblMapHashCallback)(const void *element);
typedef CblString *(*CblMapStringCallback)(const void *element);
typedef bool (*CblMapForeachFunction)(CblMap *map, const void *key, const void *value, void *userData);

typedef struct CblMapKeyContext {
    CblMapOwnCallback ownCallback;
    CblMapDisownCallback disownCallback;
    CblMapCompareCallback compareCallback;
    CblMapHashCallback hashCallback;
    CblMapStringCallback stringCallback;
} CblMapKeyContext;

typedef struct CblMapValueContext {
    CblMapOwnCallback ownCallback;
    CblMapDisownCallback disownCallback;
    CblMapCompareCallback compareCallback;
    CblMapStringCallback stringCallback;
} CblMapValueContext;

const CblMapKeyContext * const CBL_MAP_KEY_CONTEXT_OBJECTS;
const CblMapKeyContext * const CBL_MAP_KEY_CONTEXT_ALLOCS;

const CblMapValueContext * const CBL_MAP_VALUE_CONTEXT_OBJECTS;
const CblMapValueContext * const CBL_MAP_VALUE_CONTEXT_ALLOCS;

CblMap *cblMapNew(CblAllocator *alloc,
                  const CblMapKeyContext *keyContext,
                  const CblMapValueContext *valueContext,
                  const void **keys,
                  const void **values,
                  size_t length);

CblMap *cblMapNewCopy(CblAllocator *alloc, CblMap *map);

CblMutableMap *cblMutableMapNew(CblAllocator *alloc,
                                const CblMapKeyContext *keyContext,
                                const CblMapValueContext *valueContext);

CblMutableMap *cblMutableMapNewWithElements(CblAllocator *alloc,
                                            const CblMapKeyContext *keyContext,
                                            const CblMapValueContext *valueContext,
                                            const void **keys,
                                            const void **values,
                                            size_t length);

CblMutableMap *cblMutableMapNewCopy(CblAllocator *alloc, CblMap *map);

CblCmp cblMapCompare(CblMap *lhs, CblMap *rhs);

size_t cblMapGetLength(CblMap *map);

const void *cblMapGet(CblMap *map, const void *key);

const void *cblMapGetTransfer(CblMap *map, const void *key);

bool cblMapForeach(CblMap *map, CblMapForeachFunction foreachFunction, void *userData);

void cblMapSet(CblMutableMap *map, const void *key, const void *value);

void cblMapSetTransfer(CblMutableMap *map, const void *key, const void *value);

void cblMapRemove(CblMutableMap *map, const void *key);

void cblMapEmpty(CblMutableMap *map);

#endif // CBL_CORE_MAP_H_