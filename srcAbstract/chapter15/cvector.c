/*
 * File: cvector.c
 * ---------------
 * This file implements the characteristic vector operations
 * exported by cvector.h.
 */

#include <stdio.h>
#include "genlib.h"
#include "cvector.h"

/*
 * Constants: BitsPerLong, CVecWords
 * ---------------------------------
 * These constants determine the size of the concrete structure
 * used to represent a cVectorADT.  The constant BitsPerLong is
 * the number of bits in a long word; CVecWords is the number
 * of long words required to provide one bit for each element
 * in a set of size RangeSize.  Note that the calculation of
 * CVecWords must round up to the nearest word boundary.
 */

#define BitsPerByte 8
#define BitsPerLong (BitsPerByte * sizeof (long))
#define CVecWords ((RangeSize + BitsPerLong - 1) / BitsPerLong)

/*
 * Type: cVectorCDT
 * ----------------
 * This type is the concrete representation of a cVectorADT.
 * The underlying structure is an array of long words that has
 * enough bits to designate one bit for each integer in the
 * legal range from 0 to RangeSize - 1.
 */

struct cVectorCDT {
    unsigned long words[CVecWords];
};

/* Private function prototypes */

static unsigned long BitMask(int k);

/* Exported entries */

cVectorADT NewCVector(void)
{
    cVectorADT cv;
    int i;

    cv = New(cVectorADT);
    for (i = 0; i < CVecWords; i++) {
        cv->words[i] = 0;
    }
    return (cv);
}

void FreeCVector(cVectorADT cv)
{
    FreeBlock(cv);
}

/*
 * Implementation notes: CVectorSet, CVectorClear, CVectorTest
 * -----------------------------------------------------------
 * These functions are quite similar in their implementation.
 * Each function simply calculates one of the bit operations on
 * the appropriate element of the characteristic vector array
 * and a mask that has a 1 bit in the appropriate position.
 */

void CVectorSet(cVectorADT cv, int k)
{
    if (k < 0 || k >= RangeSize) {
        Error("CVectorSet: Value is out of range");
    }
    cv->words[k / BitsPerLong] |= BitMask(k);
}

void CVectorClear(cVectorADT cv, int k)
{
    if (k < 0 || k >= RangeSize) {
        Error("CVectorClear: Value is out of range");
    }
    cv->words[k / BitsPerLong] &= ~BitMask(k);
}

bool CVectorTest(cVectorADT cv, int k)
{
    if (k < 0 || k >= RangeSize) {
        Error("CVectorTest: Value is out of range");
    }
    return ((cv->words[k / BitsPerLong] & BitMask(k)) != 0);
}

/*
 * Implementation notes: CVector set operations
 * --------------------------------------------
 * These functions CVectorUnion, CVectorIntersection, and
 * CVectorDifference compute their results very efficiently
 * by performing bitwise operation on entire words.
 */

cVectorADT CVectorUnion(cVectorADT cv1, cVectorADT cv2)
{
    cVectorADT result;
    int i;

    result = New(cVectorADT);
    for (i = 0; i < CVecWords; i++) {
        result->words[i] = cv1->words[i] | cv2->words[i];
    }
    return (result);
}

cVectorADT CVectorIntersection(cVectorADT cv1, cVectorADT cv2)
{
    cVectorADT result;
    int i;

    result = New(cVectorADT);
    for (i = 0; i < CVecWords; i++) {
        result->words[i] = cv1->words[i] & cv2->words[i];
    }
    return (result);
}

cVectorADT CVectorDifference(cVectorADT cv1, cVectorADT cv2)
{
    cVectorADT result;
    int i;

    result = New(cVectorADT);
    for (i = 0; i < CVecWords; i++) {
        result->words[i] = cv1->words[i] & ~cv2->words[i];
    }
    return (result);
}

/* Private functions */

/*
 * Function: BitMask
 * Usage: mask = BitMask(k);
 * -------------------------
 * This function returns an unsigned long value in which the bit
 * corresponding to the value k is set and all others are clear.
 */

unsigned long BitMask(int k)
{
    return ((unsigned long) 1 << (k % BitsPerLong));
}
