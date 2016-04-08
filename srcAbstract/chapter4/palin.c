/*
 * File: palin.c
 * -------------
 * This file tests a recursive implementation of IsPalindrome.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

/* Function prototypes */

static bool IsPalindrome(string str);

/* Main program */

main()
{
    string str;

    printf("This program checks for palindromes.\n");
    printf("Indicate end of input with a blank line.\n");
    while (TRUE) {
        printf("Enter a string: ");
        str = GetLine();
        if (StringEqual(str, "")) break;
        if (IsPalindrome(str)) {
            printf("'%s' is a palindrome.\n", str);
        } else {
            printf("'%s' is not a palindrome.\n", str);
        }
    }
}

/*
 * Function: IsPalindrome
 * Usage: if (IsPalindrome(str)) . . .
 * -----------------------------------
 * This function returns TRUE if the string is a palindrome.
 * This implementation operates recursively by noting that all
 * strings of length 0 or 1 are palindromes (the simple case)
 * and that longer strings are palindromes only if their first
 * and last characters match and the remaining substring is a
 * palindrome.
 */

static bool IsPalindrome(string str)
{
    int len;

    len = StringLength(str);
    if (len <= 1) {
        return (TRUE);
    } else {
        return (IthChar(str, 0) == IthChar(str, len - 1)
                && IsPalindrome(SubString(str, 1, len - 2)));
    }
}
