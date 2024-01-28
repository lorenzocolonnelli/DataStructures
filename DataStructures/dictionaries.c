#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_SIZE 64
#define HASHMAP_SIZE_LIST 1

struct aiv_dict_item {
    const char *key;
    void *value; 
    size_t key_len;
    size_t value_len; 
};

struct aiv_dict {
    struct aiv_dict_item hashmap[HASHMAP_SIZE][HASHMAP_SIZE_LIST];
};

size_t djb33x_hash(const char *key, const size_t keylen) {
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

void aiv_dict_insert(struct aiv_dict *dict, const char *key, const void *value, size_t value_len) {
    const size_t key_len = strlen(key);
    const size_t hash = djb33x_hash(key, key_len);
    const size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (dict->hashmap[index][i].key_len == 0) {
            dict->hashmap[index][i].key = _strdup(key); 
            dict->hashmap[index][i].value = malloc(value_len); 
            memcpy(dict->hashmap[index][i].value, value, value_len); 
            dict->hashmap[index][i].key_len = key_len;
            dict->hashmap[index][i].value_len = value_len;
            printf("Added %s at index %zu slot %zu\n", key, index, i);
            return;
        }
    }
    printf("COLLISION! for %s (index %zu)\n", key, index);
}

void *aiv_dict_find(struct aiv_dict *dict, const char *key) {
    const size_t key_len = strlen(key);
    const size_t hash = djb33x_hash(key, key_len);
    const size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (dict->hashmap[index][i].key_len > 0) {
            if (dict->hashmap[index][i].key_len == key_len && !memcmp(dict->hashmap[index][i].key, key, key_len)) {
                return dict->hashmap[index][i].value;
            }
        }
    }
    return NULL;
}

void aiv_dict_remove(struct aiv_dict *dict, const char *key) {
    const size_t key_len = strlen(key);
    const size_t hash = djb33x_hash(key, key_len);
    const size_t index = hash % HASHMAP_SIZE;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++) {
        if (dict->hashmap[index][i].key_len == key_len && !memcmp(dict->hashmap[index][i].key, key, key_len)) {
            free((void *)dict->hashmap[index][i].key); 
            free(dict->hashmap[index][i].value); 
            dict->hashmap[index][i].key = NULL;
            dict->hashmap[index][i].value = NULL;
            dict->hashmap[index][i].key_len = 0;
            dict->hashmap[index][i].value_len = 0;
            printf("Removed %s from index %zu slot %zu\n", key, index, i);
            return;
        }
    }
    printf("Item not found: %s\n", key);
}

void aiv_dict_print(struct aiv_dict *dict) {
    printf("Printing Dictionary:\n");
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        for (size_t j = 0; j < HASHMAP_SIZE_LIST; j++) {
            if (dict->hashmap[i][j].key_len > 0) {
                if(dict->hashmap[i][j].value_len == 4)
                {
                    printf("Index %zu, Slot %zu: Key: %s, Value: %d\n ", i, j, dict->hashmap[i][j].key, *((int *)dict->hashmap[i][j].value));
                }
                else if(dict->hashmap[i][j].value_len == 8)
                {
                    printf("Index %zu, Slot %zu: Key: %s, Value: %f\n ", i, j, dict->hashmap[i][j].key, *((double *)dict->hashmap[i][j].value));
                }
                
            }
        }
    }
}

void aiv_dict_free(struct aiv_dict *dict) {
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        for (size_t j = 0; j < HASHMAP_SIZE_LIST; j++) {
            if (dict->hashmap[i][j].key_len > 0) {
                free((void *)dict->hashmap[i][j].key);
                free(dict->hashmap[i][j].value);
            }
        }
    }
}

int main(int argc, char **argv) {
    struct aiv_dict mydict;
    memset(&mydict, 0, sizeof(struct aiv_dict));

    int value = 123; 
    double value2 = 123.3f; 

    aiv_dict_insert(&mydict, "Hello", &value, sizeof(value));
    aiv_dict_insert(&mydict, "Hello2", &value2, sizeof(value2));
    aiv_dict_insert(&mydict, "Hello3", &value, sizeof(value));


    int *found_value = (int *)aiv_dict_find(&mydict, "Hello");
    if (found_value) {
        printf("Found value for 'Hello': %d\n", *found_value);
    } else {
        printf("Value for 'Hello' not found\n");
    }

    aiv_dict_print(&mydict);
    aiv_dict_free(&mydict);

    return 0;
}
