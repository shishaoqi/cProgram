/*
 * File: stackbuf.c
 * ----------------
 * This file implements the buffer.h abstraction using a pair of
 * stacks to represent the buffer.
 */

#include <stdio.h>
#include "genlib.h"
#include "buffer.h"
#include "stack.h"

/*
 * Type: bufferCDT
 * ---------------
 * In this representation of the buffer, all characters are stored
 * in one of two stacks.  Characters before the cursor are pushed
 * on the before stack; those after the cursor are pushed on the
 * after stack.  The cursor is represented implicitly by the
 * boundary between the stacks.  Characters are at deeper levels on
 * their respective stack if they are farther from the cursor.  As
 * an example, the buffer containing ABCDE with the cursor between
 * the C and the D would look like this:
 *
 *         C
 *         B         D
 *         A         E
 *      ------     -----
 *      before     after
 */

struct bufferCDT {
    stackADT before;
    stackADT after;
};

/* Exported entries */

bufferADT NewBuffer(void)
{
    bufferADT buffer;

    buffer = New(bufferADT);
    buffer->before = NewStack();
    buffer->after = NewStack();
    return (buffer);
}

void FreeBuffer(bufferADT buffer)
{
    FreeStack(buffer->before);
    FreeStack(buffer->after);
    FreeBlock(buffer);
}

void MoveCursorForward(bufferADT buffer)
{
    if (!StackIsEmpty(buffer->after)) {
        Push(buffer->before, Pop(buffer->after));
    }
}

void MoveCursorBackward(bufferADT buffer)
{
    if (!StackIsEmpty(buffer->before)) {
        Push(buffer->after, Pop(buffer->before));
    }
}

void MoveCursorToStart(bufferADT buffer)
{
    while (!StackIsEmpty(buffer->before)) {
        Push(buffer->after, Pop(buffer->before));
    }
}

void MoveCursorToEnd(bufferADT buffer)
{
    while (!StackIsEmpty(buffer->after)) {
        Push(buffer->before, Pop(buffer->after));
    }
}

void InsertCharacter(bufferADT buffer, char ch)
{
    Push(buffer->before, ch);
}

void DeleteCharacter(bufferADT buffer)
{
    if (!StackIsEmpty(buffer->after)) {
        (void) Pop(buffer->after);
    }
}

void DisplayBuffer(bufferADT buffer)
{
    int i;

    for (i = StackDepth(buffer->before) - 1; i >= 0; i--) {
        printf(" %c", GetStackElement(buffer->before, i));
    }
    for (i = 0; i < StackDepth(buffer->after); i++) {
        printf(" %c", GetStackElement(buffer->after, i));
    }
    printf("\n");
    for (i = 0; i < StackDepth(buffer->before); i++) {
        printf("  ");
    }
    printf("^\n");
}

