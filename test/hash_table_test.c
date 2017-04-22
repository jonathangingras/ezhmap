#include <sweetgreen/sweetgreen.h>
#include <ezhmap/hash_table.h>


sweetgreen_test_define(hash_table, can_set_1_value) {
  struct hash_table *hash;
  hash_table_default_init(&hash);
  int k = 42;
  int v = 43;

  hash_table_set(hash, &k, sizeof(k), &v);
  void *got_value;
  hash_table_get(hash, &k, sizeof(k), &got_value);

  sweetgreen_expect_same_address(&v, got_value);
}

sweetgreen_test_define(hash_table, can_set_2_values) {
  struct hash_table *hash;
  hash_table_default_init(&hash);
  int k1 = 42, k2 = 43;
  int v1 = 44, v2 = 45;

  hash_table_set(hash, &k1, sizeof(k1), &v1);
  hash_table_set(hash, &k2, sizeof(k2), &v2);
  void *got_value1, *got_value2;
  hash_table_get(hash, &k1, sizeof(k1), &got_value1);
  hash_table_get(hash, &k2, sizeof(k2), &got_value2);

  sweetgreen_expect_same_address(&v1, got_value1);
  sweetgreen_expect_same_address(&v2, got_value2);
}


int k1_ = 42, k2_ = 43;
int v1_ = 44, v2_ = 45;

int callback(void *key, size_t keylen, void *value, void *out) {
  int *gp = (int *)out;


  if(*(int*)key == k1_ && keylen == sizeof(k1_) && value == &v1_) {
    ++*gp;
  }
  if(*(int*)key == k2_ && keylen == sizeof(k2_) && value == &v2_) {
    ++*gp;
  }

  return 0;
}

sweetgreen_test_define(hash_table, can_iterate) {
  struct hash_table *hash;
  hash_table_default_init(&hash);

  hash_table_set(hash, &k1_, sizeof(k1_), &v1_);
  hash_table_set(hash, &k2_, sizeof(k2_), &v2_);

  int good_pairs = 0;

  hash_table_foreach(hash, &callback, &good_pairs);

  sweetgreen_expect_equal(2, good_pairs);
}
