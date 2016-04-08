/*
 * File: iterator.c
 * ----------------
 * This file implements a polymorphic version of the iterator
 * and the tools necessary to construct types that support
 * iteration.  This module implements both the iterator.h and
 * itertype.h interfaces.
 */

#include <stdio.h>
#include <string.h>
#include "genlib.h"
#include "cmpfn.h"
#include "iterator.h"
#include "itertype.h"

/*
 * Constant: IteratorPassword
 * --------------------------
 * This constant is stored in the header block of collection
 * types to catch the case when a client tries to call
 * NewIterator on a value for which iteration is not defined.
 */

#define IteratorPassword 3141592653UL

/*
 * Type: cellT, iteratorCDT
 * ------------------------
 * The iterator is implemented as a linked list chained
 * through the first word in the structure.  Because the size
 * of the actual data element can vary, the data field is not
 * represented as part of the structure.  Instead, each cell is
 * allocated dynamically with enough memory for both the link
 * field and the data value, however large it turns out to be.
 * The list is arranged like a queue, with a tail pointer to
 * its final element for efficiency.  The AddToIteratorList
 * function first checks to see whether the element belongs at
 * the end.  By checking for this case explicitly, it is often
 * possible to avoid searching the list at all.
 */

typedef struct cellT {
    struct cellT *link;
    /* The actual data is allocated here */
} cellT;

struct iteratorCDT {
    int elementSize;
    cmpFnT cmpFn;
    cellT *head, *tail;
};

/* Part 1 -- Implementation of iterator.h */

iteratorADT NewIterator(void *collection)
{
    iteratorHeaderT *hp = collection;

    if (hp->password != IteratorPassword) {
        Error("Iteration is not defined for this type");
    }
    return (hp->newFn(collection));
}

bool StepIterator(iteratorADT iterator, void *ep)
{
    cellT *cp;
    void *dp;

    cp = iterator->head;
    if (cp == NULL) {
        iterator->tail = NULL;
        return (FALSE);
    }
    dp = ((char *) cp) + sizeof (cellT);
    memcpy(ep, dp, iterator->elementSize);
    iterator->head = cp->link;
    FreeBlock(cp);
    return (TRUE);
}

void FreeIterator(iteratorADT iterator)
{
    cellT *cp;

    while ((cp = iterator->head) != NULL) {
        iterator->head = cp->link;
        FreeBlock(cp);
    }
    FreeBlock(iterator);
}

/* Part 2 -- Implementation of itertype.h */

void EnableIteration(void *collection, newIteratorFnT newFn)
{
    iteratorHeaderT *hp = collection;

    hp->password = IteratorPassword;
    hp->newFn = newFn;
}

iteratorADT NewIteratorList(int size, cmpFnT cmpFn)
{
    iteratorADT iterator;

    iterator = New(iteratorADT);
    iterator->elementSize = size;
    iterator->cmpFn = cmpFn;
    iterator->head = iterator->tail = NULL;
    return (iterator);
}

/*
 * Implementation notes: AddToIteratorList
 * ---------------------------------------
 * Most of the work of the package occurs in this function, which
 * inserts the element addressed by ep into its correct position.
 * If the list is unordered or if the element belongs at the end,
 * the element is immediately inserted at the tail.  If not, the
 * implementation walks the list to find the correct position.
 * The local pointers have the following interpretations:
 *
 * np -- pointer to the newly allocated cell
 * pp -- pointer to the cell preceding the insertion point
 * ip -- pointer used as an index in the for loop
 * dp -- pointer to the data field in the block
 */

void AddToIteratorList(iteratorADT iterator, void *ep)
{
    cellT *np, *pp, *ip;
    void *dp;

    np = GetBlock(sizeof (cellT) + iterator->elementSize);
    dp = ((char *) np) + sizeof (cellT);
    memcpy(dp, ep, iterator->elementSize);
    pp = NULL;
    if (iterator->tail != NULL) {
        if (iterator->cmpFn == UnsortedFn) {
            pp = iterator->tail;
        } else {
            dp = ((char *) iterator->tail) + sizeof (cellT);
            if (iterator->cmpFn(ep, dp) >= 0) pp = iterator->tail;
        }
    }
    if (pp == NULL) {
        for (ip = iterator->head; ip != NULL; ip = ip->link) {
            dp = ((char *) ip) + sizeof (cellT);
            if (iterator->cmpFn(ep, dp) < 0) break;
            pp = ip;
        }
    }
    if (pp == NULL) {
        np->link = iterator->head;
        if (iterator->head == NULL) iterator->tail = np;
        iterator->head = np;
    } else {
        np->link = pp->link;
        if (pp->link == NULL) iterator->tail = np;
        pp->link = np;
    }
}
