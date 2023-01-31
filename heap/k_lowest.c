#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heap.h"

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s n k\n", argv[0]);
        return EXIT_FAILURE;
    }

    int64_t n = atol(argv[1]);
    if ((n < 1) || (n > 2e10)) {
        fprintf(stderr, "error: n out of range [1..2*10^10]\n");
        return EXIT_FAILURE;
    }
    int32_t k = atol(argv[2]);
    if ((k < 1) || (k > 2e9)) {
        fprintf(stderr, "error: k out of range [1..2*10^9]\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    heap_t heap = heap_init(k);

    for (int64_t x = 0; x < n; x++) {
        element_t e = rand() % 100000;
        if (heap_size(heap) == k) {
            if (heap_peek(heap) > e) {
                heap_pop(heap);
            }
        }
        heap_push(heap, e);
        // printf("\n-> inserting %ld in heap(%d/%d): ",
        //         e, heap_size(heap), heap_capacity(heap));
        // heap_print(heap);
    }

    printf("sorted content of heap(%d/%d) after %ld insertions:\n",
            heap_size(heap), heap_capacity(heap), n);
    while (heap_size(heap) > 0) {
        printf("%ld ", heap_pop(heap));
    }
    printf("\n");

    return EXIT_SUCCESS;
}
