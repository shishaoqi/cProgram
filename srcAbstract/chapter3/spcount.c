/*
 * File: spcount.c
 * ---------------
 * This file defines the function CountSpaces, which returns
 * the number of spaces in a string.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

/* Private function prototypes */

static int CountSpacesArray(char str[]);
static int CountSpacesPointer(char *str);

/* Main program */

main()
{
    string str;

    printf("Enter a string: ");
    str = GetLine();
    printf("Array:   %d spaces.\n", CountSpacesArray(str));
    printf("Pointer: %d spaces.\n", CountSpacesPointer(str));
}

/*
 * Function: CountSpaces
 * Usage: nSpaces = CountSpaces(str);
 * ----------------------------------
 * This function counts the number of spaces in the string str.
 */

/* Array implementation */

#define CountSpaces CountSpacesArray

static int CountSpaces(char str[])
{
    int i, nSpaces;

    nSpaces = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') nSpaces++;
    }
    return (nSpaces);
}

#undef CountSpaces

/* Pointer implementation */

#define CountSpaces CountSpacesPointer

static int CountSpaces(char *str)
{
    int nSpaces;
    char *cp;

    nSpaces = 0;
    for (cp = str; *cp != '\0'; cp++) {
        if (*cp == ' ') nSpaces++;
    }
    return (nSpaces);
}

#undef CountSpaces
