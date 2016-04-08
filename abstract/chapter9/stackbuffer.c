/*
 * File: stackbuf.c
 * -------------------
 * This file implement the buffer.h abstraction using a pair of
 * stacks to represent the buffer.
 * Programming Abstracetions In C p311
 */
#include<stdio.h>
#include"genlib.h"
#include"stackbuffer.h"
#include"stack.h"

struct bufferCDT
{
	stackADT before;
	stackADT after;
};

bufferADT NewBuffer(void)
{
	bufferADT buffer;

	buffer = New(bufferADT);
	buffer->before = NewStack();
	buffer->after = NewStack();
	return buffer;
}

void FreeBuffer(bufferADT buffer)
{
	FreeStack (buffer->before);
	FreeStack (buffer->after);
	FreeBlock (buffer);
}

void MoveCursorForward(bufferADT buffer)
{
	if(!StackIsEmpty(buffer->after))
		Push(buffer->before, Pop(buffer->after));
}

void MoveCursorBackward(bufferADT buffer)
{
	if(!StackIsEmpty(buffer->before))
		Push(buffer->after, Pop(buffer->before));
}

void MoveCursorToStart(bufferADT buffer)
{
	while(!StackIsEmpty(buffer->before))
		Push(buffer->after, Pop(buffer->before));
}

void MoveCursorToEnd(bufferADT buffer)
{
	while(!StackIsEmpty(buffer->after))
		Push(buffer->before, Pop(buffer->after));
}

void InsertCharacter(bufferADT buffer, char ch)
{
	Push(buffer->before, ch);
}

void DeleteCharacter(bufferADT buffer)
{
	if(!StackIsEmpty(buffer->after))
		Pop(buffer->after);
}

void DisplayBuffer(bufferADT buffer)
{
	int i;

	for(i=StackDepth(buffer->before)-1; i>=0; i--)
		printf(" %c", GetStackElement(buffer->before, i));
	for(i=0; i<StackDepth(buffer->after); i++)
		printf(" %c", GetStackElement(buffer->after, i));
	printf("\n");
	for(i=0; i<StackDepth(buffer->before); i++)
		printf("  ");
	printf("^\n");
}
