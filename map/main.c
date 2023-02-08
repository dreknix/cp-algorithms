#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    uint64_t key;
    int64_t  value;
} entry_t;

#define EMPTY_ENTRY (UINT64_MAX)
#define REMOVED_ENTRY (UINT64_MAX - 1)

static const entry_t empty_entry = {EMPTY_ENTRY, 0};
static const entry_t removed_entry = {REMOVED_ENTRY, 0};

struct map {
    uint32_t capacity;
    uint32_t size;
    uint32_t collisions;
    entry_t  *entries;
};
typedef struct map *map_t;

map_t map_create(uint32_t capacity) {
    map_t map = malloc(sizeof(struct map));
    if (map != NULL) {
        map->entries = malloc(capacity * sizeof(entry_t));
        if (map->entries == NULL) {
            free(map);
            map = NULL;
        } else {
            map->capacity   = capacity;
            map->size       = 0;
            map->collisions = 0;
            for (uint32_t i = 0; i < map->capacity; i++) {
                map->entries[i] = empty_entry;
            }
        }
    }
    return map;
}

void map_destroy(map_t map) {
    if (map != NULL) {
        if (map->entries != NULL) {
            free(map->entries);
            map->entries = NULL;
        }
        free(map);
    }
}

uint32_t map_capacity(map_t map) { return map->capacity; }
uint32_t map_size(map_t map) { return map->size; }

// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
inline uint32_t rotl32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

inline uint32_t getblock32(const uint32_t * p, int i) {
    return p[i];
}

inline uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

void MurmurHash3_x86_32(const void *key, int len, uint32_t seed, void *out) {
  const uint8_t *data = (const uint8_t*)key;
  const int nblocks   = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t *blocks = (const uint32_t *)(data + nblocks*4);

  for (int i = -nblocks; i; i++) {
      uint32_t k1 = getblock32(blocks,i);

      k1 *= c1;
      k1  = rotl32(k1,15);
      k1 *= c2;

      h1 ^= k1;
      h1  = rotl32(h1,13);
      h1  = h1 * 5 + 0xe6546b64;
  }

  //----------
  // tail

  const uint8_t *tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3) {
    case 3:
      k1 ^= tail[2] << 16;
    case 2:
      k1 ^= tail[1] << 8;
    case 1:
      k1 ^= tail[0];
      k1 *= c1;
      k1  = rotl32(k1,15);
      k1 *= c2;
      h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;
  h1  = fmix32(h1);

  *(uint32_t*)out = h1;
}

static uint32_t generate_hash(uint64_t key) {
    uint32_t hash;
    MurmurHash3_x86_32 (&key, 8, 0xdeadbeef, &hash);
    return hash;
}

static entry_t *map_find_entry(map_t map, uint64_t key, bool empty) {
    uint32_t hash = generate_hash(key);

    uint32_t index = hash % map->capacity;

    entry_t *entry = &map->entries[index];

    uint32_t skip = 1;

    while (true) {
        // empty slots are used when searching empty slots
        if (entry->key == EMPTY_ENTRY) {
            entry = (empty ? entry : NULL);
            break;
        }
        // if searching for a empty place use removed slots
        if (empty && (entry->key == REMOVED_ENTRY)) {
            break;
        }
        if (entry->key == key) {
            break;
        }
        index = (index + skip) % map->capacity;
        entry = &map->entries[index];
        // TODO skip+=3;
    }

    if (empty && entry->key != key && index != hash % map->capacity) {
        map->collisions++;
    }

    return entry;
}

static bool map_enlarge(map_t map) {
    entry_t  *old_entries  = map->entries;
    uint32_t old_capacity  = map->capacity;
    uint32_t new_capacity  = old_capacity * 2;
    entry_t  *new_entries  = malloc(new_capacity * sizeof(entry_t));
    if (new_entries == NULL) {
        fprintf(stderr, "error: can not increase hash map\n");
        return false;
    }
    map->collisions = 0;
    map->capacity   = new_capacity;
    map->entries    = new_entries;
    for (uint32_t i = 0; i < map->capacity; i++) {
        map->entries[i] = empty_entry;
    }
    entry_t *slot = NULL;
    for (uint32_t i = 0; i < old_capacity; i++) {
        if ((old_entries[i].key != EMPTY_ENTRY) && (old_entries[i].key != REMOVED_ENTRY)) {
            slot = map_find_entry(map, old_entries[i].key, true);
            if (slot->key != EMPTY_ENTRY) {
                fprintf(stderr, "error: impossible situation in hash map occurred\n");
                exit(EXIT_FAILURE);
            }
            *slot = old_entries[i];
        }
    }
    free(old_entries);
    return true;
}

bool map_add(map_t map, entry_t entry) {
    if (map->size > 2.0/3.0 * map->capacity) {
        if (! map_enlarge(map)) {
            return false;
        }
    }
    entry_t *slot = map_find_entry(map, entry.key, true);
    if ((slot->key != EMPTY_ENTRY) && (slot->key != REMOVED_ENTRY)) {
        return false;
    }
    *slot = entry;
    map->size++;
    return true;
}

entry_t *map_get(map_t map, uint64_t key) {
    return map_find_entry(map, key, false);
}

bool map_remove(map_t map, uint64_t key) {
    entry_t *slot = map_find_entry(map, key, false);
    if (slot == NULL) {
        return false;
    } else {
        *slot = removed_entry;
        map->size--;
    }
    return true;
}

void map_dump(map_t map) {
    printf("hash map (%p/%d/%d) with %d collisions:\n",
            map, map->size, map->capacity, map->collisions);
    for (uint32_t i = 0; i < map->capacity; i++) {
        /*
        printf(" - [%u] ", i);
        if ((map->entries[i].key != EMPTY_ENTRY) && (map->entries[i].key != REMOVED_ENTRY)) {
            printf("%lu -> %ld",
                    map->entries[i].key, map->entries[i].value);
        }
        printf("\n");
        */
        if ((map->entries[i].key != EMPTY_ENTRY) && (map->entries[i].key != REMOVED_ENTRY)) {
            printf("X");
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main(int argc, const char *argv[]) {
    entry_t entry;
    entry_t *ptr;

    map_t map = map_create(2);

    srand(time(NULL));

    for (int i = 0; i < 2048; i++) {
        uint64_t k = rand();
        //printf("hash(%lu) = %u\n", k, generate_hash(k) % map_capacity(map));
        entry.key = k;
        entry.value = 2 * k;
        map_add(map, entry);
    }

    ptr = map_get(map, 47);
    if (ptr == NULL) {
        printf("%d: NULL\n", 47);
    } else {
        printf("%ld: %ld\n", ptr->key, ptr->value);
    }

    map_remove(map, 47);
    ptr = map_get(map, 47);
    if (ptr == NULL) {
        printf("%d: NULL\n", 47);
    } else {
        printf("%ld: %ld\n", ptr->key, ptr->value);
    }

    map_dump(map);

    map_destroy(map);

    return EXIT_SUCCESS;
}
