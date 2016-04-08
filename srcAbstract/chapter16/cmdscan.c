/*
 * File: cmdscan.c
 * ---------------
 * This file implements the cmdscan.h interface.
 */

#include <stdio.h>
#include "genlib.h"
#include "exception.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"
#include "scanadt.h"
#include "cmdscan.h"

/*
 * Type: cmdScannerCDT
 * -------------------
 * This type defines the concrete structure of the command scanner.
 */

struct cmdScannerCDT {
    string line;
    string cmdName;
    symtabADT cmdTable;
    scannerADT scanner;
    void *data;
    bool quit;
};

/*
 * Type: cmdEntryT
 * ---------------
 * This type defines the entry for an individual command.
 */

typedef struct {
    string cmdName;
    cmdFnT cmdFn;
} *cmdEntryT;

/* Exported entries */

cmdScannerADT NewCommandScanner(void)
{
    cmdScannerADT cs;

    cs = New(cmdScannerADT);
    cs->cmdTable = NewSymbolTable();
    cs->scanner = NewScanner();
    SetScannerSpaceOption(cs->scanner, IgnoreSpaces);
    cs->data = NULL;
    cs->quit = FALSE;
    return (cs);
}

void FreeCommandScanner(cmdScannerADT cs)
{
    FreeSymbolTable(cs->cmdTable);
    FreeScanner(cs->scanner);
    FreeBlock(cs);
}

void DefineCommand(cmdScannerADT cs, string cmdName, cmdFnT cmdFn)
{
    cmdEntryT entry;
    int len;

    entry = New(cmdEntryT);
    entry->cmdName = CopyString(cmdName);
    entry->cmdFn = cmdFn;
    Enter(cs->cmdTable, cmdName, entry);
}

void SetCommandData(cmdScannerADT cs, void *data)
{
    cs->data = data;
}

void *GetCommandData(cmdScannerADT cs)
{
    return (cs->data);
}

string ReadCommandToken(cmdScannerADT cs)
{
    return (ReadToken(cs->scanner));
}

string GetCommandLine(cmdScannerADT cs)
{
    return (CopyString(cs->line));
}

string GetCommandName(cmdScannerADT cs)
{
    return (CopyString(cs->cmdName));
}

scannerADT GetTokenScanner(cmdScannerADT cs)
{
    return (cs->scanner);
}

void CommandLoop(cmdScannerADT cs, string prompt)
{
    while (!cs->quit) {
        printf("%s", prompt);
        try {
            if (!ExecuteCommand(cs, GetLine())) {
                printf("Illegal command: %s\n", cs->line);
            }
          except(ErrorException)
            printf("Error: %s\n", (string) GetExceptionValue());
        } endtry
    }
}

bool ExecuteCommand(cmdScannerADT cs, string line)
{
    cmdEntryT entry;

    cs->line = line;
    SetScannerString(cs->scanner, line);
    cs->cmdName = ReadToken(cs->scanner);
    entry = Lookup(cs->cmdTable, cs->cmdName);
    if (entry == UNDEFINED) return (FALSE);
    entry->cmdFn(cs);
    return (TRUE);
}

void QuitCmdFn(cmdScannerADT cs)
{
    cs->quit = TRUE;
}
