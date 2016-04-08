/*
 * File: doublelistbuf.c
 * ----------------------
 * This file implement the buffer.h abstraction using double linked
 * list to represent the buffer.
 * ProgrammingAbstracetionsInC p329
 */

#include<stdio.h>
#include"genlib.h"
#include"strlib.h"
#include"doublelistbuffer.h"

typedef struct cellT
{
	char ch;
	struct cellT *next;
	struct cellT *prev;
}cellT;

struct bufferCDT
{
	cellT *start;
	cellT *end;
	cellT *cursor;
};

bufferADT NewBuffer(void)
{
	bufferADT buffer;

	buffer = New(bufferADT);
	buffer->start=buffer->cursor=New(cellT*);
	buffer->end=New(cellT*);
	buffer->start->next=buffer->end->prev=NULL;
	buffer->start->prev=buffer->end->next=NULL;

	return buffer;
}

void FreeBuffer(bufferADT buffer)
{
	cellT *cp, *temp_next;

	cp = buffer->start;
	while(cp!=NULL)
	{
		temp_next=cp->next;
		FreeBlock(cp);
		cp=temp_next;
	}
	FreeBlock(buffer);
}

void MoveCursorForward(bufferADT buffer)
{
	if(buffer->cursor->next != NULL)
		buffer->cursor=buffer->cursor->next;
}

void MoveCursorBackward(bufferADT buffer)
{
	if(buffer->cursor->prev !=NULL)
		buffer->cursor=buffer->cursor->prev;
}

void MoveCursorToStart(bufferADT buffer)
{
	buffer->cursor=buffer->start;
}

void MoveCursorToEnd(bufferADT buffer)
{
	buffer->cursor=buffer->end;
}

void InsertCharacter(bufferADT buffer, char ch)
{
	cellT *cp;

	cp = New(cellT*);
	cp->next=buffer->cursor->next;
	buffer->cursor->next=cp;
    cp->prev=buffer->cursor->prev;
	buffer->cursor->prev=cp;
	if(cp->next==NULL)
		buffer->end->prev=cp;
}

void DeletCharacter(bufferADT buffer)
{
	cellT *cp;

	if(buffer->cursor->next!=NULL)
	{
		cp=buffer->cursor->next;
		buffer->cursor->next=cp->next;
		buffer->cursor->prev=cp->prev;
		FreeBlock(cp);
	}
}

void DisplayBuffer(bufferADT buffer)
{
	cellT *cp;

	for(cp=buffer->start->next; cp!=NULL; cp= cp->next)
		printf(" %c", cp->ch);
	printf("\n");
	for(cp=buffer->start; cp!=buffer->cursor; cp=cp->next)
		printf("  ");
	printf("^\n");
}