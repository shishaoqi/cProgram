/*
 * File: listtest.c
 * ----------------
 * This program tests the list package by allowing the user to
 * enter a series of commands that manipulate a single global
 * list. The commands are listed in the function HelpCmd.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "scanadt.h"
#include "list.h"

/*
 * Type: dataBlockT
 * ----------------
 * This type is used to define a data block for the operation
 * of the program that makes it easier to decompose the code
 * into separate functions.  For details on the test program
 * operation, please see the function HelpCmd.
 */

typedef struct {
    listADT variables[26];
    scannerADT scanner;
} *dataBlockT;

/* Private function prototypes */

static void HelpCmd(void);
static void ParseOperation(dataBlockT db, string lhs);
static void SelectionOperation(dataBlockT db, string lhs);
static void LengthOperation(dataBlockT db, string lhs);
static void HeadOperation(dataBlockT db, string lhs);
static void TailOperation(dataBlockT db, string lhs);
static void ConsOperation(dataBlockT db, string lhs);
static void ConcatOperation(dataBlockT db, string lhs);
static dataBlockT NewDataBlock(void);
static bool IsVariableName(string token);
static void CheckForToken(scannerADT scanner, string expected);
static void *GetValue(dataBlockT db, string var);
static void SetValue(dataBlockT db, string var, void *value);
static int ListLength(listADT list);
static listADT ListConcat(listADT list1, listADT list2);
static listElementT NthElement(listADT list, int n);
static void DisplayStringList(listADT list);
static void RecDisplayList(listADT list);

/* Main program */

main()
{
    dataBlockT db;
    string line, cmd, token, var;
    int pos;
    printf("List structure test program\n");
    printf("Type \"help\" for help\n");
    db = NewDataBlock();
    while (TRUE) {
        printf(">");
        line = GetLine();
        SetScannerString(db->scanner, line);
        cmd = ConvertToLowerCase(ReadToken(db->scanner));
        if (IsVariableName(cmd)) {
            var = cmd;
            token = ReadToken(db->scanner);
            if (StringEqual(token, "")) {
                DisplayStringList(GetValue(db, var));
            } else if (StringEqual(token, "=")) {
                ParseOperation(db, var);
            } else {
                Error("Unexpected token %s", token);
            }
        } else if (StringEqual(cmd, "help")) {
            HelpCmd();
        } else if (StringEqual(cmd, "quit")) {
            break;
        } else {
            SaveToken(db->scanner, cmd);
            ParseOperation(db, NULL);
        }
    }
}

static void HelpCmd(void)
{
    printf("Enter commands in the following forms, where x, y, and z\n");
    printf("represent list variables named with a single letter:\n");
    printf("  NthElement(x, n) -- Display nth element of x\n");
    printf("  Length(x)        -- Display length of x\n");
    printf("  Head(x)          -- Display head of x\n");
    printf("* Tail(x)          -- Display tail of x\n");
    printf("* Cons(id, x)      -- Add id to front of x\n");
    printf("* Concat(x, y)     -- Concatenate lists x and y\n");
    printf("* x                -- Display value of x\n");
    printf("  quit             -- Exit from the program\n\n");
    printf("The command forms marked with a * may be preceded\n");
    printf("by a variable name and an equal sign to indicate\n");
    printf("assignment.\n\n");
}

static void ParseOperation(dataBlockT db, string lhs)
{
    string token;

    token = ConvertToLowerCase(ReadToken(db->scanner));
    if (StringEqual(token, "nthelement")) {
        SelectionOperation(db, lhs);
    } else if (StringEqual(token, "length")) {
        LengthOperation(db, lhs);
    } else if (StringEqual(token, "head")) {
        HeadOperation(db, lhs);
    } else if (StringEqual(token, "tail")) {
        TailOperation(db, lhs);
    } else if (StringEqual(token, "cons")) {
        ConsOperation(db, lhs);
    } else if (StringEqual(token, "concat")) {
        ConcatOperation(db, lhs);
    } else if (IsVariableName(token)) {
        if (lhs == NULL) Error("Null lhs in assignment");
        SetValue(db, lhs, GetValue(db, token));
    } else {
        Error("Illegal operation %s", token);
    }
}

static void SelectionOperation(dataBlockT db, string lhs)
{
    string var;
    int pos;

    if (lhs != NULL) Error("NthElement result cannot be assigned");
    CheckForToken(db->scanner, "(");
    var = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ",");
    pos = StringToInteger(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    printf("%s\n", NthElement(GetValue(db, var), pos));
}

static void LengthOperation(dataBlockT db, string lhs)
{
    string var;

    if (lhs != NULL) Error("Length result cannot be assigned");
    CheckForToken(db->scanner, "(");
    var = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    printf("%d\n", ListLength(GetValue(db, var)));
}

static void HeadOperation(dataBlockT db, string lhs)
{
    string var;

    if (lhs != NULL) Error("Head result cannot be assigned");
    CheckForToken(db->scanner, "(");
    var = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    printf("%s\n", ListHead(GetValue(db, var)));
}

static void TailOperation(dataBlockT db, string lhs)
{
    listADT result;
    string var;

    CheckForToken(db->scanner, "(");
    var = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    result = ListTail(GetValue(db, var));
    if (lhs == NULL) {
        DisplayStringList(result);
    } else {
        SetValue(db, lhs, result);
    }
}

static void ConsOperation(dataBlockT db, string lhs)
{
    listADT result;
    string id, var;

    CheckForToken(db->scanner, "(");
    id = ReadToken(db->scanner);
    CheckForToken(db->scanner, ",");
    var = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    result = ListCons(id, GetValue(db, var));
    if (lhs == NULL) {
        DisplayStringList(result);
    } else {
        SetValue(db, lhs, result);
    }
}

static void ConcatOperation(dataBlockT db, string lhs)
{
    listADT result;
    string v1, v2;

    CheckForToken(db->scanner, "(");
    v1 = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ",");
    v2 = ConvertToLowerCase(ReadToken(db->scanner));
    CheckForToken(db->scanner, ")");
    result = ListConcat(GetValue(db, v1), GetValue(db, v2));
    if (lhs == NULL) {
        DisplayStringList(result);
    } else {
        SetValue(db, lhs, result);
    }
}

static dataBlockT NewDataBlock(void)
{
    dataBlockT db;
    int i;

    db = New(dataBlockT);
    db->scanner = NewScanner();
    SetScannerSpaceOption(db->scanner, IgnoreSpaces);
    for (i = 0; i < 26; i++) {
        db->variables[i] = NULL;
    }
    return (db);
}

static bool IsVariableName(string token)
{
    if (StringEqual(token, "null")) return (TRUE);
    return (StringLength(token) == 1 && isalpha(token[0]));
}

static void CheckForToken(scannerADT scanner, string expected)
{
    string token;

    token = ReadToken(scanner);
    if (!StringEqual(token, expected)) {
        Error("Found '%s' when expecting '%s'", token, expected);
    }
}

static void *GetValue(dataBlockT db, string var)
{
    if (StringEqual(var, "null")) return (NULL);
    if (!IsVariableName(var)) {
        Error("Illegal variable name '%s'", var);
    }
    return (db->variables[var[0] - 'a']);
}

static void SetValue(dataBlockT db, string var, void *value)
{
    if (StringEqual(var, "null")) {
        Error("Cannot change the constant NULL");
    }
    if (!IsVariableName(var)) {
        Error("Illegal variable name '%s'", var);
    }
    db->variables[var[0] - 'a'] = value;
}

static int ListLength(listADT list)
{
    if (list == NULL) {
        return (0);
    } else {
        return (ListLength(ListTail(list)) + 1);
    }
}

static listADT ListConcat(listADT list1, listADT list2)
{
    if (list1 == NULL) {
        return (list2);
    } else {
        return (ListCons(ListHead(list1),
                         ListConcat(ListTail(list1), list2)));
    }
}

static listElementT NthElement(listADT list, int n)
{
    if (list == NULL) {
        Error("NthElement: No such element");
    } else if (n == 0) {
        return (ListHead(list));
    } else {
        return (NthElement(ListTail(list), n - 1));
    }
}

static void DisplayStringList(listADT list)
{
    printf("(");
    RecDisplayList(list);
    printf(")\n");
}

static void RecDisplayList(listADT list)
{
    if (list != NULL) {
        printf("%s", ListHead(list));
        if (ListTail(list) != NULL) printf(" ");
        RecDisplayList(ListTail(list));
    }
}
