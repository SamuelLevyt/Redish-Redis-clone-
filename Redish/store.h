#ifndef STORE_H
#define STORE_H

#include <stdbool.h>

#define STORE_MAX_ENTRIES 100
#define STORE_KEY_SIZE 100
#define STORE_VALUE_SIZE 100

struct data {
    char key[STORE_KEY_SIZE];
    char value[STORE_VALUE_SIZE];
};

typedef struct {
    struct data entries[STORE_MAX_ENTRIES];
    int count;
} Store;

/* Reset the store to an empty state. */
void store_init(Store *store);

/* Return the index of key, or -1 if it is not present. */
int store_find(const Store *store, const char *key);

/*
 * Store value under key. If key already exists its value is overwritten,
 * otherwise a new entry is appended. Returns true on success, false when the
 * arguments are invalid or the store is full.
 */
bool store_set(Store *store, const char *key, const char *value);

/* Return the value stored under key, or NULL if the key is absent. */
const char *store_get(const Store *store, const char *key);

/* Remove key from the store. Returns true if a key was removed. */
bool store_del(Store *store, const char *key);

#endif /* STORE_H */
