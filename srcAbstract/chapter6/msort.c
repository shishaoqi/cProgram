/*
 * File: msort.c
 * -------------
 * This file implements the sort.h interface using the merge
 * sort algorithm.
 */

#include <stdio.h>
#include "genlib.h"
#include "sort.h"

/* Private function prototypes */

static void Merge(int array[], int arr1[], int n1,
                               int arr2[], int n2);
static int *CopySubArray(int array[], int start, int n);

/* Implementation */

/*
 * Function: SortIntegerArray
 * Usage: SortIntegerArray(array, n);
 * ----------------------------------
 * This function sorts the first n elements of array into
 * increasing numerical order using the merge sort algorithm,
 * which requires (1) dividing the array into two halves,
 * (2) sorting each half, and (3) merging the halves together.
 */

void SortIntegerArray(int array[], int n)
{
    int n1, n2, *arr1, *arr2;

    if (n <= 1) return;
    n1 = n / 2;
    n2 = n - n1;
    arr1 = CopySubArray(array, 0, n1);
    arr2 = CopySubArray(array, n1, n2);
    SortIntegerArray(arr1, n1);
    SortIntegerArray(arr2, n2);
    Merge(array, arr1, n1, arr2, n2);
    FreeBlock(arr1);
    FreeBlock(arr2);
}

/*
 * Function: Merge
 * Usage: Merge(array, arr1, n1, arr2, n2);
 * ----------------------------------------
 * This function merges two sorted arrays (arr1 and arr2) into a
 * single output array.  Because the input arrays are sorted, the
 * implementation can always select the first unused element in
 * one of the input arrays to fill the next position in array.
 */

static void Merge(int array[], int arr1[], int n1,
                                  int arr2[], int n2)
{
    int p, p1, p2;

    p = p1 = p2 = 0;
    while (p1 < n1 && p2 < n2) {
        if (arr1[p1] < arr2[p2]) {
            array[p++] = arr1[p1++];
        } else {
            array[p++] = arr2[p2++];
        }
    }
    while (p1 < n1) array[p++] = arr1[p1++];
    while (p2 < n2) array[p++] = arr2[p2++];
}

/*
 * Function: CopySubArray
 * Usage: CopySubArray(array, start, n);
 * -------------------------------------
 * This function makes a copy of a subset of an integer array
 * and returns a pointer to a new dynamic array containing the
 * new elements.  The array begins at the indicated start
 * position in the original array and continues for n elements.
 */

static int *CopySubArray(int array[], int start, int n)
{
    int i, *result;

    result = NewArray(n, int);
    for (i = 0; i < n; i++) {
        result[i] = array[start + i];
    }
    return (result);
}

