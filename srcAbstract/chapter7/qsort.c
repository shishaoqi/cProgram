/*
 * File: qsort.c
 * -------------
 * This file implements a pure Quicksort algorithm with no
 * enhancements for efficiency.
 */

#include <stdio.h>
#include "genlib.h"
#include "sort.h"

/* Private function prototypes */

static int Partition(int array[], int n);

/*
 * Implementation notes: SortIntegerArray
 * --------------------------------------
 * This implementation of SortIntegerArray uses the Quicksort
 * algorithm, which begins by "partitioning" the array so that
 * all elements smaller than a designated pivot element appear
 * to the left of a boundary and all equal or larger values
 * appear to the right.  Sorting the subarrays to the left and
 * right of boundary ensures that the entire array is sorted.
 */

void SortIntegerArray(int array[], int n)
{
    int boundary;

    if (n < 2) return;
    boundary = Partition(array, n);
    SortIntegerArray(array, boundary);
    SortIntegerArray(array + boundary + 1, n - boundary - 1);
}

/*
 * Function: Partition
 * Usage: boundary = Partition(array, n);
 * --------------------------------------
 * This function rearranges the elements of array relative to
 * a pivot value, which is taken from array[0].  The Partition
 * function returns a boundary index such that array[i] < pivot
 * for all i < boundary, array[i] == pivot for i == boundary,
 * and array[i] >= pivot for all i > boundary.
 */

static int Partition(int array[], int n)
{
    int lh, rh, pivot, temp;

    pivot = array[0];
    lh = 1;
    rh = n - 1;
    while (TRUE) {
        while (lh < rh && array[rh] >= pivot) rh--;
        while (lh < rh && array[lh] < pivot) lh++;
        if (lh == rh) break;
        temp = array[lh];
        array[lh] = array[rh];
        array[rh] = temp;
    }
    if (array[lh] >= pivot) return (0);
    array[0] = array[lh];
    array[lh] = pivot;
    return (lh);
}

