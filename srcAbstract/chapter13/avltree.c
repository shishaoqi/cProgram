/*
 * File: avltree.c
 * ---------------
 * This file implements a self-balancing AVL tree.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

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
    int bf;
} nodeT, *treeT;

/* Private function prototypes */

static treeT FindNode(treeT t, string key);
static void InsertNode(treeT *tptr, string key);
static int InsertAVL(treeT *tptr, string key);
static void FixLeftImbalance(treeT *tptr);
static void FixRightImbalance(treeT *tptr);
static void RotateLeft(treeT *tptr);
static void RotateRight(treeT *tptr);
static void DisplayTree(treeT t);
static void DisplayStructure(treeT t);
static void RecDisplayStructure(treeT t, int depth, string label);

/* Main program */

main()
{
    treeT t;
    string key;

    t = NULL;
    while (TRUE) {
        printf("Key: ");
        key = GetLine();
        if (StringEqual(key, "quit")) break;
        InsertNode(&t, key);
    }
    DisplayStructure(t);
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
 * This function calls InsertAVL and discards the result.
 */

static void InsertNode(treeT *tptr, string key)
{
    (void) InsertAVL(tptr, key);
}

/*
 * Function: InsertAVL
 * Usage: delta = InsertAVL(&t, key);
 * ----------------------------------
 * This function enters the key into the tree whose address is
 * passed as the first argument.  The return value is the change
 * in depth in the tree, which is used to correct the balance
 * factors in ancestor nodes.
 */

static int InsertAVL(treeT *tptr, string key)
{
    treeT t;
    int sign, delta;

    t = *tptr;
    if (t == NULL) {
        t = New(treeT);
        t->key = CopyString(key);
        t->bf = 0;
        t->left = t->right = NULL;
        *tptr = t;
        return (+1);
    }
    sign = StringCompare(key, t->key);
    if (sign == 0) return (0);
    if (sign < 0) {
        delta = InsertAVL(&t->left, key);
        if (delta == 0) return (0);
        switch (t->bf) {
          case +1: t->bf =  0; return (0);
          case  0: t->bf = -1; return (+1);
          case -1: FixLeftImbalance(tptr); return (0);
        }
    } else {
        delta = InsertAVL(&t->right, key);
        if (delta == 0) return (0);
        switch (t->bf) {
          case -1: t->bf =  0; return (0);
          case  0: t->bf = +1; return (+1);
          case +1: FixRightImbalance(tptr); return (0);
        }
    }
}

/*
 * Function: FixLeftImbalance
 * Usage: FixLeftImbalance(&t);
 * ----------------------------
 * This function is called when a node has been found that
 * is out of balance with the longer subtree on the left.
 * Depending on the balance factor of the left child, the
 * code performs a single or double rotation.
 */

static void FixLeftImbalance(treeT *tptr)
{
    treeT t, parent, child, *cptr;
    int oldBF;

    parent = *tptr;
    cptr = &parent->left;
    child = *cptr;
    if (child->bf != parent->bf) {
        oldBF = child->right->bf;
        RotateLeft(cptr);
        RotateRight(tptr);
        t = *tptr;
        t->bf = 0;
        switch (oldBF) {
          case -1: t->left->bf = 0; t->right->bf = +1; break;
          case  0: t->left->bf = t->right->bf = 0; break;
          case +1: t->left->bf = -1; t->right->bf = 0; break;
        }
    } else {
        RotateRight(tptr);
        t = *tptr;
        t->right->bf = t->bf = 0;
    }
}

/*
 * Function: RotateLeft
 * Usage: RotateLeft(&t);
 * ----------------------
 * This function performs a single left rotation of the tree
 * whose address is passed as an argument.  The balance factors
 * are unchanged by this function and must be corrected at a
 * higher level of the algorithm.
 */

static void RotateLeft(treeT *tptr)
{
    treeT parent, child;

    parent = *tptr;
    child = parent->right;
    parent->right = child->left;
    child->left = parent;
    (*tptr) = child;
}

/*
 * Functions: FixRightImbalance, RotateRight
 * -----------------------------------------
 * These functions are symmetrical to their FixLeftImbalance
 * and RotateLeft counterparts.
 */

static void FixRightImbalance(treeT *tptr)
{
    treeT t, parent, child, *cptr;
    int oldBF;

    parent = *tptr;
    cptr = &parent->right;
    child = *cptr;
    if (child->bf != parent->bf) {
        oldBF = child->right->bf;
        RotateRight(cptr);
        RotateLeft(tptr);
        t = *tptr;
        t->bf = 0;
        switch (oldBF) {
          case -1: t->left->bf = 0; t->right->bf = +1; break;
          case  0: t->left->bf = t->right->bf = 0; break;
          case +1: t->left->bf = -1; t->right->bf = 0; break;
        }
    } else {
        RotateLeft(tptr);
        t = *tptr;
        t->left->bf = t->bf = 0;
    }
}

static void RotateRight(treeT *tptr)
{
    treeT parent, child;

    parent = *tptr;
    child = parent->left;
    parent->left = child->right;
    child->right = parent;
    (*tptr) = child;
}

/*
 * Function: DisplayTree
 * Usage: DisplayTree(t);
 * ----------------------
 * This function displays the keys in the specified tree in
 * alphabetical order by performing an inorder traversal.
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
 * Function: DisplayStructure
 * Usage: DisplayStructure(t);
 * ---------------------------
 * This function lists the keys in the tree in a form that
 * makes its structure clear by indenting children with respect
 * to their parents and labeling left and right children.  The
 * DisplayStructure function is implemented as a simple wrapper;
 * all the work is done by RecDisplayStructure.
 */

static void DisplayStructure(treeT t)
{
    RecDisplayStructure(t, 0, NULL);
}

static void RecDisplayStructure(treeT t, int depth, string label)
{
    if (t == NULL) return;
    printf("%*s", 3 * depth, "");
    if (label != NULL) printf("%s: ", label);
    printf("%s (%s%d)\n", t->key, (t->bf > 0) ? "+" : "", t->bf);
    RecDisplayStructure(t->left, depth + 1, "L");
    RecDisplayStructure(t->right, depth + 1, "R");
}
