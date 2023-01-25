#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int64_t *a, int64_t *b) {
    int64_t tmp;
    tmp = *a;
    *a  = *b;
    *b  = tmp;
}

int partition(int64_t array[], int32_t low, int32_t high) {
    int64_t pivot = array[high];
    int32_t i     = (low - 1);

    for (int32_t j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quicksort(int64_t array[], int32_t low, int32_t high) {
    if (low < high) {
        int32_t pi = partition(array, low, high);

        quicksort(array, low, pi - 1);
        quicksort(array, pi + 1, high);
    }
}

void sort(int64_t array[], int32_t n) {
    assert(array != NULL);

    quicksort(array, 0, n - 1);

    return;
}
