/*
 *File: buffer.h
 *
 *This file defines the interface for an editor buffer abstraction.
 *Come from Programming Abstracetions In C P299
 */

#ifndef _listbuffer_h
#define _listbuffer_h

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