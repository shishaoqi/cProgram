/*
 * File: symtest.c
 * ---------------
 * This program tests the symbol table and cmdtable abstractions.
 * For a description of the legal commands, see the definition
 * of HelpCmdFn.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"
#include "iterator.h"
#include "scanadt.h"
#include "cmdscan.h"

/* Private function prototypes */

static void InitCommandTable(cmdScannerADT cs);
static void LookupCmdFn(cmdScannerADT cs);
static void EnterCmdFn(cmdScannerADT cs);
static void ClearCmdFn(cmdScannerADT cs);
static void ListCmdFn(cmdScannerADT cs);
static void HelpCmdFn(cmdScannerADT cs);
static void CheckForExtraTokens(cmdScannerADT cs);

/* Main program */

main()
{
    cmdScannerADT cs;

    printf("Test program for the symbol table package\n");
    cs = NewCommandScanner();
    SetCommandData(cs, NewSymbolTable());
    InitCommandTable(cs);
    CommandLoop(cs, "-> ");
    FreeSymbolTable(GetCommandData(cs));
    FreeCommandScanner(cs);
}

static void InitCommandTable(cmdScannerADT cs)
{
    DefineCommand(cs, "lookup", LookupCmdFn);
    DefineCommand(cs, "enter",  EnterCmdFn);
    DefineCommand(cs, "clear",  ClearCmdFn);
    DefineCommand(cs, "list",   ListCmdFn);
    DefineCommand(cs, "help",   HelpCmdFn);
    DefineCommand(cs, "quit",   QuitCmdFn);
}

static void LookupCmdFn(cmdScannerADT cs)
{
    symtabADT table;
    string key, value;

    table = GetCommandData(cs);
    key = ReadCommandToken(cs);
    CheckForExtraTokens(cs);
    value = Lookup(table, key);
    if (value == UNDEFINED) {
        printf("'%s' is undefined.\n", key);
    } else {
        printf("%s\n", value);
    }
}

static void EnterCmdFn(cmdScannerADT cs)
{
    symtabADT table;
    string key, token, value;

    table = GetCommandData(cs);
    key = ReadCommandToken(cs);
    token = ReadCommandToken(cs);
    if (!StringEqual(token, "=")) Error("Enter: Missing =");
    value = ReadCommandToken(cs);
    CheckForExtraTokens(cs);
    Enter(table, key, value);
}

static void ClearCmdFn(cmdScannerADT cs)
{
    CheckForExtraTokens(cs);
    FreeSymbolTable(GetCommandData(cs));
    SetCommandData(cs, NewSymbolTable());
}

static void ListCmdFn(cmdScannerADT cs)
{
    symtabADT table;
    string key, value;
    iteratorADT iterator;

    CheckForExtraTokens(cs);
    table = GetCommandData(cs);
    iterator = NewIterator(table);
    while (StepIterator(iterator, &key)) {
        value = Lookup(table, key);
        printf("%s = %s\n", key, value);
    }
    FreeIterator(iterator);
}

static void HelpCmdFn(cmdScannerADT cs)
{
    printf("Legal commands:\n");
    printf("  enter key = value   Enter new value for key\n");
    printf("  lookup key          Lookup the current value of key\n");
    printf("  clear               Clear all entries from the table\n");
    printf("  list                List the defined symbols\n");
    printf("  help                Generate a list of commands\n");
    printf("  quit                Exit from the program\n");
}

static void CheckForExtraTokens(cmdScannerADT cs)
{
    string token;

    token = ReadCommandToken(cs);
    if (!StringEqual(token, "")) {
        Error("%s: Found %s when expecting end of line",
              GetCommandName(cs), token);
    }
}
