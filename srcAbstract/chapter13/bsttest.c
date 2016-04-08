/*
 * File: bsttest.c
 * ---------------
 * This file tests the bst.c implementation.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"
#include "scanadt.h"
#include "cmpfn.h"
#include "bst.h"

/* Private function prototypes */

static void NodeInitFn(void *np, void *kp, void *clientData);
static void ListNode(void *np, void *clientData);

/* Main program */

main()
{
    scannerADT scanner;
    bstADT bst;
    string line, cmd, key, *np;
    bool newFlag;

    scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);
    bst = NewBST(sizeof(string), StringCmpFn, NodeInitFn);
    while (TRUE) {
        printf(">");
        line = GetLine();
        SetScannerString(scanner, line);
        cmd = ConvertToLowerCase(ReadToken(scanner));
        if (StringEqual(cmd, "find")) {
            key = ReadToken(scanner);
            np = FindBSTNode(bst, &key);
            if (np == NULL) {
                printf("No such node\n");
            } else {
                 printf("Key %s found in node at %08lx\n", *np, (long) np);
             }
        } else if (StringEqual(cmd, "insert")) {
            key = ReadToken(scanner);
            np = InsertBSTNode(bst, &key, NULL);
        } else if (StringEqual(cmd, "delete")) {
            key = ReadToken(scanner);
            np = DeleteBSTNode(bst, &key);
        } else if (StringEqual(cmd, "inorder") || StringEqual(cmd, "list")) {
            MapBST(ListNode, bst, InOrder, NULL);
        } else if (StringEqual(cmd, "preorder")) {
            MapBST(ListNode, bst, PreOrder, NULL);
        } else if (StringEqual(cmd, "postorder")) {
            MapBST(ListNode, bst, PostOrder, NULL);
        } else if (StringEqual(cmd, "quit")) {
            break;
        } else {
            printf("Unrecognized command: %s\n", line);
        }
    }
}

static void NodeInitFn(void *np, void *kp, void *clientData)
{
    *((string *) np) = CopyString(*((string *) kp));
}

static void ListNode(void *np, void *clientData)
{
    printf("  %s\n", *((string *) np));
}
