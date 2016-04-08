/*
 * File: cmpfn.h
 * -------------
 * This interface exports several comparison functions for use
 * with ANSI library functions like qsort and bsearch as well
 * as various functions in the extended library.
 */

#ifndef _cmpfn_h
#define _cmpfn_h

/*
 * Type: cmpFnT
 * ------------
 * This type defines the type space of comparison functions,
 * each of which take the addresses of their arguments and
 * return an integer from the set {-1, 0, +1} depending on
 * whether the first argument is less than, equal to, or
 * greater than the second.
 */

typedef int (*cmpFnT)(const void *p1, const void *p2);

/*
 * Standard comparison functions
 * -----------------------------
 * The remainder of this interface exports standard comparison
 * functions for the most common built-in types.
 */

int IntCmpFn(const void *p1, const void *p2);
int ShortCmpFn(const void *p1, const void *p2);
int LongCmpFn(const void *p1, const void *p2);
int CharCmpFn(const void *p1, const void *p2);
int FloatCmpFn(const void *p1, const void *p2);
int DoubleCmpFn(const void *p1, const void *p2);
int UnsignedCmpFn(const void *p1, const void *p2);
int UnsignedShortCmpFn(const void *p1, const void *p2);
int UnsignedLongCmpFn(const void *p1, const void *p2);
int UnsignedCharCmpFn(const void *p1, const void *p2);
int StringCmpFn(const void *p1, const void *p2);
int PtrCmpFn(const void *p1, const void *p2);

#endif
