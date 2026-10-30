#include <stdio.h>
#include <kv.h>

int main() {
  kv_t *table = kv_init(1024);

  printf("%p\n", table);
  printf("%ld\n", table->capacity);

  kv_put(table, "hehe", "haha");
  kv_put(table, "hehe", "hoho");
  kv_put(table, "hello", "world");
  kv_put(table, "daisy", "chain");

  for (int i = 0; i < table->capacity; i++) {
    if (table->entries[i].key) {
      printf("[%d] %s: %s\n", i, table->entries[i].key, table->entries[i].value);
    }
  }

  char *val1 = kv_get(table, "hehe");
  char *val2 = kv_get(table, "daisy");
  char *val3 = kv_get(table, "0xDEADBEEF");
  printf("val1: %s\n", val1);
  printf("val2: %s\n", val2);
  printf("val3: %s\n", val3);
}
