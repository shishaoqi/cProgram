/*
 * File: arraybuf.c
 * -----------------
 * This file implement the buffer.h abstraction using an
 * array to represent the buffer.
 * Come from Programming Abstracetions In C p305
 */
#include<stdio.h>
#include<ctype.h>
#include"genlib.h"
#include"strlib.h"
#include"arraybuffer.h"

#define InitializeArraySize 10
struct bufferCDT{
	char *text;
	int length;
	int cursor;
	int size;
};

bufferADT NewBuffer(void)
{
	bufferADT buffer;

	buffer = New(bufferADT);
	buffer->text = NewArray(InitializeArraySize, char);
	buffer->length = buffer->cursor = 0;
	buffer->size = InitializeArraySize;
	return buffer;
}

void FreeBuffer(bufferADT buffer)
{
	FreeBlock(buffer);
}

/* character operate */
void MoveCursorForward(bufferADT buffer)
{
	if(buffer->cursor < buffer->length)
		buffer->cursor++;
}

void MoveCursorBackward(bufferADT buffer)
{
	if(buffer->cursor>0) buffer->cursor--;
}

void ExpandArray(bufferADT buffer)
{
	char *newText;
	int i, newSize;

	newSize = buffer->size*2;
	newText = NewArray(newSize, char);
	for(i=0; i<buffer->length; i++)
		newText[i] = buffer->text[i];
	FreeBlock(buffer->text);
	buffer->text = newText;
	buffer->size = newSize;
}

void InsertCharacter(bufferADT buffer, char ch)
{
	int i;
	char temp;

	if(buffer->length == buffer->size) 
		ExpandArray(buffer);
	for(i=buffer->length; i>buffer->cursor; i--)
		buffer->text[i] = buffer->text[i-1];
	
	buffer->text[buffer->cursor] = ch;
	buffer->length++;
	buffer->cursor++;
}

void DeleteCharacter(bufferADT buffer)
{
	int i;

	if(buffer->cursor < buffer->length)
	{
		for(i=buffer->cursor+1; i < buffer->length; i++)
			buffer->text[i-1]=buffer->text[i];
	    buffer->length--;
		//buffer->cursor--;
	}
}

 /* Words or sentences operation */
void MoveCursorFWord(bufferADT buffer)
{
    int i=buffer->cursor;
	while(TRUE)
	{
		i++;
	    if(isalnum(buffer->text[i]))
		{
			while(isalnum(buffer->text[i]))
				i++;
			break;
		}
	}
	buffer->cursor=i;
}

void MoveCursorBWord(bufferADT buffer)
{
	int i=buffer->cursor;
	while(TRUE)
		{
			i--;
			if(isalnum(buffer->text[i]))
			{
				while(isalnum(buffer->text[i]))
					i--;
				break;
			}
		}
	buffer->cursor=i;
}

void InsertWord(bufferADT buffer, string str)
{
	int i,length=StringLength(str);
	char ch;

	buffer->cursor+=1;
    for(i=0;i<length;i++)
	{
		ch=*(str+i);
		InsertCharacter(buffer,ch);
	}
    InsertCharacter(buffer,' ');


/*	int i,length;
	int last=buffer->length;

	length=StringLength(str)+1;
	if(buffer->length+length>=buffer->size)
		ExpandArray(buffer);
	for(i=0;i<(buffer->length-buffer->cursor);i++)
       buffer->text[last+length-i]=buffer->text[last-i];
	buffer->length=buffer->length+length;
    
	buffer->cursor+=1;
	for(i=0;i<length-1;i++)
		buffer->text[buffer->cursor++]=*(str+i);
	buffer->text[buffer->cursor]=' ';*/
}

void DeleteWord(bufferADT buffer)
{
	while(TRUE)
	{
		DeleteCharacter(buffer);
		if(isalnum(buffer->text[buffer->cursor]))
		{
			while(isalnum(buffer->text[buffer->cursor]))
				DeleteCharacter(buffer);
			break;
		}
	}
}

bool SearchBuffer(bufferADT buffer);
void ReplaceBuffer(bufferADT buffer);




/* Move cursor to start or end */
void MoveCursorToStart(bufferADT buffer)
{
	buffer->cursor = 0;
}

void MoveCursorToEnd(bufferADT buffer)
{
	buffer->cursor = buffer->length;
}

void DisplayBuffer(bufferADT buffer)
{
	int i;

	for(i=0; i<buffer->length; i++)
		printf(" %c", buffer->text[i]);
	printf("\n");
	for(i=0; i<buffer->cursor; i++)
		printf("  ");
	printf("^\n");
}
