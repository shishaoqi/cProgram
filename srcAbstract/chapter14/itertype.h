/*
 * File: itertype.h
 * ----------------
 * This file provides an interface that package designers can
 * use to export new collection types that support iteration.
 * Clients of those collection types who merely want to use
 * iterators will not need to import this interface.
 */

#ifndef _itertype_h
#define _itertype_h

#include "genlib.h"
#include "iterator.h"
#include "cmpfn.h"

/*
 * General overview
 * ----------------
 * In order to create a collection type that supports iteration,
 * the implementor of the type must first allocate space at the
 * beginning of the concrete record for an iteratorHeaderT that
 * contains a function pointer which allows NewIterator to make
 * a new iterator for that collection type.  The implementor
 * must initialize this header in the constructor function by
 * calling EnableIteration.  The specific NewIterator functions
 * for each type must perform the following operations:
 *
 * 1. Call NewIteratorList to create an empty iterator.
 * 2. Call AddToIteratorList for each element.
 * 3. Return the completed iterator.
 *
 * Elements in the iterator may be sorted or unsorted depending
 * on the comparison function passed to NewIteratorList.
 */

/*
 * Type: newIteratorFnT
 * --------------------
 * This type represents the class of functions that create new
 * iterators.
 */

typedef iteratorADT (*newIteratorFnT)(void *collection);

/*
 * Constant function: UnsortedFn
 * -----------------------------
 * This constant creates a NULL function pointer that can be
 * used in place of the comparison function to indicate that the
 * values in the iterator should not be sorted.  The constant 0
 * is used in place of NULL because some compilers do not allow
 * casting NULL to a function pointer.
 */

#define UnsortedFn ((cmpFnT) 0)

/*
 * Type: iteratorHeaderT
 * ---------------------
 * This structure must appear at the beginning of any concrete
 * structure that supports iteration.
 */

typedef struct {
    unsigned long password;
    newIteratorFnT newFn;
} iteratorHeaderT;

/*
 * Functions: EnableIteration
 * Usage: EnableIteration(collection, newFn);
 * ------------------------------------------
 * This function enables iteration for the collection.  The
 * function pointer newFn is used to initialize an iterator
 * for this type.
 */

void EnableIteration(void *collection, newIteratorFnT newFn);

/*
 * Functions: NewIteratorList
 * Usage: iterator = NewIteratorList(sizeof (type), cmpFn);
 * --------------------------------------------------------
 * This function creates a new iterator with an empty iterator
 * list.  The first argument is the size of the element type,
 * which makes it possible for the package to allocate storage
 * for values of that type.  The cmpFn is the comparison function
 * used to sort the iterator elements.  If cmpFn is the constant
 * pointer UnsortedFn, the elements are entered in the order in
 * which AddToIteratorList is called.
 */

iteratorADT NewIteratorList(int size, cmpFnT cmpFn);

/*
 * Functions: AddToIteratorList
 * Usage: AddToIteratorList(iterator, &element);
 * ---------------------------------------------
 * This function takes an iterator and a pointer to an element
 * and inserts the element into the iterator list.
 */

void AddToIteratorList(iteratorADT iterator, void *ep);

#endif
