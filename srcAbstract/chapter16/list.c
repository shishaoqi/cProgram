/*
 * File: list.c
 * ------------
 * This file implements the list.h interface, which defines
 * an immutable abstraction for manipulating lists of values.
 */

#include <stdio.h>
#include "genlib.h"
#include "list.h"

/*
 * Type: listCDT
 * -------------
 * The type listCDT is the concrete representation of the type
 * listADT defined by the interface.  Because the list exported
 * by this interface is immutable, there is no need for an extra
 * level of structure.  Thus, the concrete list type is just a
 * linked-list cell containing an element and another list.
 */

struct listCDT {
    listElementT head;
    listADT tail;
};

/* Exported entries */

listADT ListCons(listElementT head, listADT tail)
{
    listADT list;

    list = New(listADT);
    list->head = head;
    list->tail = tail;
    return (list);
}

listElementT ListHead(listADT list)
{
    if (list == NULL) Error("ListHead called on NULL list");
    return (list->head);
}

listADT ListTail(listADT list)
{
    if (list == NULL) Error("ListTail called on NULL list");
    return (list->tail);
}
