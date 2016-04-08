/*
 * File: permute.c
 * ---------------
 * This file implements and tests the ListPermutation function.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "iterator.h"
#include "cmpfn.h"
#include "set.h"

/* Private function prototypes */

static void ListPermutations(string str);
static void RecursivePermute(string str, int k, setADT set);
static void ExchangeCharacters(string str, int p1, int p2);

/* Main program */

main()
{
    string str;

    printf("This program lists unique permutations.\n");
    printf("Enter a string: ");
    str = GetLine();
    ListPermutations(str);
}

/*
 * Function: ListPermutations
 * Usage: ListPermutations(str)
 * ----------------------------
 * This function lists all permutations of the characters in the
 * string str.  If the same string is generated more than once
 * in the course of the algorithm, each of those permutations is
 * listed only once.
 */

static void ListPermutations(string str)
{
    setADT set;
    iteratorADT iterator;
    string s;

    set = NewPtrSet(StringCmpFn);
    RecursivePermute(str, 0, set);
    iterator = NewIterator(set);
    while (StepIterator(iterator, &s)) {
        printf("%s\n", s);
    }
    FreeIterator(iterator);
}

/*
 * Function: RecursivePermute
 * Usage: RecursivePermute(str, k, set);
 * -------------------------------------
 * This function implements the recursive permutation algorithm,
 * adding each permutation to the set as it goes.
 */

static void RecursivePermute(string str, int k, setADT set)
{
    int i;

    if (k == StringLength(str)) {
        AddPtrElement(set, CopyString(str));
    } else {
        for (i = k; i < StringLength(str); i++) {
            ExchangeCharacters(str, k, i);
            RecursivePermute(str, k + 1, set);
            ExchangeCharacters(str, k, i);
        }
    }
}

/*
 * Function: ExchangeCharacters
 * Usage: ExchangeCharacters(str, p1, p2);
 * ---------------------------------------
 * This function exchanges the characters at positions p1
 * and p2 of the string s.
 */

static void ExchangeCharacters(string str, int p1, int p2)
{
    char tmp;

    tmp = str[p1];
    str[p1] = str[p2];
    str[p2] = tmp;
}
