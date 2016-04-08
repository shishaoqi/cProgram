/*
 * File: arraybuf.c
 * --------------------
 * This file implement the buffer.h abstraction using a linked
 * list to represent the buffer.
 * Programming Abstracetions In C p325
 */
#include<stdio.h>
#include"genlib.h"
#include"strlib.h"
#include"listbuffer.h"

typedef struct cellT
{
	char ch;
	struct cellT *link;
}cellT;

struct bufferCDT 
{
	cellT *start;
	cellT *cursor;
};

bufferADT NewBuffer(void)
{
	bufferADT buffer;

	buffer = New(bufferADT);
	buffer->start = buffer->cursor = New(cellT*);
	buffer->start->link = NULL;
    
	return buffer;
}

void FreeBuffer(bufferADT buffer)
{
	cellT *cp, *next;

	cp = buffer->start;
	while(cp != NULL)
	{
		next = cp->link;
		FreeBlock(cp);
		cp = next;
	}

	FreeBlock(buffer);
}

void MoveCursorForward(bufferADT buffer)
{
	if(buffer->cursor->link != NULL)
		buffer->cursor = buffer->cursor->link;
}

void MoveCursorBackward(bufferADT buffer)
{
	cellT *cp;
	if(buffer->cursor != buffer->start)
	{
		cp = buffer->start;
		while(cp->link != buffer->cursor)
			cp = cp->link;
	}
	buffer->cursor =cp;
}

void MoveCursorToStart(bufferADT buffer)
{
	buffer->cursor = buffer->start;
}

void MoveCursorToEnd(bufferADT buffer)
{
	while(buffer->cursor->link != NULL)
		MoveCursorForward(buffer);
}

void InsertCharacter(bufferADT buffer, char ch)
{
	cellT *cp;

	cp = New(cellT *);
	cp->ch = ch;
	cp->link=buffer->cursor->link;
	buffer->cursor->link = cp;
	buffer->cursor = cp;
}

void DeleteCharacter(bufferADT buffer)
{
	cellT *cp;
	if(buffer->cursor->link != NULL)
	{
		cp = buffer->cursor->link;
		buffer->cursor->link = cp->link;
		FreeBlock (cp);
	}
}
void DisplayBuffer(bufferADT buffer)
{
	cellT *cp;

	for(cp=buffer->start->link; cp!=NULL; cp= cp->link)
		printf("%c", cp->ch);
	printf("\n");
	for(cp=buffer->start; cp!=buffer->cursor; cp=cp->link)
		printf("  ");
	printf("^\n");
}
