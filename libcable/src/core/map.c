#include <cable/core/map.h>
#include <cable/core/allocator.h>
#include <cable/core/set.h>
#include <cable/core/string.h>

static CblMapKeyContext OBJECT_KEY_CONTEXT = {
        .ownCallback = (CblMapOwnCallback)cblOwnInOwner,
        .disownCallback = (CblMapDisownCallback)cblDisownInOwner,
        .compareCallback = cblCompare,
        .hashCallback = cblGetHash,
        .stringCallback = cblGetString,
};

const CblMapKeyContext * const CBL_MAP_KEY_CONTEXT_OBJECTS = &OBJECT_KEY_CONTEXT;

static void dealloc(CblMap *map, const void *ptr) {
    cblAllocatorDeallocate(cblGetAllocator(map), (void *)ptr);
}

static CblMapKeyContext ALLOC_KEY_CONTEXT = {
        .ownCallback = NULL,
        .disownCallback = dealloc,
        .compareCallback = NULL,
        .hashCallback = NULL,
        .stringCallback = NULL,
};

const CblMapKeyContext * const CBL_MAP_KEY_CONTEXT_ALLOCS = &ALLOC_KEY_CONTEXT;

static CblMapValueContext OBJECT_VALUE_CONTEXT = {
        .ownCallback = (CblMapOwnCallback)cblOwnInOwner,
        .disownCallback = (CblMapDisownCallback)cblDisownInOwner,
        .compareCallback = cblCompare,
        .stringCallback = cblGetString,
};

const CblMapValueContext * const CBL_MAP_VALUE_CONTEXT_OBJECTS = &OBJECT_VALUE_CONTEXT;

static CblMapValueContext ALLOC_VALUE_CONTEXT = {
        .ownCallback = NULL,
        .disownCallback = dealloc,
        .compareCallback = NULL,
        .stringCallback = NULL,
};

const CblMapValueContext * const CBL_MAP_VALUE_CONTEXT_ALLOCS = &ALLOC_VALUE_CONTEXT;

struct CblMap {
    CblConcreteObject isa;
    CblMapKeyContext keyContext;
    CblMapValueContext valueContext;
    CblMutableSet *buffer;
};

static void finalize(CblMutableMap *map) {
    cblDisown(map->buffer);
}

typedef struct StringForeachUserData {
    CblAllocator *alloc;
    CblMutableString *string;
    const CblMapStringCallback keyCallback;
    const CblMapStringCallback valueCallback;
    size_t index;
} StringForeachUserData;

static bool stringForeach(CblMap *map, const void *key, const void *value, StringForeachUserData *userData) {
    CblAllocator *alloc = userData->alloc;
    CblString *keyString = userData->keyCallback(alloc, key);
    cblStringAppend(userData->string, keyString);
    cblDisown(keyString);
    cblStringAppendCString(userData->string, ": ");
    CblString *valueString = userData->valueCallback(alloc, value);
    cblStringAppend(userData->string, valueString);
    cblDisown(valueString);
    if (userData->index < cblSetGetLength(map->buffer) - 1) {
        cblStringAppendCString(userData->string, ", ");
    }
    userData->index++;
    return false;
}

static CblString *stringCallback(CblAllocator *alloc, CblMutableMap *map) {
    cblReturnUnless(map, NULL);
    const CblMapKeyContext *keyContext = &map->keyContext;
    const CblMapValueContext *valueContext = &map->valueContext;
    cblReturnUnless(keyContext->stringCallback && valueContext->stringCallback, NULL);
    CblMutableString *string = cblMutableStringNewFromCString(alloc, "{");
    StringForeachUserData userData = {alloc, string, keyContext->stringCallback, valueContext->stringCallback, 0};
    cblMapForeach(map, (CblMapForeachFunction)stringForeach, &userData);
    cblStringAppendCString(string, "}");
    return string;
}

static CblClass MAP_CLASS = {
        .name = "CblMap",
        .finalizeCallback = (CblObjectFinalizeCallback)finalize,
        .compareCallback = NULL,
        .hashCallback = NULL,
        .stringCallback = (CblObjectStringCallback)stringCallback,
};

CblClass * const CBL_MAP_CLASS = &MAP_CLASS;

typedef struct Pair {
    CblMap *map;
    const void *key;
    const void *value;
} Pair;

static void releasePair(CblSet *set, const Pair *pair) {
    const CblMapKeyContext *keyContext = &pair->map->keyContext;
    const CblMapValueContext *valueContext = &pair->map->valueContext;
    if (keyContext->disownCallback) {
        keyContext->disownCallback(pair->map, pair->key);
    }
    if (valueContext->disownCallback) {
        valueContext->disownCallback(pair->map, pair->value);
    }
    cblAllocatorDeallocate(cblGetAllocator(pair->map), (void *)pair);
}

static int comparePair(const Pair *lhs, const Pair *rhs) {
    const CblMapKeyContext *keyContext = &lhs->map->keyContext;
    if (keyContext->compareCallback) {
        return keyContext->compareCallback(lhs->key, rhs->key);
    }
    if (lhs->key != rhs->key) {
        return -1;
    }
    return 0;
}

static size_t hashPair(const Pair *pair) {
    const CblMapKeyContext *keyContext = &pair->map->keyContext;
    if (keyContext->hashCallback) {
        return keyContext->hashCallback(pair->key);
    }
    return (size_t)(uintptr_t)pair->key;
}

static CblSetContext PAIR_CONTEXT = {
        .ownCallback = NULL,
        .disownCallback = (CblSetDisownCallback)releasePair,
        .compareCallback = (CblSetCompareCallback)comparePair,
        .hashCallback = (CblSetHashCallback)hashPair
};

CblMap *cblMapNew(CblAllocator *alloc,
                  const CblMapKeyContext *keyContext,
                  const CblMapValueContext *valueContext,
                  const void **keys,
                  const void **values, size_t length)
{
    return cblMutableMapNewWithElements(alloc, keyContext, valueContext, keys, values, length);
}

CblMap *cblMapNewCopy(CblAllocator *alloc, CblMap *map) {
    return cblMutableMapNewCopy(alloc, map);
}

CblMutableMap *cblMutableMapNew(CblAllocator *alloc, const CblMapKeyContext *keyContext, const CblMapValueContext *valueContext) {
    return cblMutableMapNewWithElements(alloc, keyContext, valueContext, NULL, NULL, 0);
}

CblMutableMap *cblMutableMapNewWithElements(CblAllocator *alloc,
                                            const CblMapKeyContext *keyContext,
                                            const CblMapValueContext *valueContext,
                                            const void **keys,
                                            const void **values,
                                            size_t length)
{
    if (!keyContext) {
        keyContext = CBL_MAP_KEY_CONTEXT_OBJECTS;
    }
    if (!valueContext) {
        valueContext = CBL_MAP_VALUE_CONTEXT_OBJECTS;
    }
    CblMutableSet *buffer = cblMutableSetNew(alloc, &PAIR_CONTEXT);
    cblReturnUnless(buffer, NULL);
    CblMutableMap *map = cblAllocatorAllocate(alloc, sizeof(CblMutableMap));
    if (!map) {
        cblAllocatorDeallocate(alloc, buffer);
        return NULL;
    }
    cblInitialize(map, alloc, &MAP_CLASS);
    map->keyContext = *keyContext;
    map->valueContext = *valueContext;
    map->buffer = buffer;
    if (keys && values) {
        for (size_t i = 0; i < length; ++i) {
            cblMapSet(map, keys[i], values[i]);
        }
    }
    return map;
}

static bool mapCopyForeachFunction(CblSet *set, const Pair *pair, CblMutableMap *map) {
    cblMapSet(map, pair->key, pair->value);
    return false;
}

CblMutableMap *cblMutableMapNewCopy(CblAllocator *alloc, CblMap *map) {
    cblReturnUnless(map, NULL);
    CblMutableMap *newMap = cblMutableMapNew(alloc, &map->keyContext, &map->valueContext);
    cblReturnUnless(newMap, NULL);
    cblSetForeach(map->buffer, (void *)mapCopyForeachFunction, newMap);
    return newMap;
}

size_t cblMapGetLength(CblMap *map) {
    cblReturnUnless(map, 0);
    return cblSetGetLength(map->buffer);
}

const void *cblMapGet(CblMap *map, const void *key) {
    cblReturnUnless(map && key, NULL);
    const Pair getPair = {map, key, NULL};
    const Pair *pair = cblSetGet(map->buffer, &getPair);
    cblReturnUnless(pair, NULL);
    return pair->value;
}

typedef struct ForeachUserData {
    CblMapForeachFunction foreachFunction;
    void *userData;
} ForeachUserData;

bool pairwiseForeach(const CblSet* set, const Pair *pair, ForeachUserData *userData) {
    return userData->foreachFunction(pair->map, pair->key, pair->value, userData->userData);
}

bool cblMapForeach(CblMap *map, CblMapForeachFunction foreachFunction, void *userData) {
    cblReturnUnless(map && foreachFunction, false);
    ForeachUserData fatUserData = {foreachFunction, userData};
    return cblSetForeach(map->buffer, (void *)pairwiseForeach, &fatUserData);
}

void cblMapSet(CblMutableMap *map, const void *key, const void *value) {
    cblBailUnless(map && key && value);
    CblAllocator *alloc = cblGetAllocator(map);
    Pair *pair = cblAllocatorAllocate(alloc, sizeof(Pair));
    cblBailUnless(pair);
    const CblMapKeyContext *keyContext = &map->keyContext;
    const CblMapValueContext *valueContext = &map->valueContext;
    pair->map = map;
    pair->key = keyContext->ownCallback ? keyContext->ownCallback(map, key) : key;
    pair->value = valueContext->ownCallback ? valueContext->ownCallback(map, value) : value;
    cblSetSet(map->buffer, pair);
}

void cblMapRemove(CblMutableMap *map, const void *key) {
    cblBailUnless(map && key);
    const Pair getPair = {map, key, NULL};
    cblSetRemove(map->buffer, &getPair);
}

void cblMapEmpty(CblMutableMap *map) {
    cblBailUnless(map);
    cblSetEmpty(map->buffer);
}