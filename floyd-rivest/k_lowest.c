#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int64_t element_t;

typedef struct array *array_t;
struct array {
    int32_t   capacity;
    int32_t   size;
    element_t data[];
};

array_t array_init(int32_t capacity) {
    array_t array = malloc(sizeof(struct array) + capacity * sizeof(element_t));
    if (array != NULL) {
        array->capacity = capacity;
        array->size     = 0;
    }
    return array;
}

void array_destroy(array_t array) {
    if (array != NULL) {
        free(array);
    }
}

static inline void array_swap(element_t *x, element_t *y) {
    element_t tmp;
    tmp = *x;
    *x  = *y;
    *y  = tmp;
}

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int sign(double number) {
    if (number > 0.0) {
        return 1;
    } else if (number < 0.0) {
        return -1;
    } else {
        return 0;
    }
}

void floyd_rivest(array_t array, int32_t left, int32_t right, int32_t k) {
    //printf("-> %d %d %d\n", left, right, k);
    while (right > left) {
        if (right - left > 600) {
            int32_t n = right - left + 1;
            int32_t i = k - left + 1;
            double z  = log(n);
            double s  = 0.5 * exp(2 * z / 3);
            double sd = 0.5 * sqrt(z * s * (n - s) / n) * sign(i - n / 2);

            int32_t newLeft  = MAX(left, (int)(k - i * s / n + sd));
            int32_t newRight = MIN(right, (int)(k + (n - i) * s / n + sd));

            floyd_rivest(array, newLeft, newRight, k);
        }

        element_t t = array->data[k];
        int32_t i   = left;
        int32_t j   = right;
        array_swap(&array->data[left], &array->data[k]);
        if (array->data[right] > t) {
            array_swap(&array->data[left], &array->data[right]);
        }
        while (i < j) {
            array_swap(&array->data[i], &array->data[j]);
            i++;
            j--;
            while (array->data[i] < t) {
                i++;
            }
            while (array->data[j] > t) {
                j--;
            }
        }
        if (array->data[left] == t) {
            array_swap(&array->data[left], &array->data[j]);
        } else {
            j++;
            array_swap(&array->data[j], &array->data[right]);
        }
        if (j <= k) {
            left = j + 1;
        }
        if (k <= j) {
            right = j - 1;
        }
    }
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s n k\n", argv[0]);
        return EXIT_FAILURE;
    }

    int32_t n = atol(argv[1]);
    if ((n < 1) || (n > 2e9)) {
        fprintf(stderr, "error: n out of range [1..2*10^9]\n");
        return EXIT_FAILURE;
    }
    int32_t k = atol(argv[2]);
    if ((k < 1) || (k > 2e9)) {
        fprintf(stderr, "error: k out of range [1..2*10^9]\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    array_t array = array_init(n);

    for (int32_t x = 0; x < n; x++) {
        int64_t e = rand();
        array->data[x] = e;
    }

    floyd_rivest(array, 0, n - 1, k);

    for (int32_t x = 0; x < k; x++) {
        printf("%ld ", array->data[x]);
    }
    printf("\n");
    for (int32_t x = k; x < 2 * k; x++) {
        printf("%ld ", array->data[x]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
