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
#include"strlib.h"

static void ExecuteCommand(bufferADT buffer, string line);
static void HelpCommand(void);
int count=0;

int main()
{
	bufferADT buffer;

	buffer = NewBuffer();
	while(TRUE)
	{
		printf("*");
		char *s;
		char *ch;
		int i=0;
		s=GetLine();
		if(isdigit(*s) || *s=='-'){
            i=StringLength(s);
			ch=(char*)GetBlock(i);
			sscanf(s, "%d %s", &count, ch);
			printf("%d\n", count);
			ExecuteCommand(buffer, ch);
			FreeBlock(ch);
		}
		else
			ExecuteCommand(buffer, s);
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
	else if(count>=0)
			switch(toupper(line[0]))
		{
			case 'H': HelpCommand(); break;
			case 'D': if(count==0)
						  DeleteCharacter(buffer);
				      else
						  for(i=0;i<count;i++)
							  DeleteCharacter(buffer); break;
			case 'F': if(count==0)
						  MoveCursorForward(buffer);
				      else
						  for(i=0;i<count;i++)
							  MoveCursorForward(buffer); break;
			case 'B': if(count==0)
						  MoveCursorBackward(buffer);
				      else
                          for(i=0;i<count;i++)
							  MoveCursorBackward(buffer); break;
			case 'J': MoveCursorToStart(buffer); break;
			case 'E': MoveCursorToEnd(buffer); break;
			case 'Q': exit(0);
			default: printf(" Illegal command\n"); break;
		}
	else
		{
			switch(toupper(line[0]))
			{
			case 'D': for(i=0; i<-count; i++)
						  MoveCursorBackward(buffer);
				      for(i=0; i<-count; i++)
						  DeleteCharacter(buffer); break;
            case 'F': for(i=0;i<-count;i++)
					      MoveCursorBackward(buffer); break;
            case 'B': for(i=0;i<-count;i++)
					      MoveCursorForward(buffer); break;
			}
		}
	count=0;
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


























