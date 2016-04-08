/*
 * File: cvecset.c
 * ---------------
 * This file implements the set.h interface using the cvector.h
 * package so that the same test programs can be used for both
 * implementations.
 */

#include <stdio.h>
#include "genlib.h"
#include "itertype.h"
#include "iterator.h"
#include "cvector.h"
#include "set.h"

/* Define the concrete type */

struct setCDT {
    iteratorHeaderT header;
    cVectorADT cvec;
};

/* Private function prototypes */

static setADT SetFromCVector(cVectorADT cvec);
static iteratorADT NewSetIterator(void *collection);

/* Exported entries */

setADT NewIntSet(void)
{
    return (SetFromCVector(NewCVector()));
}

setADT NewPtrSet(cmpFnT cmpFn)
{
    Error("NewPtrSet: Not implemented in CVector version");
}

void FreeSet(setADT set)
{
    FreeCVector(set->cvec);
}

void AddIntElement(setADT set, int element)
{
    CVectorSet(set->cvec, element);
}

void AddPtrElement(setADT set, void *element)
{
    Error("AddPtrElement: Not implemented in CVector version");
}

void AddArrayToSet(setADT set, void *array, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        CVectorSet(set->cvec, ((int *) array)[i]);
    }
}

void DeleteIntElement(setADT set, int element)
{
    CVectorClear(set->cvec, element);
}

void DeletePtrElement(setADT set, void *element)
{
    Error("DeletePtrElement: Not implemented in CVector version");
}

int NElements(setADT set)
{
    int i, n;

    n = 0;
    for (i = 0; i < RangeSize; i++) {
        if (CVectorTest(set->cvec, i)) n++;
    }
    return (n);
}

bool IsIntElement(setADT set, int element)
{
    return (CVectorTest(set->cvec, element));
}

bool IsPtrElement(setADT set, void *element)
{
    Error("IsPtrElement: Not implemented in CVector version");
}

bool IsSubset(setADT s1, setADT s2)
{
    int i;
    cVectorADT cv1 = s1->cvec;
    cVectorADT cv2 = s2->cvec;

    for (i = 0; i < RangeSize; i++) {
        if (CVectorTest(cv1, i) && !CVectorTest(cv2, i)) {
            return (FALSE);
        }
    }
    return (TRUE);
}

setADT Union(setADT s1, setADT s2)
{
    return (SetFromCVector(CVectorUnion(s1->cvec, s2->cvec)));
}

setADT Intersection(setADT s1, setADT s2)
{
    return (SetFromCVector(CVectorIntersection(s1->cvec, s2->cvec)));
}

setADT SetDifference(setADT s1, setADT s2)
{
    return (SetFromCVector(CVectorDifference(s1->cvec, s2->cvec)));
}

/* Private functions */

static setADT SetFromCVector(cVectorADT cvec)
{
    setADT set;

    set = New(setADT);
    EnableIteration(set, NewSetIterator);
    set->cvec = cvec;
    return (set);
}

static iteratorADT NewSetIterator(void *collection)
{
    setADT set = collection;
    iteratorADT iterator;
    int i;

    iterator = NewIteratorList(sizeof (int), UnsortedFn);
    for (i = 0; i < RangeSize; i++) {
        if (CVectorTest(set->cvec, i)) {
            AddToIteratorList(iterator, &i);
        }
    }
    return (iterator);
}
