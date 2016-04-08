/*
 * File: cmpfn.c
 * -------------
 * This file implements the comparison functions exported by
 * the cmpfn.h interface.  The reason to centralize them in
 * a single module is to allow the same functions to be
 * shared among many different modules.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "cmpfn.h"

/* Exported entries */

int IntCmpFn(const void *p1, const void *p2)
{
    int v1, v2;

    v1 = *((int *) p1);
    v2 = *((int *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int ShortCmpFn(const void *p1, const void *p2)
{
    short v1, v2;

    v1 = *((short *) p1);
    v2 = *((short *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int LongCmpFn(const void *p1, const void *p2)
{
    long v1, v2;

    v1 = *((long *) p1);
    v2 = *((long *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int UnsignedCmpFn(const void *p1, const void *p2)
{
    unsigned v1, v2;

    v1 = *((unsigned *) p1);
    v2 = *((unsigned *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int UnsignedShortCmpFn(const void *p1, const void *p2)
{
    unsigned short v1, v2;

    v1 = *((unsigned short *) p1);
    v2 = *((unsigned short *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int UnsignedLongCmpFn(const void *p1, const void *p2)
{
    unsigned long v1, v2;

    v1 = *((unsigned long *) p1);
    v2 = *((unsigned long *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int CharCmpFn(const void *p1, const void *p2)
{
    unsigned char v1, v2;

    v1 = *((unsigned char *) p1);
    v2 = *((unsigned char *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int FloatCmpFn(const void *p1, const void *p2)
{
    float v1, v2;

    v1 = *((float *) p1);
    v2 = *((float *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int DoubleCmpFn(const void *p1, const void *p2)
{
    double v1, v2;

    v1 = *((double *) p1);
    v2 = *((double *) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}

int StringCmpFn(const void *p1, const void *p2)
{
    return (StringCompare(*((string *) p1), *((string *) p2)));
}

int PtrCmpFn(const void *p1, const void *p2)
{
    void *v1, *v2;

    v1 = *((void **) p1);
    v2 = *((void **) p2);
    if (v1 == v2) return (0);
    return ((v1 < v2) ? -1 : 1);
}
