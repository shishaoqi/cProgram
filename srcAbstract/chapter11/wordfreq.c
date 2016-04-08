/*
 * File: wordfreq.c
 * ----------------
 * This program goes through a file and keeps track of how
 * often each word appears.  When the input is exhausted, the
 * program displays a table of the words and their frequency.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "scanadt.h"
#include "symtab.h"
#include "iterator.h"

/*
 * Type: counterT
 * --------------
 * This type is used to record the count of each word in the
 * symbol table, which requires that values be pointers.
 */

typedef struct {
    int count;
} *counterT;

/* Private function prototypes */

static void RecordWord(symtabADT table, string word);
static void DisplayWordFrequencies(symtabADT table);

/* Main program */

main()
{
    FILE *infile;
    string line, token, filename;
    scannerADT scanner;
    symtabADT table;

    scanner = NewScanner();
    table = NewSymbolTable();
    printf("Input file: ");
    filename = GetLine();
    infile = fopen(filename, "r");
    if (infile == NULL) Error("Can't open %s", filename);
    while ((line = ReadLine(infile)) != NULL) {
        SetScannerString(scanner, ConvertToLowerCase(line));
        while (MoreTokensExist(scanner)) {
            token = ReadToken(scanner);
            if (isalpha(token[0])) RecordWord(table, token);
        }
    }
    fclose(infile);
    DisplayWordFrequencies(table);
}

/*
 * Function: RecordWord
 * Usage: RecordWord(table, word);
 * -------------------------------
 * This function records the fact that another instance of
 * word has appeared.  The implementation looks up the word
 * in the symbol table and increments the corresponding
 * count, creating a new entry if necessary.
 */

static void RecordWord(symtabADT table, string word)
{
    counterT entry;

    entry = Lookup(table, word);
    if (entry == UNDEFINED) {
        entry = New(counterT);
        entry->count = 0;
        Enter(table, word, entry);
    }
    entry->count++;
}

/*
 * Function: DisplayWordFrequencies
 * Usage: DisplayWordFrequencies(table);
 * -------------------------------------
 * This function displays the count associated with each word
 * in the table.
 */

static void DisplayWordFrequencies(symtabADT table)
{
    iteratorADT iterator;
    counterT entry;
    string key;

    printf("Word frequency table:\n");
    iterator = NewIterator(table);
    while (StepIterator(iterator, &key)) {
        entry = Lookup(table, key);
        printf("%-15s%5d\n", key, entry->count);
    }
    FreeIterator(iterator);
}
