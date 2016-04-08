/*
 * File: bsttab.c
 * --------------
 * This file implements the symtab.h interface using binary
 * search trees as the underlying representation.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "bst.h"
#include "cmpfn.h"
#include "symtab.h"

/*
 * Type: symtabCDT
 * ---------------
 * This type defines the underlying concrete representation
 * for a symtabADT, which is simply a binary search tree.
 */

struct symtabCDT {
    bstADT bst;
};

/*
 * Type: symtabNodeT
 * -----------------
 * This record type defines the node type for the symbol table
 * and consists of a key/value pair.
 */

typedef struct {
    string key;
    void *value;
} symtabNodeT;

/*
 * Type: callbackDataT
 * -------------------
 * This type holds the data required to build the mapping
 * function for symbol tables out of the more general one
 * for binary search trees.
 */

typedef struct {
    symtabFnT fn;
    void *clientData;
} *callbackDataT;

/* Private function prototypes */

static void InitEntry(void *np, void *kp, void *clientData);
static void FreeEntry(void *np, void *clientData);
static void MapCallbackFn(void *np, void *clientData);

/* Public entries */

symtabADT NewSymbolTable(void)
{
    symtabADT table;

    table = New(symtabADT);
    table->bst =
      NewBST(sizeof (symtabNodeT), StringCmpFn, InitEntry);
    return (table);
}

void FreeSymbolTable(symtabADT table)
{
    FreeBST(table->bst, FreeEntry);
    FreeBlock(table);
}

void Enter(symtabADT table, string key, void *value)
{
    symtabNodeT *np;

    np = InsertBSTNode(table->bst, &key, NULL);
    np->value = value;
}

void *Lookup(symtabADT table, string key)
{
    symtabNodeT *np;

    np = FindBSTNode(table->bst, &key);
    if (np == NULL) return (UNDEFINED);
    return (np->value);
}

void DeleteSymbol(symtabADT table, string key)
{
    symtabNodeT *np;

    np = DeleteBSTNode(table->bst, &key);
    if (np != NULL) {
        FreeBlock(np->key);
        FreeBlock(np);
    }
}

void MapSymbolTable(symtabFnT fn, symtabADT table,
                    void *clientData)
{
    callbackDataT cd;

    cd = New(callbackDataT);
    cd->fn = fn;
    cd->clientData = clientData;
    MapBST(MapCallbackFn, table->bst, InOrder, cd);
}

/* Private functions */

static void InitEntry(void *np, void *kp, void *clientData)
{
    ((symtabNodeT *) np)->key = CopyString(*((string *) kp));
}

static void FreeEntry(void *np, void *clientData)
{
    FreeBlock(((symtabNodeT *) np)->key);
    FreeBlock(np);
}

static void MapCallbackFn(void *np, void *clientData)
{
    symtabNodeT *ep = np;
    callbackDataT cd = clientData;

    cd->fn(ep->key, ep->value, cd->clientData);
}
