#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define COMPARE(x,y) (x > y)  /* max heap */
//#define COMPARE(x,y) (x < y)  /* min heap */

struct heap {
    int32_t   capacity;
    int32_t   size;
    element_t array[];
};

static inline int32_t parent(int32_t i) { return (i - 1) / 2; }
// static inline int32_t parent(int32_t i) { printf("  parent(%d)->%d\n", i, (i-1)/2); return (i-1)/2; }

static inline int32_t left(int32_t i) { return (2 * i + 1); }
// static inline int32_t left(int32_t i) { printf("  left(%d)->%d\n", i, 2*i+1); return (2*i + 1); }

static inline int32_t right(int32_t i) { return (2 * i + 2); }
// static inline int32_t right(int32_t i) { printf("  left(%d)->%d\n", i, 2*i+2); return (2*i + 2); }

heap_t heap_init(int32_t capacity) {
    heap_t heap = malloc(sizeof(heap_t) + capacity * sizeof(element_t));
    if (heap != NULL) {
        heap->capacity = capacity;
        heap->size     = 0;
    }
    return heap;
}

void heap_destroy(heap_t heap) {
    free(heap);
}

int32_t heap_size(heap_t heap) { return heap->size; }
int32_t heap_capacity(heap_t heap) { return heap->capacity; }

static inline void heap_swap(element_t *x, element_t *y) {
    element_t tmp;
    tmp = *x;
    *x  = *y;
    *y  = tmp;
}

static void heapify(heap_t heap, int32_t i) {
    size_t l    = left(i);
    size_t r    = right(i);
    size_t node = i;
    if (l < heap->size && COMPARE(heap->array[l], heap->array[i])) {
        node = l;
    }
    if (r < heap->size && COMPARE(heap->array[r], heap->array[node])) {
        node = r;
    }
    if (node != i) {
        heap_swap(&heap->array[i], &heap->array[node]);
        heapify(heap, node);
    }
}

element_t heap_peek(heap_t heap) {
    if (heap->size <= 0) {
        return -1;
    }
    return heap->array[0];
}

element_t heap_pop(heap_t heap) {
    if (heap->size <= 0) {
        return -1;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }

    element_t root = heap->array[0];
    heap->array[0] = heap->array[--heap->size];
    heapify(heap, 0);

    return root;
}

bool heap_push(heap_t heap, element_t e) {
    if (heap->size == heap->capacity) {
        return false;
    }

    int32_t i      = heap->size++;
    heap->array[i] = e;

    while (i != 0 && COMPARE(heap->array[i], heap->array[parent(i)])) {
       heap_swap(&heap->array[i], &heap->array[parent(i)]);
       i = parent(i);
    }
    return true;
}

bool heap_decrease_key(heap_t heap, int32_t i, element_t e) {
    if (COMPARE(heap->array[i], e)) {
        return false;
    }
    heap->array[i] = e;
    while (i != 0 && COMPARE(heap->array[i], heap->array[parent(i)])) {
       heap_swap(&heap->array[i], &heap->array[parent(i)]);
       i = parent(i);
    }
    return true;
}


void heap_delete_key(heap_t heap, int32_t i) {
    heap_decrease_key(heap, i, INT32_MIN);
    heap_pop(heap);
}

void heap_print(heap_t heap) {
    printf("array(%d/%d): ", heap->size, heap->capacity);
    for (int i = 0; i < heap->size; i++) {
        printf("%ld ", heap->array[i]);
    }
    printf("\n");
}
