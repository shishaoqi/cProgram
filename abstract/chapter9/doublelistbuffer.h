/*
 * File: doublelistbuffer.h
 * -------------------------
 * This file defines the interface for an editor buffer abstraction.
 * ProgrammingAbstracetionsInC use double list implements the editor
 */

#ifndef _doublelistbuffer_h
#define _doublelistbuffer_h

#include "genlib.h"

typedef struct bufferCDT *bufferADT;

bufferADT NewBuffer(void);
void FreeBuffer(bufferADT buffer);
void MoveCursorForward(bufferADT buffer);
void MoveCursorBackward(bufferADT buffer);
void MoveCursorToStart(bufferADT buffer);
void MoveCursorToEnd(bufferADT buffer);
void InsertCharacter(bufferADT buffer, char ch);
void DeleteCharacter(bufferADT buffer);
void DisplayBuffer(bufferADT buffer);

#endif 