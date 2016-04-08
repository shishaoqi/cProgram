/*
 * File: bsearch.c
 * ---------------
 * This program tests the recursive implementation of the
 * function FindStringInSortedArray by reimplementing the
 * mileage table example from Chapter 12 of The Art and
 * Science of C.
 */

#include <stdio.h>
#include <ctype.h>
#include "simpio.h"
#include "strlib.h"
#include "genlib.h"

/*
 * Constants
 * ---------
 * NCities -- Number of cities
 */

#define NCities 12

/*
 * Global variable: mileageTable
 * -----------------------------
 * This table is a two-dimensional array that holds the distance
 * between the cities whose indices are the row and column numbers.
 * Data source: Rand McNally Road Atlas.
 */

static int mileageTable[NCities][NCities] = {
    {   0,1108, 708,1430, 732, 791,2191, 663, 854, 748,2483,2625},
    {1108,   0, 994,1998, 799,1830,3017,1520, 222, 315,3128,3016},
    { 708, 994,   0,1021, 279,1091,2048,1397, 809, 785,2173,2052},
    {1430,1998,1021,   0,1283,1034,1031,2107,1794,1739,1255,1341},
    { 732, 799, 279,1283,   0,1276,2288,1385, 649, 609,2399,2327},
    { 791,1830,1091,1034,1276,   0,1541,1190,1610,1511,1911,2369},
    {2191,3017,2048,1031,2288,1541,   0,2716,2794,2703, 387,1134},
    { 663,1520,1397,2107,1385,1190,2716,   0,1334,1230,3093,3303},
    { 854, 222, 809,1794, 649,1610,2794,1334,   0, 101,2930,2841},
    { 748, 315, 785,1739, 609,1511,2703,1230, 101,   0,2902,2816},
    {2483,3128,2173,1255,2399,1911, 387,3093,2930,2902,   0, 810},
    {2625,3016,2052,1341,2327,2369,1134,3303,2841,2816, 810,   0},
};

/*
 * Global variable: cityTable
 * --------------------------
 * The array cityTable holds the names of the cities used in the
 * mileage table.
 */

static string cityTable[NCities] = {
    "Atlanta",
    "Boston",
    "Chicago",
    "Denver",
    "Detroit",
    "Houston",
    "Los Angeles",
    "Miami",
    "New York",
    "Philadelphia",
    "San Francisco",
    "Seattle",
};

/* Private function declarations */

static int GetCity(string prompt);
static int FindStringInSortedArray(string key, string array[], int n);
static int BinarySearch(string key, string array[],
                        int low, int high);

/* Main program */

main()
{
    int city1, city2;

    printf("This program looks up intercity mileage.\n");
    city1 = GetCity("Enter name of city #1: ");
    city2 = GetCity("Enter name of city #2: ");
    printf("Distance between %s", cityTable[city1]);
    printf(" and %s:", cityTable[city2]);
    printf(" %d miles.\n", mileageTable[city1][city2]);
}

/*
 * Function: GetCity
 * Usage: n = GetCity(prompt);
 * ---------------------------
 * This function prompts the user for a city name, reads in a
 * string, and returns the index corresponding to that city,
 * if it exists.  If an undefined city name is entered, the
 * user is given a chance to retry.
 */

static int GetCity(string prompt)
{
    string cityName;
    int index;

    while (TRUE) {
        printf("%s", prompt);
        cityName = GetLine();
        index = FindStringInSortedArray(cityName, cityTable, NCities);
        if (index >= 0) break;
        printf("Unknown city name -- try again.\n");
    }
    return (index);
}

/*
 * Function: FindStringInSortedArray
 * Usage: index = FindStringInSortedArray(key, array, n);
 * ------------------------------------------------------
 * This function searches the array looking for the specified
 * key. The argument n specifies the effective size of the
 * array, which must be sorted according to the lexicographic
 * order imposed by StringCompare.  If the key is found, the
 * function returns the index in the array at which that key
 * appears.  (If the key appears more than once in the array,
 * any of the matching indices may be returned).  If the key
 * does not exist in the array, the function returns -1.  In
 * this implementation, FindStringInSortedArray is simply a
 * wrapper; all the work is done by the recursive function
 * BinarySearch.
 */

static int FindStringInSortedArray(string key, string array[], int n)
{
    return (BinarySearch(key, array, 0, n - 1));
}

/*
 * Function: BinarySearch
 * Usage: index = BinarySearch(key, array, low, high);
 * ---------------------------------------------------
 * This function does the work for FindStringInSortedArray.
 * The only difference is that BinarySearch takes both the
 * upper and lower limit of the search.
 */

static int BinarySearch(string key, string array[],
                        int low, int high)
{
    int mid, cmp;

    if (low > high) return (-1);
    mid = (low + high) / 2;
    cmp = StringCompare(key, array[mid]);
    if (cmp == 0) return (mid);
    if (cmp < 0) {
        return (BinarySearch(key, array, low, mid - 1));
    } else {
        return (BinarySearch(key, array, mid + 1, high));
    }
}
