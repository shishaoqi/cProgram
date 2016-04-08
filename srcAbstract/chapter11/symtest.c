/*
 * File: symtest.c
 * ---------------
 * This program tests the symbol table abstraction.  The user
 * types in command lines, which are assumed to be in one of
 * the following forms:
 *
 *    key               Display the current value of key
 *    key = value       Enter new value for key
 *    quit              Exit from the program
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "symtab.h"
#include "scanadt.h"

/* Main program */

main()
{
    symtabADT table;
    scannerADT scanner;
    string line, name, value;

    table = NewSymbolTable();
    scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);
    while (TRUE) {
        printf("-> ");
        line = GetLine();
        if (StringEqual(line, "quit")) break;
        SetScannerString(scanner, line);
        name = ReadToken(scanner);
        if (MoreTokensExist(scanner)) {
            if (!StringEqual(ReadToken(scanner), "=")) {
                Error("Missing equal sign in assignment");
            }
            value = ReadToken(scanner);
            if (MoreTokensExist(scanner)) {
                Error("Statement contains additional data");
            }
            Enter(table, name, value);
        } else {
            value = Lookup(table, name);
            if (value == UNDEFINED) {
                printf("'%s' is undefined.\n", name);
            } else {
                printf("%s\n", value);
            }
        }
    }
    FreeSymbolTable(table);
}
