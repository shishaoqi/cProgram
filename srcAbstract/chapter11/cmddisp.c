/*
 * File: cmddisp.c
 * ---------------
 * This program implements a command dispatch mechanism that
 * stores function pointers in a symbol table.
 */

#include <stdio.h>
#include <ctype.h>

#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"

/*
 * Type: commandFnT
 * ----------------
 * This type defines the class of command functions.
 */

typedef void (*commandFnT)(void);

/*
 * Type: commandEntryT
 * -------------------
 * This type consists of a structure containing only a command
 * function.  This extra level of structure is required because
 * function pointers are not compatible with void * in ANSI C.
 */

typedef struct {
    commandFnT fn;
} *commandEntryT;

/*
 * Private variable: commandTable
 * ------------------------------
 * The entries in this table are used to hold the commands and
 * their corresponding actions.
 */

static symtabADT commandTable;

/* Local function declarations */

static void InitCommandTable(void);
static void DefineCommand(string cmd, commandFnT fn);
static void ExecuteCommand(string cmd);
static void ClearCmd(void);
static void RunCmd(void);
static void HelpCmd(void);
static void QuitCmd(void);

/* Main test program */

main()
{
    string line;

    InitCommandTable();
    while (TRUE) {
        printf("-> ");
        line = GetLine();
        ExecuteCommand(line);
        if (StringEqual(line, "quit")) break;
    }
}

static void InitCommandTable(void)
{
    commandTable = NewSymbolTable();
    DefineCommand("clear", ClearCmd);
    DefineCommand("run", RunCmd);
    DefineCommand("help", HelpCmd);
    DefineCommand("quit", QuitCmd);
}

static void DefineCommand(string cmd, commandFnT fn)
{
    commandEntryT entry;

    entry = New(commandEntryT);
    entry->fn = fn;
    Enter(commandTable, cmd, entry);
}

static void ExecuteCommand(string cmd)
{
    commandEntryT entry;

    entry = Lookup(commandTable, cmd);
    if (entry == UNDEFINED) {
        printf("Undefined command: %s\n", cmd);
        return;
    }
    entry->fn();
}

/* Command dispatch functions */

static void ClearCmd(void)
{
    printf("Command = clear\n");
}

static void RunCmd(void)
{
    printf("Command = run\n");
}

static void HelpCmd(void)
{
    printf("Command = help\n");
}

static void QuitCmd(void)
{
    printf("Command = quit\n");
}
