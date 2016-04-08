/*
 * File: pigword.c (strlib.h version)
 * ----------------------------------
 * This program translates a word from English to Pig Latin by
 * applying the following rules:
 *
 * o  If the word contains no vowels, no translation is done,
 *    so that the new word is the same as the original.
 *
 * o  If the word begins with a vowel, the function adds "way"
 *    to the end of the original word.
 *
 * o  If the word begins with a consonant, the function extracts
 *    the string of consonants up to the first vowel, moves that
 *    to the end of the word, and adds "ay".
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

/* Private function prototypes */

static string PigLatin(string word);
static int FindFirstVowel(string word);
static bool IsVowel(char ch);

/* Main program */

main()
{
    string word;

    printf("Enter a word: ");
    word = GetLine();
    printf("Pig Latin: %s\n", PigLatin(word));
}

/*
 * Function: PigLatin
 * Usage: translation = PigLatin(word);
 * ------------------------------------
 * This function translates a word from English to Pig Latin.
 * The translated word is returned as the value of the function.
 */

static string PigLatin(string word)
{
    int vp;
    string head, tail;

    vp = FindFirstVowel(word);
    if (vp == -1) {
        return (word);
    } else if (vp == 0) {
        return (Concat(word, "way"));
    } else {
        head = SubString(word, 0, vp - 1);
        tail = SubString(word, vp, StringLength(word) - 1);
        return (Concat(tail, Concat(head, "ay")));
    }
}

/*
 * Function: FindFirstVowel
 * Usage: k = FindFirstVowel(word);
 * --------------------------------
 * This function returns the index position of the first vowel
 * in word.  If word does not contain a vowel, FindFirstVowel
 * returns -1.
 */

static int FindFirstVowel(string word)
{
    int i;

    for (i = 0; i < StringLength(word); i++) {
        if (IsVowel(IthChar(word, i))) return (i);
    }
    return (-1);
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
