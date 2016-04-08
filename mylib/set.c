/*
 * File: set.c
 * -------------
 * This file implements the set abstraction fefined in set.h.
 */
#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "bst.h" 
#include "iterator2.h"
#include "itertype.h"
#include "set.h"

/* 
 * Type: setCDT
 * -------------
 * This type defines the concrete structure of a set.
 */

struct setCDT{
	iteratorHeaderT header;
	setClassT Class;
	cmpFnT cmpFn;
	int nElements;
	bstADT bst;
};

typedef union{
	int intRep;
	void *ptrRep;
}setElementT;

/* Private function  prototypes */
static setADT NewSet(setClassT Class, cmpFnT cmpFn);
static void InitSetNodeFn(void *np, void *kp, void *clientData);
static void FreeNodeFn(void *np, void *clientData);
static void AddERef(setADT set, void *ep);
static void DeleteERef(setADT set, void *ep);
static bool TestERef(setADT set, void *ep);
static iteratorADT NewSetIterator(void *collection);
static void AddElementToIterator(void *np, void *clientData);

/* Exported entries */
setADT NewIntSet(void)
{
	return (NewSet(IntSet, IntCmpFn));
}

setADT NewPtrSet(cmpFnT cmpFn)
{
	return (NewSet(PtrSet, cmpFn));
}

static setADT NewSet(setClassT Class, cmpFnT cmpFn)
{
	setADT set;

	set = New(setADT);
	EnableIteration(set, NewSetIterator);
	set->Class = Class;
	set->cmpFn = cmpFn;
	set->nElements = 0;
	set->bst = NewBST(sizeof(setElementT), cmpFn, InitSetNodeFn);
	return (set);
}

static iteratorADT NewSetIterator(void *collection)
{
	setADT set = (setADT) collection;
	int elementSize;
	iteratorADT iterator;

	switch(set->Class){
	   case IntSet: elementSize = sizeof(int); break;
	   case PtrSet: elementSize = sizeof(void *); break;
	}
	iterator = NewIteratorList(elementSize, UnsortedFn);
	MapBST(AddElementToIterator, set->bst, InOrder, iterator);
	return iterator;
}

static void AddElementToIterator(void *np, void *clientData)
{
    AddToIteratorList( (iteratorADT)clientData, np);
}

static void InitSetNodeFn(void *np, void *kp, void *clientData)
{
    setADT set = (setADT) clientData;

    switch(set->Class){
        case IntSet: *((int *)np) = *((int *)kp);   break;
        case PtrSet: *((void**)np) = *((void**)kp); break;
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
    return (set->Class);
}

cmpFnT GetCompareFunction(setADT set)
{
    return (set->cmpFn);
}

int NElements(setADT set)
{
    return (set->nElements);
}

bool SetIsEmpty(setADT set)
{
    return (set->nElements == 0);
}

void AddIntElement(setADT set, int element)
{
    if(set->Class != IntSet) Error("Set is not an iteger set");
    AddERef(set, &element);
}

void AddPtrElement(setADT set, void *element)
{
    if(set->Class != PtrSet) Error("Set is not a pointer set");
    AddERef(set, element);
}

static void AddERef(setADT set, void *ep)
{
    (void) InsertBSTNode(set->bst, ep, set);
}

void DeleteIntElement(setADT set, int element)
{
    if(set->Class != IntSet) Error("Set if not an integer set");
    DeleteERef(set, &element);
}

void DeletePtrElement(setADT set, void *element)
{
    if(set->Class != PtrSet) Error("Set is not a pointer set");
    DeleteERef(set, element);
}
 
static void DeleteERef(setADT set, void *ep)
{
    if( DeleteBSTNode(set->bst, ep) )
        set->nElements--;
}

bool IsIntElement(setADT set, int element)
{
    if(set->Class != IntSet) Error("Set if not an integer set");
    return (TestERef(set, &element));
}

bool IsPtrment(setADT set, void *element)
{
    if(set->Class != PtrSet) Error("Set if not an pointer set");
    return (TestERef(set, element));
}

static bool TestERef(setADT set, void *ep)
{
    return (FindBSTNode(set->bst, ep) != NULL);
}

/*
 * Implementation notes: Set operations
 * ------------------------------------
 * The function IsSubset, Union, Intersection, and SetDifference
 * are similar in structure. Each one an iterator to walk over
 * the appropriate set, Because the functions in bst.h need only 
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
    if(s1->Class != s2->Class || s1->cmpFn != s2->cmpFn){
        Error ("IsSubset: Set types do not match");
    }
    result = TRUE;
    iterator = NewIterator(s1);
    while(result && StepIterator(iterator, (void *)&element)){
        if(!TestERef(s2, &element)) result = FALSE;
    }
    FreeIterator(iterator);
    return (result);
}

setADT Union(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if(s1->Class != s2->Class || s1->cmpFn != s2->cmpFn){
        Error("Union: Set types do not match");
    }
    set = NewSet(s1->Class, s1->cmpFn);
    iterator = NewIterator(s1);
    while(StepIterator(iterator, (void*)&element)){
        AddERef(set, &element);
    }
    FreeIterator(iterator);
    iterator = NewIterator(s2);
    while(StepIterator(iterator, (void*)&element)){
        AddERef(set, &element);
    }
    FreeIterator(iterator);
    return set;
}

setADT Intersection(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if(s1->Class != s2->Class || s1->cmpFn != s2->cmpFn){
        Error("Intersection: Set types do not match");
    }

    set = NewSet(s1->Class, s1->cmpFn);
    iterator = NewIterator(s1);
    while(StepIterator(iterator, (void*)&element)){
        if(TestERef(s2, &element)) AddERef(set, &element);
    }
    FreeIterator(iterator);
    return set;
}

setADT SetDifference(setADT s1, setADT s2)
{
    iteratorADT iterator;
    setElementT element;
    setADT set;

    if(s1->Class != s2->Class || s1->cmpFn != s2->cmpFn){
        Error("SetDifference: Set types do not match");
    }
    set = NewSet(s1->Class, s1->cmpFn);
    iterator = NewIterator(s1);
    while(StepIterator(iterator, (void*)&element)){
        if(!TestERef(s2, &element)) AddERef(set, &element);
    }
    FreeIterator(iterator);
    return set;
}
