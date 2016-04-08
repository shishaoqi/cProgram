/*
 * File: ssort.c
 * -------------
 * This file implements the sort.h interface using the
 * selection sort algorithm.
 */

#include <stdio.h>
#include "genlib.h"
#include "sort.h"

/*
 * Function: SortIntegerArray
 * --------------------------
 * This implementation uses an algorithm called selection sort,
 * which can be described in English as follows.  With your left
 * hand (lh), point at each element in the array in turn,
 * starting at index 0.  At each step in the cycle:
 *
 * 1. Find the smallest element in the range between your left
 *    hand and the end of the array, and point at that element
 *    with your right hand (rh).
 *
 * 2. Move that element into its correct position by exchanging
 *    the elements indicated by your left and right hands.
 */

void SortIntegerArray(int array[], int n)
{
    int lh, rh, i, temp;

    for (lh = 0; lh < n; lh++) {
        rh = lh;
        for (i = lh + 1; i < n; i++) {
            if (array[i] < array[rh]) rh = i;
        }
        temp = array[lh];
        array[lh] = array[rh];
        array[rh] = temp;
    }
}
