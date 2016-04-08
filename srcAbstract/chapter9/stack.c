/*
 * File: stack.c
 * -------------
 * This file implements the stack.h interface.  Note that the
 * implementation is independent of the type of value stored
 * in the stack.  That type is defined by the interface as
 * the type name stackElementT.
 */

#include <stdio.h>
#include "genlib.h"
#include "stack.h"

/*
 * Constant: InitialStackSize
 * --------------------------
 * This constant defines the initial size of the stack array.
 * Any positive value will work correctly, although changing
 * this parameter can affect performance.  Making this value
 * larger postpones the first reallocation but causes stacks
 * to consume more memory.
 */

#define InitialStackSize 100

/*
 * Type: stackCDT
 * --------------
 * The type stackCDT is the concrete representation of the type
 * stackADT defined by the interface.  In this implementation,
 * the elements are stored in a dynamic array that doubles in
 * size if the old stack becomes full.
 */

struct stackCDT {
    stackElementT *elements;
    int count;
    int size;
};

/* Private function prototypes */

static void ExpandStack(stackADT stack);

/* Exported entries */

stackADT NewStack(void)
{
    stackADT stack;

    stack = New(stackADT);
    stack->elements = NewArray(InitialStackSize, stackElementT);
    stack->count = 0;
    stack->size = InitialStackSize;
    return (stack);
}

void FreeStack(stackADT stack)
{
    FreeBlock(stack->elements);
    FreeBlock(stack);
}

void Push(stackADT stack, stackElementT element)
{
    if (stack->count == stack->size) ExpandStack(stack);
    stack->elements[stack->count++] = element;
}

stackElementT Pop(stackADT stack)
{
    if (StackIsEmpty(stack)) Error("Pop of an empty stack");
    return (stack->elements[--stack->count]);
}

bool StackIsEmpty(stackADT stack)
{
    return (stack->count == 0);
}

bool StackIsFull(stackADT stack)
{
    return (FALSE);
}

int StackDepth(stackADT stack)
{
    return (stack->count);
}

stackElementT GetStackElement(stackADT stack, int index)
{
    if (index < 0 || index >= stack->count) {
        Error("Non-existent stack element");
    }
    return (stack->elements[stack->count - index - 1]);
}

/* Private functions */

/* Function: ExpandStack
 * Usage: ExpandStack(stack);
 * --------------------------
 * This function expands a full stack by doubling the size of its
 * dynamic array, copying the old elements to the new array, and
 * then freeing the old array storage.
 */

static void ExpandStack(stackADT stack)
{
    stackElementT *array;
    int i, newSize;

    newSize = stack->size * 2;
    array = NewArray(newSize, stackElementT);
    for (i = 0; i < stack->size; i++) {
        array[i] = stack->elements[i];
    }
    FreeBlock(stack->elements);
    stack->elements = array;
    stack->size = newSize;
}
