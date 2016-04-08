/*
 * File: set.c
 * -----------
 * This file implements the set abstraction defined in set.h.
 */

#include <stdio.h>
#include <stdarg.h>
#include "genlib.h"
#include "bst.h"
#include "itertype.h"
#include "iterator.h"
#include "set.h"

/*
 * Type: setCDT
 * ------------
 * This type defines the concrete structure of a set.
 */

struct setCDT {
    iteratorHeaderT header;
    setClassT class;
    cmpFnT cmpFn;
    int nElements;
    bstADT bst;
};

/*
 * Type: setElementT
 * -----------------
 * This union type combines the two representations of an element.
 * It is used in the implementation to allocate storage for an
 * element that is guaranteed to be large enough no matter whether
 * the set contains integers or pointers.  This type is used only
 * to reduce the number of special cases in the code and is not
 * exported to the client.
 */

typedef union {
    int intRep;
    void *ptrRep;
} setElementT;

/* Private function prototypes */

static setADT NewSet(setClassT class, cmpFnT cmpFn);
static void InitSetNodeFn(void *np, void *kp, void *clientData);
static void FreeNodeFn(void *np, void *clientData);
static void AddERef(setADT set, void *ep);
static void DeleteERef(setADT set, void *ep);
static bool TestERef(setADT set, void *ep);
static iteratorADT NewSetIterator(void *collection);
static void AddElementToIterator(void *np, void *clientData);

/* Exported entries */

/*
 * Implementation notes: NewIntSet, NewPtrSet
 * ------------------------------------------
 * The contructor functions call a common function that creates
 * a new set from a class and a comparison function.  The
 * elements in the set are stored in a binary search tree.
 */

setADT NewIntSet(void)
{
    return (NewSet(IntSet, IntCmpFn));
}

setADT NewPtrSet(cmpFnT cmpFn)
{
    return (NewSet(PtrSet, cmpFn));
}

static setADT NewSet(setClassT class, cmpFnT cmpFn)
{
    setADT set;

    set = New(setADT);
    EnableIteration(set, NewSetIterator);
    set->class = class;
    set->cmpFn = cmpFn;
    set->nElements = 0;
    set->bst = NewBST(sizeof (setElementT), cmpFn, InitSetNodeFn);
    return (set);
}

static void InitSetNodeFn(void *np, void *kp, void *clientData)
{
    setADT set = (setADT) clientData;

    switch (set->class) {
      case IntSet: *((int *) np) = *((int *) kp); break;
      case PtrSet: *((void **) np) = *((void **) kp); break;
    }
    set->nElements++;
}

void FreeSet(setADT set)
{
    FreeBST(set->bst, FreeNodeFn);
    FreeBlock(set);
}

static void FreeNodeFn(void *np, void *clientData)
{
    FreeBlock(np);
}

/* Selection functions */

setClassT GetSetClass(setADT set)
{
    return (set->class);
}

cmpFnT GetCompareFunction(setADT set)
{
    return (set->cmpFn);
}

/*
 * Implementation notes: NElements, SetIsEmpty
 * -------------------------------------------
 * For efficiency, this package stores the number of elements
 * as part of the set data structure, updating it as necessary.
 */

int NElements(setADT set)
{
    return (set->nElements);
}

bool SetIsEmpty(setADT set)
{
    return (set->nElements == 0);
}

/*
 * Implementation notes: AddIntElement, AddPtrElement
 * --------------------------------------------------
 * These functions call a common function AddERef, which takes
 * a pointer to the element.
 */


void AddIntElement(setADT set, int element)
{
    if (set->class != IntSet) Error("Set is not an integer set");
    AddERef(set, &element);
}

void AddPtrElement(setADT set, void *element)
{
    if (set->class != PtrSet) Error("Set is not a pointer set");
    AddERef(set, &element);
}

void AddElement(setADT set, ...)
{
    va_list args;
    int n;
    void *p, *np, *ep;

    va_start(args, set);
    switch (set->class) {
      case IntSet: n = va_arg(args, int); ep = &n; break;
      case PtrSet: p = va_arg(args, void *); ep = &p; break;
    }
    va_end(args);
    AddERef(set, ep);
}

static void AddERef(setADT set, void *ep)
{
    (void) InsertBSTNode(set->bst, ep, set);
}

/*
 * Implementation note: AddArrayToSet
 * ----------------------------------
 * This function calls AddERef for each element in the array.
 * Note that the addressing depends on the set class.
 */

void AddArrayToSet(setADT set, void *array, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        switch (set->class) {
          case IntSet: AddERef(set, ((int *) array) + i); break;
          case PtrSet: AddERef(set, ((void **) array) + i); break;
        }
    }
}

/*
 * Implementation notes: DeleteIntElement, DeletePtrElement
 * --------------------------------------------------------
 * These functions simply check to see that the set type is
 * appropriate and then call a common function.
 */

void DeleteIntElement(setADT set, int element)
{
    if (set->class != IntSet) Error("Set is not an integer set");
    DeleteERef(set, &element);
}

void DeletePtrElement(setADT set, void *element)
{
    if (set->class != PtrSet) Error("Set is not a pointer set");
    DeleteERef(set, &element);
}

void DeleteElement(setADT set, ...)
{
    va_list args;
    int n;
    void *p, *np, *ep;

    va_start(args, set);
    switch (set->class) {
      case IntSet: n = va_arg(args, int); ep = &n; break;
      case PtrSet: p = va_arg(args, void *); ep = &p; break;
    }
    va_end(args);
    DeleteERef(set, ep);
}

static void DeleteERef(setADT set, void *ep)
{
    void *np;

    np = DeleteBSTNode(set->bst, ep);
    if (np != NULL) {
        FreeBlock(np);
        set->nElements--;
    }
}

/*
 * Implementation notes: IsIntElement, IsPtrElement
 * ------------------------------------------------
 * These implementations call a common TestERef function, which
 * in turn calls FindBSTNode to look up the element.
 */

bool IsIntElement(setADT set, int element)
{
    if (set->class != IntSet) Error("Set is not an integer set");
    return (TestERef(set, &element));
}

bool IsPtrElement(setADT set, void *element)
{
    if (set->class != PtrSet) Error("Set is not a pointer set");
    return (TestERef(set, &element));
}

bool IsElement(setADT set, ...)
{
    va_list args;
    int n;
    void *p, *np, *ep;

    va_start(args, set);
    switch (set->class) {
      case IntSet: n = va_arg(args, int); ep = &n; break;
      case PtrSet: p = va_arg(args, void *); ep = &p; break;
    }
    va_end(args);
    return (TestERef(set, ep));
}

static bool TestERef(setADT set, void *ep)
{
    return (FindBSTNode(set->bst, ep) != NULL);
}

/*
 * Implementation notes: Set operations
 * ------------------------------------
 * The functions IsSubset, Union, Intersection, and SetDifference
 * are similar in structure.  Each one uses an iterator to walk over
 * the appropriate set.  Because the functions in bst.h need only
 * the address of an element, the functions can use the union type
 * setElementT to avoid special-case code for the two set classes.
 */

bool SetEqual(setADT s1, setADT s2)
{
    return (IsSubset(s1, s2) && IsSubset(s2, s1));
}

bool IsSubset(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    bool result;

    if (s1->class != s2->class || s1->cmpFn != s2->cmpFn) {
        Error("IsSubset: Set types do not match");
    }
    result = TRUE;
    iterator = NewIterator(s1);
    while (result && StepIterator(iterator, &element)) {
        if (!TestERef(s2, &element)) result = FALSE;
    }
    FreeIterator(iterator);
    return (result);
}

setADT Union(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if (s1->class != s2->class || s1->cmpFn != s2->cmpFn) {
        Error("Union: Set types do not match");
    }
    set = NewSet(s1->class, s1->cmpFn);
    iterator = NewIterator(s1);
    while (StepIterator(iterator, &element)) {
        AddERef(set, &element);
    }
    FreeIterator(iterator);
    iterator = NewIterator(s2);
    while (StepIterator(iterator, &element)) {
        AddERef(set, &element);
    }
    FreeIterator(iterator);
    return (set);
}

setADT Intersection(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if (s1->class != s2->class || s1->cmpFn != s2->cmpFn) {
        Error("Intersection: Set types do not match");
    }
    set = NewSet(s1->class, s1->cmpFn);
    iterator = NewIterator(s1);
    while (StepIterator(iterator, &element)) {
        if (TestERef(s2, &element)) AddERef(set, &element);
    }
    FreeIterator(iterator);
    return (set);
}

setADT SetDifference(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if (s1->class != s2->class || s1->cmpFn != s2->cmpFn) {
        Error("SetDifference: Set types do not match");
    }
    set = NewSet(s1->class, s1->cmpFn);
    iterator = NewIterator(s1);
    while (StepIterator(iterator, &element)) {
        if (!TestERef(s2, &element)) AddERef(set, &element);
    }
    FreeIterator(iterator);
    return (set);
}

/*
 * Implementation notes: NewSetIterator, AddElementToIterator
 * ----------------------------------------------------------
 * These functions make it possible to use the general iterator
 * facility on sets.  For details on the general strategy, see
 * the comments in the itertype.h interface.  The comparison
 * function passed to NewIteratorList is UnsortedFn because the
 * InOrder walk already guarantees that the elements will appear
 * in sorted order.
 */

static iteratorADT NewSetIterator(void *collection)
{
    setADT set = collection;
    int elementSize;
    iteratorADT iterator;

    switch (set->class) {
      case IntSet: elementSize = sizeof(int); break;
      case PtrSet: elementSize = sizeof(void *); break;
    }
    iterator = NewIteratorList(elementSize, UnsortedFn);
    MapBST(AddElementToIterator, set->bst, InOrder, iterator);
    return (iterator);
}

static void AddElementToIterator(void *np, void *clientData)
{
    AddToIteratorList((iteratorADT) clientData, np);
}
