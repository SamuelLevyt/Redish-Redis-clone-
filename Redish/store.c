#include "store.h"

#include <string.h>

void store_init(Store *store) {
    if (store == NULL) {
        return;
    }
    store->count = 0;
}

int store_find(const Store *store, const char *key) {
    if (store == NULL || key == NULL) {
        return -1;
    }
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

bool store_set(Store *store, const char *key, const char *value) {
    if (store == NULL || key == NULL || value == NULL) {
        return false;
    }
    if (strlen(key) >= STORE_KEY_SIZE || strlen(value) >= STORE_VALUE_SIZE) {
        return false;
    }

    int index = store_find(store, key);
    if (index < 0) {
        if (store->count >= STORE_MAX_ENTRIES) {
            return false;
        }
        index = store->count;
        store->count++;
        strcpy(store->entries[index].key, key);
    }
    strcpy(store->entries[index].value, value);
    return true;
}

const char *store_get(const Store *store, const char *key) {
    int index = store_find(store, key);
    if (index < 0) {
        return NULL;
    }
    return store->entries[index].value;
}

bool store_del(Store *store, const char *key) {
    int index = store_find(store, key);
    if (index < 0) {
        return false;
    }
    /* Compact the array so lookups stay contiguous. */
    for (int i = index; i < store->count - 1; i++) {
        store->entries[i] = store->entries[i + 1];
    }
    store->count--;
    return true;
}
