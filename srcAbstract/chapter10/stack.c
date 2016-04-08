/*
 * File: stack.c
 * -------------
 * This file implements the stack.h interface using linked lists.
 */

#include <stdio.h>
#include "genlib.h"
#include "stack.h"

/*
 * Type: cellT
 * -----------
 * This type defines the linked list cell used for the stack.
 */

typedef struct cellT {
    stackElementT element;
    struct cellT *link;
} cellT;

/*
 * Type: stackCDT
 * --------------
 * This type defines the concrete structure of a stack.
 */

struct stackCDT {
    cellT *start;
};

/* Exported entries */

stackADT NewStack(void)
{
    stackADT stack;

    stack = New(stackADT);
    stack->start = NULL;
    return (stack);
}

void FreeStack(stackADT stack)
{
    cellT *cp, *next;

    cp = stack->start;
    while (cp != NULL) {
        next = cp->link;
        FreeBlock(cp);
        cp = next;
    }
    FreeBlock(stack);
}

void Push(stackADT stack, stackElementT element)
{
    cellT *cp;

    cp = New(cellT *);
    cp->element = element;
    cp->link = stack->start;
    stack->start = cp;
}

stackElementT Pop(stackADT stack)
{
    stackElementT result;
    cellT *cp;

    if (StackIsEmpty(stack)) Error("Pop of an empty stack");
    cp = stack->start;
    result = cp->element;
    stack->start = cp->link;
    FreeBlock(cp);
    return (result);
}

bool StackIsEmpty(stackADT stack)
{
    return (stack->start == NULL);
}

bool StackIsFull(stackADT stack)
{
    return (FALSE);
}

int StackDepth(stackADT stack)
{
    int n;
    cellT *cp;

    n = 0;
    for (cp = stack->start; cp != NULL; cp = cp->link) n++;
    return (n);
}

stackElementT GetStackElement(stackADT stack, int depth)
{
    int i;
    cellT *cp;

    if (depth < 0 || depth >= StackDepth(stack)) {
        Error("Non-existent stack element");
    }
    cp = stack->start;
    for (i = 0; i < depth; i++) cp = cp->link;
    return (cp->element);
}
