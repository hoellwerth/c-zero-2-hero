#include <kv.h>
#include <stdlib.h>
#include <string.h>

size_t hash(char *value, size_t capacity)
{
  size_t hash = 0x13371337deadbeef;
  
  while (*value) {
    hash ^= *value;
    hash = hash << 8;
    hash += *value;

    value++;
  }

  return hash % capacity;
}

/*  kv_get put a value into the key 
 *  @params: 
 *    db: a pointer to the key-value structure
 *    key: a char pointer to the key 
 *    value: a char pointer to the value
 *  @return:
 *    pointer to the value, NULL if not found
*/
char *kv_get(kv_t *db, char *key)
{
  if (!db || !key) {
    return NULL;
  }

  // hash key
  size_t index = hash(key, db->capacity);

  for (int i = 0; i < db->capacity - 1; i++) {
    size_t search_index = (index + 1) % db->capacity;
    kv_entry_t *entry = &db->entries[search_index];

    if (entry->key == NULL) {
      return NULL;
    }

    if (!strcmp(entry->key, key) && entry->key != (void *)TOMBSTONE) {
      return entry->value;
    }
  }

  return NULL;
}


/*  kv_put: put a value into the key 
 *  @params: 
 *    db: a pointer to the key-value structure
 *    key: a char pointer to the key 
 *    value: a char pointer to the value
 *  @return:
 *    index of the key, on error -1
*/
int kv_put(kv_t *db, char *key, char *value)
{
  if (!db || !key || !value) {
    return -1;
  }

  // hash key
  size_t index = hash(key, db->capacity);

  for (int i = 0; i < db->capacity - 1; i++) {
    size_t search_index = (index + 1) % db->capacity;
    kv_entry_t *entry = &db->entries[search_index];

    // two cases:
    // 1. key already set: update it
    // 2. empty slot: put value there
    if (entry->key && entry->key != (void *)TOMBSTONE && !strcmp(entry->key, key)) {
      char *newval = strdup(value);
      if (!newval) {
        return -1;
      }
      entry->value = newval;
      return 0;
    }

    if (!entry->key || entry->key == (void *)TOMBSTONE) {
      char *newval = strdup(value);
      char *newkey = strdup(key);
      if (!newval || !newkey) {
        free(newval);
        free(newkey);
        return -1;
      }
      entry->value = newval;
      entry->key = newkey;
      db->count++;
      return 0;
    }
  }

  return -2;
}

int kv_delete(kv_t *db, char *key)
{
  if (!db || !key) {
    return -1;
  }

  // hash key
  size_t index = hash(key, db->capacity);

  for (int i = 0; i < db->capacity - 1; i++) {
    size_t search_index = (index + 1) % db->capacity;
    kv_entry_t *entry = &db->entries[search_index];

    if (entry->key == NULL) {
      return -1;
    }

    if (!strcmp(entry->key, key) && entry->key != (void *)TOMBSTONE) {
      free(entry->key);
      free(entry->value);

      db->count--;

      entry->key = TOMBSTONE;
      entry->value = NULL;

      return 0; //search_index;
    }
  }

  return -1;
}

kv_t *kv_init(size_t capacity)
{
  if (capacity == 0) {
    return NULL;
  }

  kv_t *table = malloc(sizeof(kv_t));
  if (table == NULL) {
    return NULL;
  }

  table->capacity = capacity;
  table->count = 0;

  table->entries = calloc(sizeof(kv_entry_t), capacity);
  if (table->entries == NULL) {
    return NULL;
  }

  return table;
}
