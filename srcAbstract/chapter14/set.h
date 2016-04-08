/*
 * File: set.h
 * -----------
 * This interface exports the type setADT, which can be used
 * to represent sets of objects.  The objects themselves can
 * be either of the following two types:
 *
 * int     This base type makes it easy to represent sets
 *         of integers (or any other scalar type), which come
 *         up frequently in practice.
 *
 * void *  This base type makes it possible for clients to
 *         use the set package with more sophisticated
 *         client-defined types.  The only requirement is that
 *         the client must supply a comparison function that
 *         compares two elements of the specified type.
 *
 * These two base types define two classes of sets, IntSet and
 * PtrSet.  The class of the set is determined when the set is
 * created, and the interface in some cases provides separate
 * functions for dealing with sets of each class.
 *
 * In addition to the set operations shown in this interface,
 * the set type allows iteration, as described in the iterator.h
 * interface.  The standard iterator idiom looks like this:
 *
 *     iterator = NewIterator(s);
 *     while (StepIterator(iterator, &x)) {
 *        . . . body of loop involving x . . .
 *     }
 *     FreeIterator(iterator);
 */

#ifndef _set_h
#define _set_h

#include "genlib.h"
#include "cmpfn.h"

/*
 * Type: setClassT
 * ---------------
 * This enumeration type defines the two possible set classes.
 */

typedef enum { IntSet, PtrSet } setClassT;

/*
 * Type: setADT
 * ------------
 * This type defines the abstract set type.  Depending on how
 * you initialize it, a set may be of either class.
 */

typedef struct setCDT *setADT;

/* Exported entries */

/*
 * Functions: NewIntSet, NewPtrSet
 * Usage: set = NewIntSet();
 *        set = NewPtrSet(cmpFn);
 * ------------------------------
 * These functions are used to create empty setADT values of the
 * specified class.  The NewIntSet function creates sets capable
 * of holding integers; the NewPtrSet function creates a more
 * general set type capable of holding client-specified types.
 */

setADT NewIntSet(void);
setADT NewPtrSet(cmpFnT cmpFn);

/*
 * Function: FreeSet
 * Usage: FreeSet(set);
 * --------------------
 * This function frees the storage associated with set, which
 * may be of either class.
 */

void FreeSet(setADT set);

/*
 * Functions: GetSetClass, GetCompareFunction
 * Usage: class = GetSetClass(set);
 *        fn = GetCompareFunction(set);
 * ------------------------------------
 * These functions return the set class and the comparison
 * function for an existing set.
 */

setClassT GetSetClass(setADT set);
cmpFnT GetCompareFunction(setADT set);

/*
 * Function: NElements
 * Usage: n = NElements(set);
 * --------------------------
 * This function returns the number of elements in the set.
 */

int NElements(setADT set);

/*
 * Function: SetIsEmpty
 * Usage: if (SetIsEmpty(set)) . . .
 * ---------------------------------
 * This function returns TRUE if the set has no elements.
 */

bool SetIsEmpty(setADT set);

/*
 * Functions: AddIntElement, AddPtrElement
 * Usage: AddIntElement(set, element);
 *        AddPtrElement(set, element);
 * ---------------------------------------
 * These functions each add a new element to an existing set
 * and differ only in the type of that element.
 */

void AddIntElement(setADT set, int element);
void AddPtrElement(setADT set, void *element);

/*
 * Function: AddElement
 * Usage: AddElement(set, element);
 * --------------------------------
 * This function adds an element to the set, using the set class
 * to determine the addressing structure of the element.
 */

void AddElement(setADT set, ...);

/*
 * Function: AddArrayToSet
 * Usage: AddArrayToSet(set, array, n);
 * ------------------------------------
 * This function adds the elements in the specified array to the
 * set.  The parameter n gives the effective size of the array.
 * The client is responsible for ensuring that the type of the
 * array matches the element type of the set.
 */

void AddArrayToSet(setADT set, void *array, int n);

/*
 * Functions: DeleteIntElement, DeletePtrElement
 * Usage: DeleteIntElement(set, element);
 *        DeletePtrElement(set, element);
 * ---------------------------------------------
 * These functions delete the element from the set, if it exists.
 */

void DeleteIntElement(setADT set, int element);
void DeletePtrElement(setADT set, void *element);

/*
 * Function: DeleteElement
 * Usage: DeleteElement(set, element);
 * -----------------------------------
 * This function deletes an element from the set, using the set
 * class to determine the addressing structure of the element.
 */

void DeleteElement(setADT set, ...);

/*
 * Functions: IsIntElement, IsPtrElement
 * Usage: if (IsIntElement(set, element)) . . .
 *        if (IsPtrElement(set, element)) . . .
 * --------------------------------------------
 * These functions return TRUE if the element is in the set.
 */

bool IsIntElement(setADT set, int element);
bool IsPtrElement(setADT set, void *element);

/*
 * Function: IsElement
 * Usage: if (IsElement(set, element)) . . .
 * -----------------------------------------
 * This function returns TRUE if the element is in the set.
 */

bool IsElement(setADT set, ...);

/*
 * Functions: SetEqual, IsSubset
 * Usage: if (SetEqual(s1, s2)) . . .
 *        if (IsSubset(s1, s2)) . . .
 * ----------------------------------
 * These predicate functions implement the equality and subset
 * relations on sets, respectively.  SetEqual(s1, s2) returns
 * TRUE if s1 and s2 have the same elements.  IsSubset(s1, s2)
 * returns TRUE if all elements of s1 are also elements of s2.
 */

bool SetEqual(setADT s1, setADT s2);
bool IsSubset(setADT s1, setADT s2);

/*
 * Functions: Union, Intersection, SetDifference
 * Usage: set = Union(s1, s2);
 *        set = Intersection(s1, s2);
 *        set = SetDifference(s1, s2);
 * ---------------------------------------------
 * These functions each return a new set, as follows:
 *
 * Union(s1, s2)          All elements in either s1 or s2.
 * Intersection(s1, s2)   All elements in both s1 and s2.
 * SetDifference(s1, s2)  All elements in s1 but not in s2.
 */

setADT Union(setADT s1, setADT s2);
setADT Intersection(setADT s1, setADT s2);
setADT SetDifference(setADT s1, setADT s2);

#endif
