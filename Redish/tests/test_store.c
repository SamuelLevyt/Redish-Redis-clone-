#include <stdio.h>
#include <string.h>

#include "store.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        tests_run++;                                                         \
        if (!(cond)) {                                                       \
            tests_failed++;                                                  \
            printf("  FAIL: %s (line %d)\n", #cond, __LINE__);               \
        }                                                                    \
    } while (0)

static void test_init_starts_empty(void) {
    Store store;
    store_init(&store);
    CHECK(store.count == 0);
    CHECK(store_get(&store, "missing") == NULL);
    CHECK(store_find(&store, "missing") == -1);
}

static void test_init_handles_null(void) {
    /* Must not crash when handed a NULL store. */
    store_init(NULL);
}

static void test_set_then_get(void) {
    Store store;
    store_init(&store);
    CHECK(store_set(&store, "name", "redish") == true);
    CHECK(store.count == 1);
    const char *value = store_get(&store, "name");
    CHECK(value != NULL);
    CHECK(strcmp(value, "redish") == 0);
    CHECK(store_find(&store, "name") == 0);
}

static void test_set_overwrites_existing_key(void) {
    Store store;
    store_init(&store);
    CHECK(store_set(&store, "key", "first") == true);
    CHECK(store_set(&store, "key", "second") == true);
    CHECK(store.count == 1);
    CHECK(strcmp(store_get(&store, "key"), "second") == 0);
}

static void test_get_missing_key_returns_null(void) {
    Store store;
    store_init(&store);
    store_set(&store, "present", "1");
    CHECK(store_get(&store, "absent") == NULL);
}

static void test_del_removes_key(void) {
    Store store;
    store_init(&store);
    store_set(&store, "a", "1");
    store_set(&store, "b", "2");
    CHECK(store_del(&store, "a") == true);
    CHECK(store.count == 1);
    CHECK(store_get(&store, "a") == NULL);
    CHECK(strcmp(store_get(&store, "b"), "2") == 0);
}

static void test_del_missing_key_returns_false(void) {
    Store store;
    store_init(&store);
    store_set(&store, "a", "1");
    CHECK(store_del(&store, "missing") == false);
    CHECK(store.count == 1);
}

static void test_set_rejects_null_arguments(void) {
    Store store;
    store_init(&store);
    CHECK(store_set(NULL, "k", "v") == false);
    CHECK(store_set(&store, NULL, "v") == false);
    CHECK(store_set(&store, "k", NULL) == false);
    CHECK(store.count == 0);
}

static void test_set_rejects_oversized_input(void) {
    Store store;
    store_init(&store);
    char big_key[STORE_KEY_SIZE + 10];
    memset(big_key, 'k', sizeof(big_key) - 1);
    big_key[sizeof(big_key) - 1] = '\0';
    CHECK(store_set(&store, big_key, "v") == false);

    char big_value[STORE_VALUE_SIZE + 10];
    memset(big_value, 'v', sizeof(big_value) - 1);
    big_value[sizeof(big_value) - 1] = '\0';
    CHECK(store_set(&store, "k", big_value) == false);
    CHECK(store.count == 0);
}

static void test_set_rejects_when_full(void) {
    Store store;
    store_init(&store);
    char key[16];
    for (int i = 0; i < STORE_MAX_ENTRIES; i++) {
        sprintf(key, "k%d", i);
        CHECK(store_set(&store, key, "v") == true);
    }
    CHECK(store.count == STORE_MAX_ENTRIES);
    CHECK(store_set(&store, "overflow", "v") == false);
    /* An existing key can still be overwritten even when the store is full. */
    CHECK(store_set(&store, "k0", "updated") == true);
    CHECK(strcmp(store_get(&store, "k0"), "updated") == 0);
}

static void test_find_and_get_reject_null(void) {
    CHECK(store_find(NULL, "k") == -1);
    Store store;
    store_init(&store);
    CHECK(store_find(&store, NULL) == -1);
    CHECK(store_get(&store, "k") == NULL);
}

int main(void) {
    test_init_starts_empty();
    test_init_handles_null();
    test_set_then_get();
    test_set_overwrites_existing_key();
    test_get_missing_key_returns_null();
    test_del_removes_key();
    test_del_missing_key_returns_false();
    test_set_rejects_null_arguments();
    test_set_rejects_oversized_input();
    test_set_rejects_when_full();
    test_find_and_get_reject_null();

    printf("\n%d checks run, %d failed\n", tests_run, tests_failed);
    return tests_failed == 0 ? 0 : 1;
}
