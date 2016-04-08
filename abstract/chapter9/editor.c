/*
 *File: editor.c
 *--------------
 *This program implements a simple character editor, which
 *is used to test the buffer abstraction. The editor reads
 *and executes simple commands entered by the user.
 */

#include<stdio.h>
#include<ctype.h>
#include"genlib.h"
#include"arraybuffer.h"
#include"simpio.h"

static void ExecuteCommand(bufferADT buffer, string line);
static void HelpCommand(void);

int main()
{
	bufferADT buffer;

	buffer = NewBuffer();
	while(TRUE)
	{
		printf("*");
		char *s;
		ExecuteCommand(buffer, s=GetLine());
		DisplayBuffer(buffer);
	}
	FreeBuffer(buffer);
	return 0;
}

static void ExecuteCommand(bufferADT buffer, string line)
{
	int i;
	if(toupper(line[0])=='I')
		for(i=1; line[i] != '\0'; i++)
			InsertCharacter(buffer, line[i]);
		else
			switch(toupper(line[0]))
		{
			case 'H': HelpCommand(); break;
			case 'D': DeleteCharacter(buffer); break;
			case 'F': MoveCursorForward(buffer); break;
			case 'B': MoveCursorBackward(buffer); break;
			case 'J': MoveCursorToStart(buffer); break;
			case 'E': MoveCursorToEnd(buffer); break;
			case 'Q': exit(0);
			default: printf(" Illegal command\n"); break;
		}
}

static void HelpCommand(void)
{
	printf(" use the following commands to edit the buffer: \n");
	printf(" I ... Inserts text up to the end of the line\n");
	printf(" F     Moves forward a character\n");
	printf(" B     Moves backward a character\n");
	printf(" J     Jumps to the beginning of the buffer\n");
	printf(" E     Jumps to the end of the Buffer\n");
	printf(" D     Diletes the nex character\n");
	printf(" H     Generates a help message\n");
	printf(" Q     Quits the program\n");
}


























