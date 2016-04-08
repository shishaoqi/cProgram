/*
 * File: isvowel.c
 * ---------------
 * This file defines the predicate function IsVowel.
 */

#include <stdio.h>
#include "genlib.h"

/* Private function prototypes */

static bool IsVowel(char ch);

/* Main program */

main()
{
    char ch;

    printf("The vowels are:");
    for (ch = 'A'; ch <= 'Z'; ch++) {
        if (IsVowel(ch)) printf(" %c", ch);
    }
    printf("\n");
}

/*
 * Function: IsVowel
 * Usage: if (IsVowel(ch)) . . .
 * -----------------------------
 * This function returns TRUE if ch is a vowel, which are
 * recognized in either upper or lower case.
 */

static bool IsVowel(char ch)
{
    switch (ch) {
      case 'A': case 'E': case 'I': case 'O': case 'U':
      case 'a': case 'e': case 'i': case 'o': case 'u':
        return (TRUE);
      default:
        return (FALSE);
    }
}
