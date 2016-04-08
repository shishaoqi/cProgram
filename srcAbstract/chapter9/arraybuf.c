/*
 * File: arraybuf.c
 * ----------------
 * This file implements the buffer.h abstraction using an
 * array to represent the buffer.
 */

#include <stdio.h>
#include "genlib.h"
#include "buffer.h"

/*
 * Type: bufferCDT
 * ---------------
 * In this representation of the buffer, the characters are stored
 * in an array embedded within a structure that also holds the
 * length of the buffer and the cursor position as integers.
 * The cursor indicates the index position of the character that
 * follows where the cursor would appear on the screen.
 */

#define MaxBuffer 100

struct bufferCDT {
    char text[MaxBuffer];
    int length;
    int cursor;
};

/* Exported entries */

bufferADT NewBuffer(void)
{
    bufferADT buffer;

    buffer = New(bufferADT);
    buffer->length = buffer->cursor = 0;
    return (buffer);
}

void FreeBuffer(bufferADT buffer)
{
    FreeBlock(buffer);
}

void MoveCursorForward(bufferADT buffer)
{
    if (buffer->cursor < buffer->length) buffer->cursor++;
}

void MoveCursorBackward(bufferADT buffer)
{
    if (buffer->cursor > 0) buffer->cursor--;
}

void MoveCursorToStart(bufferADT buffer)
{
    buffer->cursor = 0;
}

void MoveCursorToEnd(bufferADT buffer)
{
    buffer->cursor = buffer->length;
}

/*
 * Implementation notes: InsertCharacter and DeleteCharacter
 * ---------------------------------------------------------
 * Each of the functions that inserts or deletes characters must
 * shift all subsequent characters in the array, either to make
 * room for new insertions or to close up space left by deletions.
 */

void InsertCharacter(bufferADT buffer, char ch)
{
    int i;

    if (buffer->length == MaxBuffer) Error("Buffer size exceeded");
    for (i = buffer->length; i > buffer->cursor; i--) {
        buffer->text[i] = buffer->text[i - 1];
    }
    buffer->text[buffer->cursor] = ch;
    buffer->length++;
    buffer->cursor++;
}

void DeleteCharacter(bufferADT buffer)
{
    int i;

    if (buffer->cursor < buffer->length) {
        for (i = buffer->cursor+1; i < buffer->length; i++) {
            buffer->text[i - 1] = buffer->text[i];
        }
        buffer->length--;
    }
}

void DisplayBuffer(bufferADT buffer)
{
    int i;

    for (i = 0; i < buffer->length; i++) {
        printf(" %c", buffer->text[i]);
    }
    printf("\n");
    for (i = 0; i < buffer->cursor; i++) {
        printf("  ");
    }
    printf("^\n");
}
