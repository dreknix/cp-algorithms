#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sort.h"

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s [rand|dup|sort] n\n", argv[0]);
        return EXIT_FAILURE;
    }

    int32_t n = atol(argv[2]);

    if ((n < 1) || (n > 2e5)) {
        fprintf(stderr, "error: n out of range [1..2*10^5]\n");
        return EXIT_FAILURE;
    }

    int64_t *array = malloc(n * sizeof(int64_t));

    if (strncmp(argv[1], "rand", strlen("rand")) == 0) {
        srand(time(NULL));
        for (int32_t i = 0; i < n; i++) {
            array[i] = rand() % INT64_MAX;
        }
    } else if (strncmp(argv[1], "sort", strlen("sort")) == 0) {
        for (int32_t i = 0; i < n; i++) {
            array[i] = i;
        }
    } else if (strncmp(argv[1], "dup", strlen("dup")) == 0) {
        srand(time(NULL));
        for (int32_t i = 0; i < n; i++) {
            if (i % 3 == 0) {
                array[i] = 3;
            } else if (i % 5) {
                array[i] = 3;
            } else if (i % 7) {
                array[i] = 3;
            } else {
                array[i] = rand() % INT64_MAX;
            }
        }
    } else if (strncmp(argv[1], "equal", strlen("equal")) == 0) {
        for (int32_t i = 0; i < n; i++) {
            array[i] = 1;
        }
    } else {
        fprintf(stderr, "usage: %s [rand|dup|sort] n\n", argv[0]);
        return EXIT_FAILURE;
    }

    sort(array, n);

    bool sorted = true;
    for (int32_t i = 0; i < n - 1; i++) {
        if (array[i] > array[i + 1]) {
            sorted = false;
            break;
        }
    }
    if (!sorted) {
        fprintf(stderr, "error: array is not sorted\n");
    }

    free(array);

    return EXIT_SUCCESS;
}
