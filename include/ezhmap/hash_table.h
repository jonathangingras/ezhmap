#ifndef _EZHMAP_H_
#define _EZHMAP_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hash_table;
typedef size_t (*hash_function) (void *key, size_t keylen);

size_t hash_table_default_hash(void *key, size_t keylen);

struct hash_table_allocator {
  void *(*allocate)(size_t size);
  void (*deallocate)(void *object);
  void *(*reallocate)(void *object, size_t size);
};

extern struct hash_table_allocator hash_table_default_allocator;

int hash_table_init(struct hash_table **hash_table, hash_function hash, size_t capacity, struct hash_table_allocator *allocator);
int hash_table_default_init(struct hash_table **hash_table);
void hash_table_free(struct hash_table *hash_table);

int hash_table_set(struct hash_table *hash_table, void *key, size_t keylen, void *value);
int hash_table_get(struct hash_table *hash_table, void *key, size_t keylen, void **value);

int hash_table_foreach(struct hash_table *hash_table, int (*callback)(void *key, size_t keylen, void *value, void *output), void *output);

size_t hash_table_capacity(struct hash_table *hash_table);
int hash_table_rehash(struct hash_table *hash_table, size_t capacity);

#ifdef __cplusplus
}
#endif

#endif
