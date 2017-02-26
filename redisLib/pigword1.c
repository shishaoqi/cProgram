/*
 * File: pigword.c (string.h version)
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
#include <string.h>
#include "genlib.h"
#include "simpio.h"

/*
 * Constants
 * ---------
 * MaxWord -- Length of the maximum word
 */

#define MaxWord 20

/* Private function prototypes */

static void PigLatin(char *word, char buffer[], int bufferSize);
static char *FindFirstVowel(char *word);
static bool IsVowel(char ch);

/* Main program */

main()
{
    string word;
    char translationBuffer[MaxWord + 1];

    printf("Enter a word: ");
    word = GetLine();
    PigLatin(word, translationBuffer, MaxWord + 1);
    printf("Pig Latin: %s\n", translationBuffer);
}

/*
 * Function: PigLatin
 * Usage: PigLatin(word, buffer, bufferSize);
 * ------------------------------------------
 * This function translates a word from English to Pig Latin.  The
 * translated word is written into the array buffer, which has an
 * allocated size of bufferSize.  The code checks for buffer
 * overflow and generates an error if it occurs.
 */

static void PigLatin(char *word, char buffer[], int bufferSize)
{
    char *vp;
    int wordLength;

    vp = FindFirstVowel(word);
    wordLength = strlen(word);
    if (vp == word) {
        wordLength += 3;
    } else if (vp != NULL) {
        wordLength += 2;
    }
    if (wordLength >= bufferSize) Error("Buffer overflow");
    if (vp == NULL) {
        strcpy(buffer, word);
    } else if (vp == word) {
        strcpy(buffer, word);
        strcat(buffer, "way");
    } else {
        strcpy(buffer, vp);
        strncat(buffer, word, vp - word);
        strcat(buffer, "ay");
    }
}

/*
 * Function: FindFirstVowel
 * Usage: vp = FindFirstVowel(word);
 * ---------------------------------
 * This function returns a pointer of the first vowel in word.
 * If word does not contain a vowel, FindFirstVowel returns NULL.
 */

static char *FindFirstVowel(char *word)
{
    char *cp;

    for (cp = word; *cp != '\0'; cp++) {
        if (IsVowel(*cp)) return (cp);
    }
    return (NULL);
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
