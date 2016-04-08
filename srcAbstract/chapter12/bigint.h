/*
 * File: bigint.h
 * --------------
 * This interface is a primitive version of a facility intended
 * to support integers of an arbitrary size. This package has the
 * following limitations, which significantly reduce its utility:
 *
 * 1. The package does not support negative integers.
 * 2. The only operations are addition and multiplication.
 * 3. It is inefficient in terms of both space and time.
 */

#ifndef _bigint_h
#define _bigint_h

#include "genlib.h"

/*
 * Type: bigIntADT
 * ---------------
 * This type is the abstract type used to store a nonnegative
 * integer of arbitrary size.
 */

typedef struct bigIntCDT *bigIntADT;

/*
 * Function: NewBigInt
 * Usage: n = NewBigInt(i);
 * ------------------------
 * This function creates a new bigIntADT with the value i.  Use
 * StringToBigInt to create bigIntADTs larger than an integer.
 */

bigIntADT NewBigInt(int i);

/*
 * Functions: StringToBigInt, BigIntToString
 * Usage: n = StringToBigInt(str);
 *        str = BigIntToString(n);
 * -----------------------------------------
 * These functions convert between strings of digits and values
 * of type bigIntADT.
 */

bigIntADT StringToBigInt(string str);
string BigIntToString(bigIntADT n);

/*
 * Function: AddBigInt
 * Usage: n = AddBigInt(n1, n2);
 * -----------------------------
 * This function allocates a new bigIntADT n whose value is
 * the sum of n1 and n2.
 */

bigIntADT AddBigInt(bigIntADT n1, bigIntADT n2);

/*
 * Function: MultiplyBigInt
 * Usage: n = MultiplyBigInt(n1, n2);
 * ----------------------------------
 * This function allocates a new bigIntADT n whose value is
 * the product of n1 and n2.
 */

bigIntADT MultiplyBigInt(bigIntADT n1, bigIntADT n2);

#endif
