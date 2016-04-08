/*
 * File: settest.c
 * ---------------
 * This program performs a simple test of the set abstraction
 * by calling the basic functions on some constant sets.
 */

#include <stdio.h>
#include "genlib.h"
#include "foreach.h"
#include "iterator.h"
#include "set.h"

#define DisplaySet(set) DisplaySetWithLabel(set, #set)
static void DisplaySetWithLabel(setADT set, string label);
static int SumIntSet(setADT set);

main()
{
    setADT s13579, s2468, s1248, s2357, s23, s1234;

    s13579 = NewIntSet();
    AddIntElement(s13579, 1);
    AddIntElement(s13579, 3);
    AddIntElement(s13579, 5);
    AddIntElement(s13579, 7);
    AddIntElement(s13579, 9);
    s2468 = NewIntSet();
    AddIntElement(s2468, 2);
    AddIntElement(s2468, 4);
    AddIntElement(s2468, 6);
    AddIntElement(s2468, 8);
    s1248 = NewIntSet();
    AddIntElement(s1248, 1);
    AddIntElement(s1248, 2);
    AddIntElement(s1248, 4);
    AddIntElement(s1248, 8);
    s2357 = NewIntSet();
    AddIntElement(s2357, 2);
    AddIntElement(s2357, 3);
    AddIntElement(s2357, 5);
    AddIntElement(s2357, 7);
    s23 = NewIntSet();
    AddIntElement(s23, 2);
    AddIntElement(s23, 3);
    s1234 = NewIntSet();
    AddIntElement(s1234, 1);
    AddIntElement(s1234, 2);
    AddIntElement(s1234, 3);
    AddIntElement(s1234, 4);
    DisplaySet(Union(s13579, s2468));
    DisplaySet(Union(s1248, s2357));
    DisplaySet(Union(s23, s1234));
    printf("\n");
    DisplaySet(Intersection(s13579, s2468));
    DisplaySet(Intersection(s1248, s2357));
    DisplaySet(Intersection(s23, s1234));
    printf("\n");
    DisplaySet(SetDifference(s13579, s2468));
    DisplaySet(SetDifference(s1248, s2357));
    DisplaySet(SetDifference(s23, s1234));
    printf("\n");
    printf("SumIntSet(s13579) = %d\n", SumIntSet(s13579));
}

static void DisplaySetWithLabel(setADT set, string label)
{
    iteratorADT iterator;
    int x;
    bool first;

    printf("%s = { ", label);
    iterator = NewIterator(set);
    first = TRUE;
    while (StepIterator(iterator, &x)) {
        if (!first) printf(", ");
        first = FALSE;
        printf("%d", x);
    }
    printf(" }\n");
}

int SumIntSet(setADT set)
{
    int x, sum;

    sum = 0;
    foreach (x in set) {
        sum += x;
    }
    return (sum);
}


