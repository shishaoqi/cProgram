/*
 * File: stack.h
 * -------------
 * This interface defines an abstraction for stacks.  In any
 * single application that uses this interface, the values in
 * the stack are constrained to a single type, although it
 * is easy to change that type by changing the definition of
 * stackElementT in this interface.
 */

#ifndef _stack_h
#define _stack_h

#include "genlib.h"

/*
 * Type: stackElementT
 * -------------------
 * The type stackElementT is used in this interface to indicate
 * the type of values that can be stored in the stack.  Here the
 * stack is used to store values of type char, but that can
 * be changed by editing this definition line.
 */

typedef char stackElementT;

/*
 * Type: stackADT
 * --------------
 * The type stackADT represents the abstract type used to store
 * the elements that have been pushed.  Because stackADT is
 * defined only as a pointer to a concrete structure that is not
 * itself defined in the interface, clients have no access to
 * the underlying fields.
 */

typedef struct stackCDT *stackADT;

/*
 * Function: NewStack
 * Usage: stack = NewStack();
 * --------------------------
 * This function allocates and returns a new stack, which is
 * initially empty.
 */

stackADT NewStack(void);

/*
 * Function: FreeStack
 * Usage: FreeStack(stack);
 * ------------------------
 * This function frees the storage associated with the stack.
 */

void FreeStack(stackADT stack);

/*
 * Function: Push
 * Usage: Push(stack, element);
 * ----------------------------
 * This function pushes the specified element onto the stack.
 */

void Push(stackADT stack, stackElementT element);

/*
 * Function: Pop
 * Usage: element = Pop(stack);
 * ----------------------------
 * This function pops the top element from the stack and returns
 * that value.  The first value popped is always the last one
 * that was pushed.  If the stack is empty when Pop is called,
 * the function calls Error with an appropriate message.
 */

stackElementT Pop(stackADT stack);

/*
 * Functions: StackIsEmpty, StackIsFull
 * Usage: if (StackIsEmpty(stack)) . . .
 *        if (StackIsFull(stack)) . . .
 * -------------------------------------
 * This functions test whether the stack is empty or full.
 */

bool StackIsEmpty(stackADT stack);
bool StackIsFull(stackADT stack);

/*
 * Function: StackDepth
 * Usage: depth = StackDepth(stack);
 * ---------------------------------
 * This function returns the number of elements currently pushed
 * on the stack.
 */

int StackDepth(stackADT stack);

/*
 * Function: GetStackElement
 * Usage: element = GetStackElement(stack, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in
 * the stack, where the top of the stack is defined as index 0.
 * For example, calling GetStackElement(stack, 0) returns the top
 * element on the stack without removing it.  If the caller tries
 * to select an out-of-range element, GetStackElement calls Error.
 * Note: This function is not a fundamental stack operation and
 * is instead provided principally to facilitate debugging.
 */

stackElementT GetStackElement(stackADT stack, int index);

#endif
