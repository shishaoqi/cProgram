#include<stdio.h>
#include<ctype.h>
#include"genlib.h"
#include"simpio.h"
#include"strlib.h"
#include"scanadt.h"
#include"symtab.h"
#include"iterator1.h"


typedef struct{
	int count;
} *counterT;


static void RecordWord (symtabADT table, string word);
static void DisplayWordFrequencies(symtabADT table);

/* Display wordfrequencies in screen */
void DisplayEntry(string key, void *value, void *clientData);

/* Save wordfrequencies in the file that name of temp */
void DumpWordFrequencies(symtabADT table, string filename);
void DumpEntry(string key, void *value, void *clientData);

/* use iterator to implements the DumpWordFrequencies */
static void DumpWordFrequencies_second_edition(symtabADT table);  

int main()
{
	FILE *infile;
	string line, token, filename,outFN;
	scannerADT scanner;
	symtabADT table;

	scanner = NewScanner();
	table = NewSymbolTable();
	printf("Input file: ");
	filename = GetLine();
	outFN= "./symtabtestOutputFile";

	infile = fopen(filename, "r");
	if(infile == NULL)  Error("Can't open %s", filename);
	while((line = ReadLine(infile)) != NULL)
	{
		SetScannerString (scanner, ConvertToLowerCase(line));
		while(MoreTokensExist(scanner)){
			token = ReadToken(scanner);
			if(isalpha(token[0]))
				RecordWord(table, token);
		}
	}
   
     /**
      *first edition
      *pass additional state information to the function DumpEntry 
      */  
   	 DumpWordFrequencies(table, outFN);
   	
   	/**
	 * second edition
	 * use iterator implements the DumpWordFrequencies
	 */
    //DumpWordFrequencies_second_edition(table);
   	
   	fclose(infile);
	return 0;
}

static void RecordWord (symtabADT table, string word)
{
	counterT entry;

	entry = (counterT)Lookup(table, word);
	if(entry == UNDEFINED)
	{
		entry = New(counterT);
		entry->count = 0;
		Enter(table, word, entry);
	}
	entry->count++;
}

/* Show wordFrequencies on screen */
static void DisplayWordFrequencies(symtabADT table)
{
	printf("Word Frequency table:\n");
	MapSymbolTable(DisplayEntry, table, NULL);
}

void DisplayEntry(string key, void *value, void *clientData)
{
	printf("%-15s%5d\n", key, ((counterT)value)->count);
}


/* Save the result in a file
 * pass additional state information to DumpEntry
 */
void DumpWordFrequencies(symtabADT table, string filename)
{
    FILE *outfile;

    outfile = fopen(filename, "w");
	if(outfile == NULL) Error("Can't open output file");
	MapSymbolTable(DumpEntry, table, outfile);
	fclose(outfile);
}

void DumpEntry(string key, void *value, void *clientData)
{
	FILE *outfile;

	outfile = (FILE *)clientData;
	fprintf(outfile, "%-15s%5d\n", key, ((counterT)(value))->count );
}


/* use iterator to implements the DumpWordFrequencies 
 * show the result on the screen
 */
static void DumpWordFrequencies_second_edition(symtabADT table)
{
	iteratorADT iterator;
	counterT entry;
	string key;

	printf("Word frequency table:\n");
	iterator = NewIterator(table);
	while(StepIterator(iterator, &key)){
		entry=(counterT)Lookup(table, key);
		printf("%-15s%5d\n", key, entry->count );
	}
	FreeIterator(iterator);
}
