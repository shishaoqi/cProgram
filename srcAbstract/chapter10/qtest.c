/*
 * File: qtest.c
 * -------------
 * This program tests the queue package by allowing the user to
 * keep track of a waiting list.  The commands are listed in the
 * function GiveInstructions.
 */

#include <stdio.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "scanadt.h"
#include "queue.h"

/* Private function prototypes */

static void GiveInstructions(void);
static void DisplayQueue(queueADT queue);

/* Main program */

main()
{
    queueADT queue;
    scannerADT scanner;
    string line, cmd, value;

    GiveInstructions();
    scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);
    queue = NewQueue();
    while (TRUE) {
        printf(":");
        line = GetLine();
        SetScannerString(scanner, line);
        cmd = ConvertToLowerCase(ReadToken(scanner));
        if (StringEqual(cmd, "enqueue")) {
            value = ReadToken(scanner);
            Enqueue(queue, value);
        } else if (StringEqual(cmd, "dequeue")) {
            printf("%s\n", Dequeue(queue));
        } else if (StringEqual(cmd, "display")) {
            DisplayQueue(queue);
        } else if (StringEqual(cmd, "quit")) {
            break;
        } else {
            printf("Unrecognized command: %s\n", line);
        }
    }
    FreeQueue(queue);
    FreeScanner(scanner);
}

/*
 * Function: GiveInstructions
 * Usage: GiveInstructions();
 * --------------------------
 * This function displays a message to the user showing
 * what options are available in the test program.
 */

static void GiveInstructions(void)
{
    printf("Enter commands in the following forms:\n");
    printf("  enqueue x   -- Enqueue the token x\n");
    printf("  dequeue     -- Dequeue the value at the head of the queue\n");
    printf("  display     -- Display the contents of the queue\n");
    printf("  quit        -- Exit from the program\n");
}

/*
 * Function: DisplayQueue
 * Usage: DisplayQueue(queue);
 * ---------------------------
 * This function displays the contents of a string queue on
 * a single line.
 */

static void DisplayQueue(queueADT queue)
{
    int i, len;

    len = QueueLength(queue);
    if (len == 0) {
        printf("Queue is empty.");
    } else {
        printf("Queue contains: ");
        for (i = 0; i < len; i++) {
            if (i > 0) printf(", ");
            printf("%s", GetQueueElement(queue, i));
        }
    }
    printf("\n");
}
