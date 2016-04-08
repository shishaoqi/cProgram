/*
 * File: cvector.h
 * ---------------
 * This interface exports functions that implement set operations
 * on integers using a highly efficient representation called
 * characteristic vectors, which is logically an array of bits.
 * This interface, however, does not provide a general set
 * facility because the characteristic-vector representation
 * limits the range of integers that can be stored.   Despite
 * the lack of generality, this interface is extremely useful
 * as the basis for higher-level packages.
 */

#ifndef _cvector_h
#define _cvector_h

/*
 * Type: cVectorADT
 * ----------------
 * This type defines the ADT for a characteristic vector.
 */

typedef struct cVectorCDT *cVectorADT;

/*
 * Constant: RangeSize
 * -------------------
 * This constant specifies the range of integers that can be
 * stored in a set implemented as a characteristic vector.  The
 * values are restricted to the interval [0, RangeSize - 1].
 */

#define RangeSize 256

/* Exported entries */

cVectorADT NewCVector(void);
void FreeCVector(cVectorADT cv);

void CVectorSet(cVectorADT cv, int k);
void CVectorClear(cVectorADT cv, int k);
bool CVectorTest(cVectorADT cv, int k);

cVectorADT CVectorUnion(cVectorADT cv1, cVectorADT cv2);
cVectorADT CVectorIntersection(cVectorADT cv1, cVectorADT cv2);
cVectorADT CVectorDifference(cVectorADT cv1, cVectorADT cv2);

#endif
