/*
 * File: buffer.h
 * --------------
 * This file defines the interface for an editor buffer abstraction.
 */

#ifndef _buffer_h
#define _buffer_h

#include "genlib.h"

/*
 * Type: bufferADT
 * ---------------
 * This type defines the abstract type used to represent
 * an editor buffer.
 */

typedef struct bufferCDT *bufferADT;

/* Exported entries */

/*
 * Function: NewBuffer
 * Usage: buffer = NewBuffer();
 * ----------------------------
 * This function dynamically allocates enough memory for the
 * underlying representation of a bufferADT and initializes
 * it to represent an empty buffer.
 */

bufferADT NewBuffer(void);

/*
 * Function: FreeBuffer
 * Usage: FreeBuffer(buffer);
 * --------------------------
 * This function frees the storage associated with the buffer.
 */

void FreeBuffer(bufferADT buffer);

/*
 * Functions: MoveCursorForward, MoveCursorBackward
 * Usage: MoveCursorForward(buffer);
 *        MoveCursorBackward(buffer);
 * ----------------------------------
 * These functions move the cursor forward or backward one
 * character, respectively.  If you call MoveCursorForward
 * at the end of the buffer or MoveCursorBackward at the
 * beginning, the function call has no effect.
 */

void MoveCursorForward(bufferADT buffer);
void MoveCursorBackward(bufferADT buffer);

/*
 * Functions: MoveCursorToStart, MoveCursorToEnd
 * Usage: MoveCursorToStart(buffer);
 *        MoveCursorToEnd(buffer);
 * -------------------------------
 * These functions move the cursor to the start or the
 * end of the buffer, respectively.
 */

void MoveCursorToStart(bufferADT buffer);
void MoveCursorToEnd(bufferADT buffer);

/*
 * Function: InsertCharacter
 * Usage: InsertCharacter(buffer, ch);
 * -----------------------------------
 * This function inserts the single character ch into the
 * buffer at the current cursor position.  The cursor is
 * positioned after the inserted character, which allows
 * for consecutive insertions.
 */

void InsertCharacter(bufferADT buffer, char ch);

/*
 * Function: DeleteCharacter
 * Usage: DeleteCharacter(buffer);
 * -------------------------------
 * This function deletes the character immediately after
 * the cursor.  If the cursor is at the end of the buffer,
 * this function has no effect.
 */

void DeleteCharacter(bufferADT buffer);

/*
 * Function: DisplayBuffer
 * Usage: DisplayBuffer(buffer);
 * -----------------------------
 * This function displays the current contents of the buffer
 * on the console.
 */

void DisplayBuffer(bufferADT buffer);

#endif
