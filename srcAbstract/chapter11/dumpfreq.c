/*
 * File: dumpfreq.c
 * ----------------
 * This program goes through a file and keeps track of how
 * often each word appears.  When the input is exhausted, the
 * program writes a data file containing a table of the words
 * and their frequency in the original file.
 */

#include <stdio.h>
#include <ctype.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "scanadt.h"
#include "symtab.h"

/*
 * Constant: OutputFileName
 * ------------------------
 * This program always writes its output the the file named by
 * the following constant.
 */

#define OutputFileName "dumpfreq.dat"

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
static void DumpWordFrequencies(symtabADT table, string filename);
static void DumpEntry(string key, void *value, void *clientData);

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
    DumpWordFrequencies(table, OutputFileName);
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
 * Function: DumpWordFrequencies
 * Usage: DumpWordFrequencies(table, filename);
 * --------------------------------------------
 * This function dumps the count associated with each word
 * in the table to the specified data file.
 */

static void DumpWordFrequencies(symtabADT table, string filename)
{
    FILE *outfile;

    outfile = fopen(filename, "w");
    if (outfile == NULL) Error("Can't open output file");
    MapSymbolTable(DumpEntry, table, outfile);
    fclose(outfile);
}

/*
 * Function: DumpEntry
 * Usage: Called by MapSymbolTable
 * -------------------------------
 * This callback function is passed to MapSymbolTable to dump
 * every entry in a symbol table to a file.  The original call
 * in the client is
 *
 *    MapSymbolTable(DumpEntry, table, outfile);
 *
 * The effect of this call is to invoke DumpEntry as follows
 * on each key/value pair in the table:
 *
 *    DumpEntry(key, value, clientData);
 *
 * where clientData has the same value as outfile, although it
 * is declared as a void * pointer.
 */

static void DumpEntry(string key, void *value, void *clientData)
{
    FILE *outfile;

    outfile = clientData;
    fprintf(outfile, "%-15s%5d\n", key,
            ((counterT) value)->count);
}
