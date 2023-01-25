#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int64_t *a, int64_t *b) {
  int64_t tmp;
  tmp = *a;
  *a  = *b;
  *b  = tmp;
}

// from: https://www.geeksforgeeks.org/3-way-quicksort-dutch-national-flag/
// quicksort with many duplicates
void partition(int64_t array[], int32_t l, int32_t r, int32_t *pi, int32_t *pj) {
    int32_t i = l - 1;
    int32_t j = r;
    int32_t p = l - 1;
    int32_t q = r;
    int32_t v = array[r];

    while (true) {
        // From left, find the first element greater than
        // or equal to v. This loop will definitely
        // terminate as v is last element
        while (array[++i] < v)
            ;

        // From right, find the first element smaller than
        // or equal to v
        while (v < array[--j])
            if (j == l)
                break;

        // If i and j cross, then we are done
        if (i >= j)
            break;

        // Swap, so that smaller goes on left greater goes
        // on right
        swap(&array[i], &array[j]);

        // Move all same left occurrence of pivot to
        // beginning of array and keep count using p
        if (array[i] == v) {
            p++;
            swap(&array[p], &array[i]);
        }

        // Move all same right occurrence of pivot to end of
        // array and keep count using q
        if (array[j] == v) {
            q--;
            swap(&array[j], &array[q]);
        }
    }

    // Move pivot element to its correct index
    swap(&array[i], &array[r]);

    // Move all left same occurrences from beginning
    // to adjacent to arr[i]
    j = i - 1;
    for (int k = l; k < p; k++, j--)
        swap(&array[k], &array[j]);

    // Move all right same occurrences from end
    // to adjacent to arr[i]
    i = i + 1;
    for (int k = r - 1; k > q; k--, i++)
        swap(&array[i], &array[k]);

    *pi = i;
    *pj = j;
}

// 3-way partition based quick sort
void quicksort(int64_t array[], int32_t l, int32_t r) {
    if (r <= l)
        return;

    int32_t i, j;

    partition(array, l, r, &i, &j);

    quicksort(array, l, j);
    quicksort(array, i, r);
}

#define NUMBER_OF_SORTS 100
bool issorted(int64_t array[], int32_t n) {
    bool sorted = false;
    int step = 0;
    while (! sorted && step < NUMBER_OF_SORTS) {
        sorted = true;
        for (int32_t i = 0; i < n - 1; i++) {
            if (array[i] > array[i+1]) {
                swap(&array[i], &array[i+1]);
                i++;
                sorted = false;
            }
        }
        step++;
    }
    return sorted;
}

// try to sort a nearly sorted array differently
// otherwise use quicksort (with possible duplicates)
void sort(int64_t a[], int32_t n) {
    if (issorted(a, n)) {
        return;
    } else {
        quicksort(a, 0, n-1);
    }
}
