/*
 * File: strtest.c
 * ---------------
 * This program tests the set abstraction.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "iterator.h"
#include "set.h"

static string monthNames[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August", "September",
    "October", "November", "December"
};
static string shorterNames[] = {
    "September", "April", "June", "November", "February"
};
static string firstNames[] = {
    "January", "February", "March", "April", "May", "June"
};

#define DisplaySet(set) DisplaySetWithLabel(set, #set)
static void DisplaySetWithLabel(setADT set, string label);

main()
{
    setADT months, shorter, first;

    months = NewPtrSet(StringCmpFn);
    AddArrayToSet(months, monthNames, sizeof monthNames / sizeof (string));
    shorter = NewPtrSet(StringCmpFn);
    AddArrayToSet(shorter, shorterNames, sizeof shorterNames / sizeof (string));
    first = NewPtrSet(StringCmpFn);
    AddArrayToSet(first, firstNames, sizeof firstNames / sizeof (string));
    DisplaySet(shorter);
    DisplaySet(first);
    DisplaySet(Intersection(shorter, first));
    DisplaySet(Union(shorter, first));
}

static void DisplaySetWithLabel(setADT set, string label)
{
    iteratorADT iterator;
    string x;
    bool first;

    printf("%s:\n  {", label);
    iterator = NewIterator(set);
    first = TRUE;
    while (StepIterator(iterator, &x)) {
        if (!first) printf(", ");
        first = FALSE;
        printf("%s", x);
    }
    printf("}\n");
}
