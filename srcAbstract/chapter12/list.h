/*
 * File: list.h
 * ------------
 * This interface defines an abstraction for an immutable list.
 */

#ifndef _list_h
#define _list_h

#include "genlib.h"

/*
 * Type: listElementT
 * ------------------
 * The type listElementT is used in this interface to indicate
 * the type of values that can be stored in the list.  Here the
 * list is used to store values of type void *, but that can
 * be changed by editing this definition line.
 */

typedef void *listElementT;

/*
 * Type: listADT
 * -------------
 * The type listADT represents the abstract type used to represent
 * the list itself.  The empty list is always represented by NULL.
 * The client can rely on the following aspects of the internal
 * structure:
 *
 * 1. Each list value returned is dynamically allocated.
 *
 * 2. The list element is stored at the beginning of the
 *    structure used internally to hold the record so that
 *    the address of the list cell is also the address of the
 *    stored value.  This constraint makes it possible to apply
 *    comparison functions to the stored data.
 */

typedef struct listCDT *listADT;

/*
 * Function: ListCons
 * Usage: list = ListCons(head, tail);
 * -----------------------------------
 * This function allocates and returns a new list, which consists
 * of the element head followed by the list tail.
 */

listADT ListCons(listElementT head, listADT tail);

/*
 * Functions: ListHead, ListTail
 * Usage: head = ListHead(list);
 *        tail = ListTail(list);
 * -----------------------------
 * These functions return the head and tail of a list, where the
 * head is defined to be the first element and the tail is the
 * list that remains after removing the head of the list.
 */

listElementT ListHead(listADT list);
listADT ListTail(listADT list);

#endif
