/*
 * File: iterator.h
 * ----------------
 * Many abstract data types -- including those for such common
 * structures as sets and symbol tables -- represent collections
 * of values.  In many cases, clients need to perform some
 * operation on each of the values in that collection.  This
 * file provides a polymorphic interface for iterators, which
 * provide that capability in a very general way.
 */

#ifndef _iterator_h
#define _iterator_h

#include "genlib.h"

/*
 * Type: iteratorADT
 * -----------------
 * This abstract type is used to iterate over the elements
 * of any collection.
 */

typedef struct iteratorCDT *iteratorADT;

/* Exported entries */

/*
 * Functions: NewIterator, StepIterator, FreeIterator
 * Usage: iterator = NewIterator(collection);
 *        while (StepIterator(iterator, &element)) {
 *            . . . body of loop involving element . . .
 *        }
 *        FreeIterator(iterator);
 * -----------------------------------------------------
 * These functions make it possible to iterate over the
 * elements in any collection that supports iteration.  Each
 * call to StepIterator advances the iterator and returns the
 * next element using the reference parameter.  StepIterator
 * returns TRUE until the keys are exhausted, after which it
 * returns FALSE.  The FreeIterator function releases any
 * storage associated with the iterator.
 */

iteratorADT NewIterator(void *collection);
bool StepIterator(iteratorADT iterator, void *ep);
void FreeIterator(iteratorADT iterator);

#endif
