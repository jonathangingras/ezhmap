#include <string.h>
#include <ezhmap/murmur3.h>
#include "ezhmap/hash_table.h"


struct hash_table_allocator hash_table_default_allocator = {
  .allocate = &malloc,
  .deallocate = &free,
  .reallocate = &realloc,
};

#define HT_MALLOC(hash_table, size) (hash_table)->allocator->allocate(size)
#define HT_FREE(hash_table, object) (hash_table)->allocator->deallocate(object)
#define HT_REALLOC(hash_table, object, size) (hash_table)->allocator->reallocate(object, size)

struct node {
  void *key;
  size_t keylen;
  void *value;

  struct node *bucket_next;
  struct node **in_bucket_pointer;

  struct node *list_next;
  struct node **in_list_pointer;
};


struct hash_table {
  size_t capacity;
  size_t size;
  hash_function hash;
  struct node *node_list;
  struct node *node_list_last;
  struct node **buckets;
  struct hash_table_allocator *allocator;
};


int hash_table_init(struct hash_table **hash_table, hash_function hash, size_t capacity, struct hash_table_allocator *allocator) {
  *hash_table = allocator->allocate(sizeof(struct hash_table));
  (*hash_table)->allocator = allocator;

  (*hash_table)->hash = hash;
  (*hash_table)->capacity = capacity;
  (*hash_table)->size = 0;
  (*hash_table)->node_list = NULL;
  (*hash_table)->node_list_last = NULL;
  (*hash_table)->buckets = HT_MALLOC(*hash_table, sizeof(struct node *) * capacity);
  memset((*hash_table)->buckets, 0, sizeof(struct node *) * capacity);

  return 0;
}

int hash_table_default_init(struct hash_table **hash_table) {
  return hash_table_init(hash_table,
                         &hash_table_default_hash,
                         256,
                         &hash_table_default_allocator);
}

static void node_free(struct hash_table *hash_table, struct node *node) {
  HT_FREE(hash_table, node->key);
  /* free(tmp->value); */
  HT_FREE(hash_table, node);
}

void hash_table_free(struct hash_table *hash_table) {
  struct node *node = hash_table->node_list;
  while(node) {
    struct node *tmp = node;
    node = node->list_next;

    node_free(hash_table, tmp);
  }

  HT_FREE(hash_table, hash_table->buckets);
  HT_FREE(hash_table, hash_table);
}

size_t hash_table_default_hash(void *key, size_t keylen) {
  uint32_t buffer;
  MurmurHash3_x86_32(key, keylen, 42, &buffer);
  return buffer;
}

static struct node **get_in_bucket_pointer(struct hash_table *hash_table, void *key, size_t keylen) {
  size_t index = hash_table->hash(key, keylen) % hash_table->capacity;
  struct node **node_pointer = &hash_table->buckets[index];

  while(*node_pointer != NULL) {
    struct node *node = (*node_pointer);
    if(keylen == node->keylen && !memcmp(key, node->key, keylen)) {
      break; /* found key */
    }
    node_pointer = &((*node_pointer)->bucket_next);
  }

  return node_pointer; /* either found key or end of the bucket */
}

static void append_list(struct hash_table *hash_table, struct node *new_node) {
  if(hash_table->size == 0) {
    hash_table->node_list = new_node;
    hash_table->node_list_last = new_node;
  }

  hash_table->node_list_last->list_next = new_node;
  new_node->list_next = NULL;
  new_node->in_list_pointer = &(hash_table->node_list_last->list_next);

  hash_table->size++;
}

static void create_node(struct hash_table *hash_table, struct node **node_pointer, void *key, size_t keylen, void *value) {
  *node_pointer = HT_MALLOC(hash_table, sizeof(struct node));

  (*node_pointer)->key = HT_MALLOC(hash_table, keylen);
  memcpy((*node_pointer)->key, key, keylen);
  (*node_pointer)->keylen = keylen;

  (*node_pointer)->value = value;

  append_list(hash_table, *node_pointer);
}

static void bucket_node(struct node *node, struct node **node_pointer) {
  *node_pointer = node;
  node->bucket_next = NULL;
  node->in_bucket_pointer = node_pointer;
}

int hash_table_set(struct hash_table *hash_table, void *key, size_t keylen, void *value) {
  struct node **node_pointer = get_in_bucket_pointer(hash_table, key, keylen);

  if(*node_pointer != NULL) {
    return -1; /* key already present in table */
  }

  create_node(hash_table, node_pointer, key, keylen, value);
  bucket_node(*node_pointer, node_pointer);

  return 0;
}

int hash_table_unset(struct hash_table *hash_table, void *key, size_t keylen) {
  struct node **node_pointer = get_in_bucket_pointer(hash_table, key, keylen);

  if(*node_pointer == NULL) {
    return -1; /* key not present in table */
  }

  *((*node_pointer)->in_list_pointer) = (*node_pointer)->list_next;
  *((*node_pointer)->in_bucket_pointer) = (*node_pointer)->bucket_next;
  node_free(hash_table, *node_pointer);

  return 0;
}

int hash_table_rehash(struct hash_table *hash_table, size_t capacity) {
  if(hash_table->size >= capacity || capacity < 1) {
    return -1;
  }
  hash_table->buckets = HT_REALLOC(hash_table, hash_table->buckets, capacity);
  hash_table->capacity = capacity;

  struct node *node = hash_table->node_list;
  while(node) {
    struct node **node_pointer = get_in_bucket_pointer(hash_table, node->key, node->keylen);
    bucket_node(node, node_pointer);

    node = node->list_next;
  }

  return 0;
}

int hash_table_get(struct hash_table *hash_table, void *key, size_t keylen, void **value) {
  struct node **node_pointer = get_in_bucket_pointer(hash_table, key, keylen);

  if(*node_pointer == NULL) {
    return -1; /* key not present in table */
  }

  *value = (*node_pointer)->value;
  return 0;
}

int hash_table_foreach(struct hash_table *hash_table,
                       int (*callback)(void *key, size_t keylen, void *value, void *output),
                       void *output) {
  struct node *node = hash_table->node_list;
  while(node) {
    int return_value;
    if((return_value = callback(node->key, node->keylen, node->value, output))) {
      return return_value;
    }

    node = node->list_next;
  }

  return 0;
}
