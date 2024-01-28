#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_SIZE 64
#define HASHMAP_SIZE_LIST 2

struct aiv_set_item {
    char *key;
    size_t key_len;
};

struct aiv_set {
    struct aiv_set_item hashmap[HASHMAP_SIZE][HASHMAP_SIZE_LIST];
};

size_t djb33x_hash(const char *key, const size_t keylen) {
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}


int key_exists(struct aiv_set *set, const char *key, size_t key_len) {
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (set->hashmap[index][i].key_len == key_len && !memcmp(set->hashmap[index][i].key, key, key_len)) {
            return 1; 
        }
    }
    return 0; 
}

void aiv_set_insert(struct aiv_set *set, const char *key) {
    size_t key_len = strlen(key);

    
    if (key_exists(set, key, key_len)) {
        printf("Key already exists: %s\n", key);
        return;
    }

    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (set->hashmap[index][i].key_len == 0) {
            set->hashmap[index][i].key = _strdup(key);
            set->hashmap[index][i].key_len = key_len;
            printf("Added %s at index %zu slot %zu\n", key, index, i);
            return;
        }
    }
    printf("COLLISION! for %s (index %zu)\n", key, index);
}

void aiv_set_find(struct aiv_set *set, const char *key) {
    size_t key_len = strlen(key);
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (set->hashmap[index][i].key_len > 0) {
            if (set->hashmap[index][i].key_len == key_len && !memcmp(set->hashmap[index][i].key, key, key_len)) {
                printf("FOUND %s at index %zu slot %zu\n", key, index, i);
                return;
            }
        }
    }
    printf("Key not found: %s\n", key);
}

void aiv_set_remove(struct aiv_set *set, const char *key) {
    size_t key_len = strlen(key);
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (set->hashmap[index][i].key_len == key_len && !memcmp(set->hashmap[index][i].key, key, key_len)) {
            free(set->hashmap[index][i].key); 
            set->hashmap[index][i].key = NULL;
            set->hashmap[index][i].key_len = 0;
            printf("Removed %s from index %zu slot %zu\n", key, index, i);
            return;
        }
    }
    printf("Key not found: %s\n", key);
}

void aiv_set_print(struct aiv_set *set) {
    printf("Printing Set:\n");
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        for (size_t j = 0; j < HASHMAP_SIZE_LIST; j++) {
            if (set->hashmap[i][j].key_len > 0) {
                printf("Index %zu, Slot %zu: %s\n", i, j, set->hashmap[i][j].key);
            }
        }
    }
}

void aiv_set_free(struct aiv_set *set) {
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        for (size_t j = 0; j < HASHMAP_SIZE_LIST; j++) {
            if (set->hashmap[i][j].key != NULL) {
                free(set->hashmap[i][j].key);
            }
        }
    }
}

int main(int argc, char **argv) {
    struct aiv_set myset;
    memset(&myset, 0, sizeof(struct aiv_set));

    aiv_set_insert(&myset, "Hello");
    aiv_set_insert(&myset, "Hello");
    aiv_set_insert(&myset, "Test");
    aiv_set_insert(&myset, "Foobar");
    aiv_set_insert(&myset, "XYZ");
    aiv_set_insert(&myset, "AAAAAA");
    aiv_set_insert(&myset, "AAAAAa");

    aiv_set_find(&myset, "XYZ");

    aiv_set_remove(&myset, "XYZ");

    aiv_set_print(&myset);

    aiv_set_free(&myset); 

    return 0;
}
