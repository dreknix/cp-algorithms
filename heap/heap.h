#ifndef HEAP_H
#define HEAP_H 1

#include <stdint.h>

typedef int64_t element_t;
typedef struct heap *heap_t;

heap_t heap_init(int32_t capacity);
void heap_destroy(heap_t heap);

int32_t heap_size(heap_t heap);
int32_t heap_capacity(heap_t heap);

element_t heap_peek(heap_t heap);
element_t heap_pop(heap_t heap);
bool heap_push(heap_t heap, element_t e);

bool heap_decrease_key(heap_t heap, int32_t i, element_t e);
void heap_delete_key(heap_t heap, int32_t i);

void heap_print(heap_t heap);

#endif // HEAP_H
