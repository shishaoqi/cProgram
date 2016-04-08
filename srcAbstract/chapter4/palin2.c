/*
 * File: palin2.c
 * --------------
 * This file tests a recursive implementation of IsPalindrome.
 * This implementation uses the array representation of strings
 * rather than the strlib.h library.
 */

#include <stdio.h>
#include <string.h>
#include "genlib.h"
#include "simpio.h"

/* Function prototypes */

static bool IsPalindrome(char str[]);
static bool CheckPalindrome(char str[], int len);

/* Main program */

main()
{
    string str;

    printf("This program checks for palindromes.\n");
    printf("Indicate end of input with a blank line.\n");
    while (TRUE) {
        printf("Enter a string: ");
        str = GetLine();
        if (strlen(str) == 0) break;
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
 * This function returns TRUE if the character string str
 * is a palindrome.  This level of the implementation is
 * just a wrapper for the CheckPalindrome function, which
 * does the real work.
 */

static bool IsPalindrome(char str[])
{
    return (CheckPalindrome(str, strlen(str)));
}

/*
 * Function: CheckPalindrome
 * Usage: if (CheckPalindrome(str, len)) . . .
 * -------------------------------------------
 * This function returns TRUE if the next len characters
 * in the character array str form a palindrome.  The
 * implementation uses the recursive insight that all
 * strings of length 0 or 1 are palindromes (the simple
 * case) and that longer strings are palindromes only if
 * their first and last characters match and the remaining
 * substring is a palindrome.  Extracting the substring
 * is performed by array arithmetic.  The new substring
 * begins at str+1 and proceeds for len-2 characters.
 */

static bool CheckPalindrome(char str[], int len)
{
    if (len <= 1) {
        return (TRUE);
    } else {
        return (str[0] == str[len - 1]
                && CheckPalindrome(str + 1, len - 2));
    }
}
