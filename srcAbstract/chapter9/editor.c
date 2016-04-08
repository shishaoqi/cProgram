/*
 * File: editor.c
 * --------------
 * This program implements a simple character editor, which
 * is used to test the buffer abstraction.  The editor reads
 * and executes simple commands entered by the user.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "buffer.h"
#include "simpio.h"

/* Private function prototypes */

static void ExecuteCommand(bufferADT buffer, string line);
static void HelpCommand(void);

/* Main program */

main()
{
    bufferADT buffer;

    buffer = NewBuffer();
    while (TRUE) {
        printf("*");
        ExecuteCommand(buffer, GetLine());
        DisplayBuffer(buffer);
    }
    FreeBuffer(buffer);
}

/*
 * Function: ExecuteCommand
 * Usage: ExecuteCommand(buffer, line);
 * ------------------------------------
 * This function parses the user command in the string line
 * and executes it on the buffer.
 */

static void ExecuteCommand(bufferADT buffer, string line)
{
    int i;

    switch (toupper(line[0])) {
      case 'I': for (i = 1; line[i] != '\0'; i++) {
                    InsertCharacter(buffer, line[i]);
                }
                break;
      case 'D': DeleteCharacter(buffer); break;
      case 'F': MoveCursorForward(buffer); break;
      case 'B': MoveCursorBackward(buffer); break;
      case 'J': MoveCursorToStart(buffer); break;
      case 'E': MoveCursorToEnd(buffer); break;
      case 'H': HelpCommand(); break;
      case 'Q': exit(0);
      default:  printf("Illegal command\n"); break;
    }
}

/*
 * Function: HelpCommand
 * Usage: HelpCommand();
 * ---------------------
 * This function lists the available editor commands.
 */

static void HelpCommand(void)
{
    printf("Use the following commands to edit the buffer:\n");
    printf("  I...   Inserts text up to the end of the line\n");
    printf("  F      Moves forward a character\n");
    printf("  B      Moves backward a character\n");
    printf("  J      Jumps to the beginning of the buffer\n");
    printf("  E      Jumps to the end of the buffer\n");
    printf("  D      Deletes the next character\n");
    printf("  H      Generates a help message\n");
    printf("  Q      Quits the program\n");
}
