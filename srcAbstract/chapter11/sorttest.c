/*
 * File: sorttest.c
 * ----------------
 * This program tests the qsort library function by using it
 * as both an alphabetization and numeric sorting function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "cmpfn.h"

/*
 * Constants
 * ---------
 * MaxArray        -- Maximum size of test arrays
 * NumericSentinel -- Sentinel value used to terminate integer list
 */

#define MaxArray        100
#define NumericSentinel  -1

/* Private function prototypes */

static void NumericSort(void);
static void AlphabeticSort(void);
static bool PrefixMatch(string prefix, string str);
static int GetIntegerArray(int array[], int max, int sentinel);
static int GetStringArray(string array[], int max, string sentinel);
static void PrintIntegerArray(int array[], int n);
static void PrintStringArray(string array[], int n);

/* Main program */

main()
{
    string stype;

    printf("Enter sort type (numeric or alphabetic): ");
    stype = ConvertToLowerCase(GetLine());
    if (PrefixMatch(stype, "numeric")) {
        NumericSort();
    } else if (PrefixMatch(stype, "alphabetic")) {
        AlphabeticSort();
    } else {
        Error("Unrecognized sort type");
    }
}

/* Private functions */

/*
 * Function: NumericSort
 * Usage: NumericSort();
 * ---------------------
 * This function tests the use of qsort with integers.
 */

static void NumericSort(void)
{
    int array[MaxArray];
    int n;

    printf("Enter numeric elements (use %d to stop)\n",
           NumericSentinel);
    n = GetIntegerArray(array, MaxArray, NumericSentinel);
    qsort(array, n, sizeof(int), IntCmpFn);
    printf("The sorted array is:\n");
    PrintIntegerArray(array, n);
}

/*
 * Function: AlphabeticSort
 * Usage: AlphabeticSort();
 * ------------------------
 * This function tests the use of qsort with strings.
 */

static void AlphabeticSort(void)
{
    string array[MaxArray];
    int n;

    printf("Enter strings (use blank line to stop)\n");
    n = GetStringArray(array, MaxArray, "");
    qsort(array, n, sizeof(string), StringCmpFn);
    printf("The alphabetized array is:\n");
    PrintStringArray(array, n);
}

/*
 * Function: PrefixMatch
 * Usage: if (PrefixMatch(prefix, str)) . . .
 * ------------------------------------------
 * This function returns TRUE if prefix matches the beginning of str.
 */

static bool PrefixMatch(string prefix, string str)
{
    return (strncmp(prefix, str, strlen(prefix)) == 0);
}

/*
 * Function: GetIntegerArray
 * Usage: n = GetIntegerArray(array, max, sentinel);
 * -------------------------------------------------
 * This function reads elements into an integer array by
 * reading values, one per line, from the keyboard.  The end
 * of the input data is indicated by the parameter sentinel.
 * The caller is responsible for declaring the array and
 * passing it as a parameter, along with its allocated
 * size.  The value returned is the number of elements
 * actually entered and therefore gives the effective size
 * of the array, which is typically less than the allocated
 * size given by max.  If the user types in more than max
 * elements, GetIntegerArray generates an error.
 */

static int GetIntegerArray(int array[], int max, int sentinel)
{
    int n, value;

    n = 0;
    while (TRUE) {
        printf(" ? ");
        value = GetInteger();
        if (value == sentinel) break;
        if (n == max) Error("Too many input items for array");
        array[n] = value;
        n++;
    }
    return (n);
}

/*
 * Function: GetStringArray
 * Usage: n = GetStringArray(array, max, sentinel);
 * ------------------------------------------------
 * This function reads elements into an array of strings by
 * reading values, one per line, from the keyboard.  The end
 * of the input data is indicated by the parameter sentinel.
 * The caller is responsible for declaring the array and
 * passing it in as a parameter, along with its allocated
 * size.  The value returned is the number of elements
 * actually entered and therefore gives the effective size
 * of the array, which is typically less than the allocated
 * size given by max.  If the user types in more than max
 * elements, GetStringArray generates an error.
 */

static int GetStringArray(string array[], int max, string sentinel)
{
    int n;
    string value;

    n = 0;
    while (TRUE) {
        printf(": ");
        value = GetLine();
        if (StringEqual(value, sentinel)) break;
        if (n == max) Error("Too many input items for array");
        array[n] = value;
        n++;
    }
    return (n);
}

/*
 * Function: PrintIntegerArray
 * Usage: PrintIntegerArray(array, n);
 * -----------------------------------
 * This function displays the first n values in array,
 * one per line, on the console.
 */

static void PrintIntegerArray(int array[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        printf("%d\n", array[i]);
    }
}

/*
 * Function: PrintStringArray
 * Usage: PrintStringArray(array, n);
 * ----------------------------------
 * This function displays the first n values in array,
 * one per line, on the console.
 */

static void PrintStringArray(string array[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        printf("%s\n", array[i]);
    }
}
