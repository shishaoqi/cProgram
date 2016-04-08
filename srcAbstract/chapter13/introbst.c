/*
 * File: introbst.c
 * ----------------
 * This program introduces the standard operations used for
 * binary search trees and serves as a model for the more
 * general bst.h interface.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"

/*
 * Types: nodeT, treeT
 * -------------------
 * These types define the structure of the binary search tree.
 * The type nodeT is a record that corresponds to an individual
 * node; the type treeT is simply a pointer to a node.
 */

typedef struct nodeT {
    string key;
    struct nodeT *left, *right;
} nodeT, *treeT;

/* Private function prototypes */

static void TestSearch(treeT tree, string key);
static treeT FindNode(treeT t, string key);
static void InsertNode(treeT *tptr, string key);
static void PreOrderWalk(treeT t);
static void PostOrderWalk(treeT t);
static void DisplayTree(treeT t);

/* Main program */

main()
{
    treeT dwarfTree;

    dwarfTree = NULL;
    InsertNode(&dwarfTree, "Grumpy");
    InsertNode(&dwarfTree, "Doc");
    InsertNode(&dwarfTree, "Sleepy");
    InsertNode(&dwarfTree, "Bashful");
    InsertNode(&dwarfTree, "Dopey");
    InsertNode(&dwarfTree, "Happy");
    InsertNode(&dwarfTree, "Sneezy");
    TestSearch(dwarfTree, "Grumpy");
    TestSearch(dwarfTree, "Sleepy");
    TestSearch(dwarfTree, "Snow White");
}

/*
 * Function: TestSearch
 * Usage: TestSearch(tree, key);
 * -----------------------------
 * This function writes out the result of calling FindNode on
 * the specified parameters.
 */

static void TestSearch(treeT tree, string key)
{
    treeT np;

    np = FindNode(tree, key);
    printf("FindNode(tree, \"%s\") returns ", key);
    if (np == NULL) {
        printf("NULL");
    } else {
        printf("a pointer to a node at %08lx", (long) np);
        if (!StringEqual(key, np->key)) {
            Error("Search failed");
        }
    }
    printf("\n");
}

/*
 * Function: FindNode
 * Usage: np = FindNode(t, key);
 * -----------------------------
 * This function searches the tree t for a node containing key.
 * If it finds it, it returns a pointer to that node, which can
 * also be regarded as the subtree rooted at that point.  If
 * the key does not appear, FindNode returns NULL.
 */

static treeT FindNode(treeT t, string key)
{
    int sign;

    if (t == NULL) return (NULL);
    sign = StringCompare(key, t->key);
    if (sign == 0) return (t);
    if (sign < 0) {
        return (FindNode(t->left, key));
    } else {
        return (FindNode(t->right, key));
    }
}

/*
 * Function: InsertNode
 * Usage: InsertNode(&t, key);
 * ---------------------------
 * This function inserts the specified key into the tree, which
 * is passed by reference as the first argument to InsertNode.
 * If the key already exists in the tree, the call to InsertNode
 * returns without making any additional changes.
 */

static void InsertNode(treeT *tptr, string key)
{
    treeT t;
    int sign;

    t = *tptr;
    if (t == NULL) {
        t = New(treeT);
        t->key = CopyString(key);
        t->left = t->right = NULL;
        *tptr = t;
        return;
    }
    sign = StringCompare(key, t->key);
    if (sign == 0) return;
    if (sign < 0) {
        InsertNode(&t->left, key);
    } else {
        InsertNode(&t->right, key);
    }
}

/*
 * Functions: DisplayTree
 * Usage: DisplayTree(t);
 * ----------------------
 * These functions displays the keys in the specified tree
 * in alphabetical order by keys.  This function performs an
 * inorder traversal.
 */

static void DisplayTree(treeT t)
{
    if (t != NULL) {
        DisplayTree(t->left);
        printf("%s\n", t->key);
        DisplayTree(t->right);
    }
}

/*
 * Functions: PreOrderWalk, PostOrderWalk
 * Usage: PreOrderWalk(t);
 *        PostOrderWalk(t);
 * --------------------------------------
 * These functions perform the indicated type of tree traversal
 * on the tree t, displaying the keys in each node as it goes.
 * PreOrderWalk processes the current node before making the
 * recursive calls; PostOrderWalk processes the current node
 * after the recursive calls.  The DisplayTree function earlier
 * in the file processes the current node between the recursive
 * calls and is sometimes called an "inorder" walk.
 */

static void PreOrderWalk(treeT t)
{
    if (t != NULL) {
        printf("%s\n", t->key);
        PreOrderWalk(t->left);
        PreOrderWalk(t->right);
    }
}

static void PostOrderWalk(treeT t)
{
    if (t != NULL) {
        PostOrderWalk(t->left);
        PostOrderWalk(t->right);
        printf("%s\n", t->key);
    }
}
