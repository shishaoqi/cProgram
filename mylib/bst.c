/*
 * File: bst.c
 * -----------
 * This file implements the bst.h interface, which provides a
 * general implementation of binary search treel
 */

#include <stdio.h>
#include "genlib.h"
#include "cmpfn.h"
#include "bst.h"

/*
 * Type: treeT
 * ------------
 * Because the implementation does not know the structure of a
 * node, ponters to nodes cannot be defined explicitly and must
 * be represented using void *. For readability, the code declarees
 * any void * pointers that are in fact trees to be of type treeT.
 */
typedef void *treeT;

/*
 * Type: bstCDT
 * ------------
 * This type is the concrete type used to represent the bstADT.
 */
struct bstCDT {
    treeT root;
    int userSize, totalSize;
    cmpFnT cmpFn;
    nodeInitFnT nodeInitFn;
};

typedef struct {
    int bf;
    treeT left, right;
} bstDataT;

struct deleteE {
    int R;
    void *target;
};

/* Private function prototypes */
static treeT RecFindNode(bstADT bst, treeT t, void *kp);
static int RecInsertNode(bstADT bst, treeT *tptr, void *kp, void *clientData);
static deleteT RecDeleteNode(bstADT bst, treeT *tptr, void *kp, deleteT d);
static int DeleteTargetNode(bstADT bst, treeT *tptr);
static void RecMapBST(nodeFnT fn, bstADT bst, treeT t, OrderT order, void *clientData);
static bstDataT *BSTData(bstADT bst, treeT t);
static void FixLeftImbalance(bstADT bst, treeT *tptr);
static void FixRightImbalance(bstADT bst, treeT *tptr);
static int Delete_FixLeftImbalance(bstADT bst, treeT *tptr);
static int Delete_FixRightImbalance(bstADT bst, treeT *tptr);
static int tree_height(bstADT bst, treeT root, int height);
static void RotateLeft(bstADT bst, treeT *tptr);
static void RotateRight(bstADT bst, treeT *tptr);
static int RecRight(bstADT bst, bstDataT *tdp, treeT *rptr, treeT *tptr);

/* Exported entries */

bstADT NewBST(int size, cmpFnT cmpFn, nodeInitFnT nodeInitFn)
{
    bstADT bst;

    bst = New(bstADT);
    bst->root = NULL;
    bst->userSize = size;
    bst->totalSize = bst->userSize + sizeof(bstDataT);
    bst->cmpFn = cmpFn;
    bst->nodeInitFn = nodeInitFn;
    return bst;
}

void FreeBST(bstADT bst, nodeFnT freeNodeFn)
{
    MapBST(freeNodeFn, bst, PostOrder, NULL);
    FreeBlock(bst);
}

void *FindBSTNode(bstADT bst, void *kp)
{
    return ( RecFindNode(bst, bst->root, kp) );
}

static treeT RecFindNode(bstADT bst, treeT t, void *kp)
{
    bstDataT *dp;
    int sign;

    if (t == NULL)  return NULL;
    sign = bst->cmpFn(kp, t);
    if (sign == 0)  return t;
    dp = BSTData(bst, t);
    if (sign < 0) {
        return ( RecFindNode(bst, dp->left, kp) );
    } else {
        return ( RecFindNode(bst, dp->right, kp) );
    }
}

int InsertBSTNode(bstADT bst, void *kp, void *clientData)
{
    return (RecInsertNode(bst, &bst->root, kp, clientData));
}

static int RecInsertNode(bstADT bst, treeT *tptr, void *kp, void *clientData)
{
    bstDataT *dp;
    treeT t;
    int sign, delta;

    t = *tptr;
    if (t == NULL) {
        t = GetBlock(bst->totalSize);
        /*if (bst->cmpFn == StringCmpFn)
            bst->nodeInitFn(t, kp, clientData);
        else
            bst->nodeInitFn(t, kp, clientData);*/
        bst->nodeInitFn(t, kp, clientData);
        dp = BSTData(bst, t);
        dp->bf = 0;
        dp->left = dp->right = NULL;
        *tptr = t;
        return 1;
    }

    sign = bst->cmpFn(kp, t);

    if (sign == 0) return 0;

    dp = BSTData(bst, t);
    if (sign < 0 ) {
        delta = RecInsertNode(bst, &dp->left, kp, clientData);
        if (delta == 0) return 0;
        switch (dp->bf) {
        case 1: dp->bf = 0; return 0;
        case 0: dp->bf = -1; return 1;
        case -1: FixLeftImbalance (bst, tptr); return 0;
        }
    } else {
        delta = RecInsertNode(bst, &dp->right, kp, clientData);
        if (delta == 0) return 0;
        switch (dp->bf) {
        case -1: dp->bf = 0; return 0;
        case 0:  dp->bf = 1; return 1;
        case 1:  FixRightImbalance (bst, tptr); return 0;
        }
    }
}

static void FixLeftImbalance(bstADT bst, treeT *tptr)
{
    treeT t, parent, child, *cptr;
    bstDataT *pdp, *cdp;
    int oldBF;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    cptr = &pdp->left;
    child = pdp->left;
    cdp = BSTData(bst, child);
    if (cdp->bf != pdp->bf) {
        cdp = BSTData(bst, cdp->right);
        oldBF = cdp->bf;
        RotateLeft(bst, cptr);
        RotateRight(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        pdp->bf = 0;
        switch (oldBF) {
        case -1: {
            cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
            cdp = BSTData(bst, pdp->right); cdp->bf = 1; break;
        }
        case  0: {
            cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
            cdp = BSTData(bst, pdp->right);  cdp->bf = 0; break;
        }
        case  1: {
            cdp = BSTData(bst, pdp->left);   cdp->bf = -1;
            cdp = BSTData(bst, pdp->right);  cdp->bf = 0; break;
        }
        }
    } else {
        RotateRight(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        cdp = BSTData(bst, pdp->right);
        pdp->bf = cdp->bf = 0;
    }
}

static void FixRightImbalance(bstADT bst, treeT *tptr)
{
    treeT t, parent, child, *cptr;
    bstDataT *pdp, *cdp;
    int oldBF;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    cptr = &pdp->right;
    child = pdp->right;
    cdp = BSTData(bst, child);
    if (cdp->bf != pdp->bf) {
        cdp = BSTData(bst, cdp->left);
        oldBF = cdp->bf;
        RotateRight(bst, cptr);
        RotateLeft(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        pdp->bf = 0;
        switch (oldBF) {
        case -1: {
            cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
            cdp = BSTData(bst, pdp->right); cdp->bf = 1; break;
        }
        case  0: {
            cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
            cdp = BSTData(bst, pdp->right); cdp->bf = 0; break;
        }
        case  1: {
            cdp = BSTData(bst, pdp->left);   cdp->bf = -1;
            cdp = BSTData(bst, pdp->right);  cdp->bf = 0; break;
        }
        }
    } else {
        RotateLeft(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        cdp = BSTData(bst, pdp->left);
        cdp->bf = pdp->bf = 0;
    }
}

static void RotateLeft(bstADT bst, treeT *tptr)
{
    treeT parent, child;
    bstDataT *pdp, *cdp;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    child = pdp->right;
    cdp = BSTData(bst, child);
    pdp->right = cdp->left;
    cdp->left = parent;
    (*tptr) = child;
}

static void RotateRight(bstADT bst, treeT *tptr)
{
    treeT parent, child;
    bstDataT *pdp, *cdp;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    child = pdp->left;
    cdp = BSTData(bst, child);
    pdp->left = cdp->right;
    cdp->right = parent;
    (*tptr) = child;
}

bool DeleteBSTNode(bstADT bst, void *kp)
{
    bool notNull;
    deleteT d;
    d = New(deleteT);
    d = RecDeleteNode(bst, &bst->root, kp, d);
    if (d->target == NULL) {
        notNull = FALSE;
    } else {
        notNull = TRUE;
    }
    FreeBlock(d->target);
    FreeBlock(d);
    return notNull;
}

static deleteT RecDeleteNode(bstADT bst, treeT *tptr, void *kp, deleteT delta)
{
    bstDataT *dp, *cdp, *newdp, *cl_dp, *cr_dp;
    treeT t, *ctr;
    int sign, r, height;

    t = *tptr;
    if (t == NULL) {
        delta->target = t;
        delta->R = 0;
        return delta;
    }

    sign = bst->cmpFn(kp, t);
    dp = BSTData(bst, t);
    if (sign < 0) {
        delta = RecDeleteNode(bst, &dp->left, kp, delta);
        if (delta->R == 0) return delta;
        switch (dp->bf) {
        case 1: {
            height = tree_height(bst, dp->right, 0);
            if (height >= 3) {
                r = Delete_FixRightImbalance (bst, tptr);
                delta->R = r;
                return delta;
            } else if (height == 2) {
                cdp = BSTData(bst, dp->right);
                if (cdp->bf == -1) {
                    FixRightImbalance(bst, tptr); delta->R = 1;
                } else if (cdp->bf == 0) {
                    RotateLeft(bst, tptr);
                    t = *tptr;
                    newdp = BSTData(bst, t); newdp->bf = -1;
                    cl_dp = BSTData(bst, newdp->left);
                    cl_dp->bf = 1; delta->R = 0;
                } else {
                    RotateLeft(bst, tptr);
                    t = *tptr;
                    newdp = BSTData(bst, t);
                    newdp->bf = 0; delta->R = 1;
                    cl_dp = BSTData(bst, newdp->left);
                    cl_dp->bf = 0;
                }
                return delta;
            }
        }
        case 0:  dp->bf = 1; delta->R = 0; return delta;
        case -1: dp->bf = 0; delta->R = 1; return delta;
        }
    } else if (sign > 0) {
        delta = RecDeleteNode(bst, &dp->right, kp, delta);
        if (delta->R == 0) return delta;
        switch (dp->bf) {
        case -1: {
            height = tree_height(bst, dp->left, 0);
            if (height >= 3) {
                r = Delete_FixLeftImbalance (bst, tptr); delta->R = r;
                return delta;
            } else if (height == 2) {
                cdp = BSTData(bst, dp->left);
                if (cdp->bf == -1) {
                    RotateRight(bst, tptr);
                    newdp = BSTData(bst, *tptr);
                    newdp->bf = 0; delta->R = 1;
                    cr_dp = BSTData(bst, newdp->right); cr_dp->bf = 0;
                } else if (cdp->bf == 0) {
                    RotateRight(bst, tptr);
                    t = *tptr;
                    newdp = BSTData(bst, t); newdp->bf = 1;
                    cr_dp = BSTData(bst, newdp->right);
                    cr_dp->bf = -1; delta->R = 0;
                } else {
                    FixLeftImbalance (bst, tptr); delta->R = 1;
                }
                return delta;
            }
        }
        case 0:  dp->bf = -1; delta->R = 0; return delta;
        case 1:  dp->bf = 0; delta->R = 1; return delta;
        }
    } else {
        if (dp->left == NULL) {
            *tptr = dp->right;
            delta->target = t;
            delta->R = 1;
            return delta;
        } else if (dp->right == NULL) {
            *tptr = dp->left;
            delta->target = t;
            delta->R = 1;
            return delta;
        } else {
            ctr = &dp->left;
            cdp = BSTData(bst, *ctr);
            while (cdp->right != NULL) {
                ctr = &cdp->right;
                cdp = BSTData(bst, *ctr);
            }

            if (bst->cmpFn(*ctr, dp->left) == 0) {
                *tptr = *ctr;
                *ctr = cdp->left;
                cdp->bf = dp->bf;
                cdp->right = dp->right;
                delta->target = t;
                switch (dp->bf) {
                case  1: {
                    height = tree_height(bst, dp->right, 0);
                    if (height >= 3) {
                        r = Delete_FixRightImbalance (bst, tptr);
                        delta->R = r;
                        return delta;
                    } else if (height == 2) {
                        cdp = BSTData(bst, dp->right);
                        if (cdp->bf == -1) {
                            FixRightImbalance(bst, tptr); delta->R = 1;
                        } else if (cdp->bf == 0) {
                            RotateLeft(bst, tptr);
                            t = *tptr;
                            newdp = BSTData(bst, t); newdp->bf = -1;
                            cl_dp = BSTData(bst, newdp->left);
                            cl_dp->bf = 1; delta->R = 0;
                        } else {
                            RotateLeft(bst, tptr);
                            t = *tptr;
                            newdp = BSTData(bst, t);
                            newdp->bf = 0; delta->R = 1;
                            cl_dp = BSTData(bst, newdp->left);
                            cl_dp->bf = 0;
                        }
                        return delta;
                    }
                }
                case  0: cdp->bf = 1; delta->R = 0; return delta;
                case -1: cdp->bf = 0; delta->R = 1; return delta;
                }
            } else {
                *tptr = *ctr;
                cdp->bf = dp->bf;
                delta = RecDeleteNode(bst, &dp->left, *ctr, delta);
                cdp->left = dp->left;
                cdp->right = dp->right;
                delta->target = t;
                if (delta->R == 0) return delta;
                switch (dp->bf) {
                case  1: {
                    height = tree_height(bst, dp->right, 0);
                    if (height >= 3) {
                        r = Delete_FixRightImbalance (bst, tptr);
                        delta->R = r; return delta;
                    } else if (height == 2) {
                        cdp = BSTData(bst, dp->right);
                        if (cdp->bf == -1) {
                            FixRightImbalance(bst, tptr); delta->R = 1;
                        } else if (cdp->bf == 0) {
                            RotateLeft(bst, tptr);
                            t = *tptr;
                            newdp = BSTData(bst, t); newdp->bf = -1;
                            cl_dp = BSTData(bst, newdp->left);
                            cl_dp->bf = 1; delta->R = 0;
                        } else {
                            RotateLeft(bst, tptr);
                            t = *tptr;
                            newdp = BSTData(bst, t);
                            newdp->bf = 0; delta->R = 1;
                            cl_dp = BSTData(bst, newdp->left);
                            cl_dp->bf = 0;
                        }
                        return delta;
                    }
                }
                case  0: cdp->bf = 1; delta->R = 0; return delta;
                case -1: cdp->bf = 0; delta->R = 1; return delta;
                }
            }
        }
    }
}

static int tree_height(bstADT bst, treeT root, int height)
{
    bstDataT *pdp;
    int leftHeight, rightHeight;

    if (height == 4) return 0;
    if (root == NULL) return 0;
    height++;
    pdp = BSTData(bst, root);
    leftHeight = tree_height(bst, pdp->left, height) + 1;
    rightHeight = tree_height(bst, pdp->right, height) + 1;
    return leftHeight >= rightHeight ? leftHeight : rightHeight;
}

static int Delete_FixLeftImbalance(bstADT bst, treeT *tptr)
{
    treeT t, parent, child, *cptr;
    bstDataT *pdp, *cdp, *cl_dp, *cr_dp;
    int ll, lr;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    cptr = &pdp->left;
    child = pdp->left;
    cdp = BSTData(bst, child);
    if (cdp->bf != pdp->bf) {
        cl_dp = BSTData(bst, cdp->left);  ll = cl_dp->bf;
        cr_dp = BSTData(bst, cdp->right); lr = cr_dp->bf;
        RotateLeft(bst, cptr);
        RotateRight(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        switch (cdp->bf) {
        case  0: { //L == 0;
            if (lr == -1) { //lr == -1;
                pdp->bf = -1;
                switch (ll) {
                case -1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                case  0:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                case  1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                }
            } else if (lr == 0) { //lr == 0;
                pdp->bf = -1;
                switch (ll) {
                case -1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 0;
                case  0:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 0;
                case  1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 0;
                }
            } else { //lr == 1;
                switch (ll) {
                case -1:
                    pdp->bf = 0;
                    RotateRight(bst, &pdp->left);
                    cdp = BSTData(bst, pdp->left); cdp->bf = 0;
                    cr_dp = BSTData(bst, cdp->right); cr_dp->bf = 0;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 1;
                case  0:
                    pdp->bf = -1;
                    RotateRight(bst, &pdp->left);
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 1;
                    cr_dp = BSTData(bst, cdp->right);  cr_dp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 0;
                case  1:
                    pdp->bf = 0;
                    cdp = BSTData(bst, pdp->left); cdp->bf = -1;
                    FixLeftImbalance(bst, &pdp->left);

                    cdp = BSTData(bst, pdp->right); cdp->bf = 0; return 1;
                }
            }
        }
        case  1: { //L == 1;
            pdp->bf = 0;
            switch (lr) {
            case -1:
                cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 1;
            case  0:
                cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 1;
            case  1:
                cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 1;
            }
        }
        }
    } else { //L == -1;
        RotateRight(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        cdp = BSTData(bst, pdp->right);
        pdp->bf = cdp->bf = 0; return 1;
    }
}

static int Delete_FixRightImbalance(bstADT bst, treeT *tptr)
{
    treeT t, parent, child, *cptr;
    bstDataT *pdp, *cdp, *cl_dp, *cr_dp;
    int rl, rr;

    parent = *tptr;
    pdp = BSTData(bst, parent);
    cptr = &pdp->right;
    child = pdp->right;
    cdp = BSTData(bst, child);
    if (cdp->bf != pdp->bf) {
        cl_dp = BSTData(bst, cdp->left);   rl = cl_dp->bf; //mean "rl"
        cr_dp = BSTData(bst, cdp->right);  rr = cr_dp->bf;
        RotateRight(bst, cptr);
        RotateLeft(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        switch (cdp->bf) {
        case  0: { //r == 0;
            if (rl == -1) { //rl == -1;
                switch (rr) {
                case -1:
                    pdp->bf = 0;
                    cdp = BSTData(bst, pdp->right); cdp->bf = 1;
                    FixRightImbalance(bst, &pdp->right);
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0; return 1;//last problem
                case  0:
                    pdp->bf = 1;
                    RotateLeft(bst, &pdp->right);
                    cdp = BSTData(bst, pdp->right);
                    cdp->bf = -1;
                    cl_dp = BSTData(bst, cdp->left);
                    cl_dp->bf = 1;

                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0; return 0;
                case  1:
                    pdp->bf = 0;
                    RotateLeft(bst, &pdp->right);
                    cdp = BSTData(bst, pdp->right); cdp->bf = 0;
                    cl_dp = BSTData(bst, cdp->left); cl_dp->bf = 0;
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0; return 1;
                }
            } else if (rl == 0) { //rl == 0;
                pdp->bf = 1;
                switch (rr) {
                case -1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                    cdp = BSTData(bst, pdp->right); cdp->bf = 1; return 0;
                case  0:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                case  1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                }
            } else { //rl == 1;
                pdp->bf = 1;
                switch (rr) {
                case -1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                case  0:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                case  1:
                    cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                    cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 0;
                }
            }
        }

        case  -1: { //r == -1;
            pdp->bf = 0;
            switch (rl) {
            case -1:
                cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 1; return 1;
            case  0:
                cdp = BSTData(bst, pdp->left);  cdp->bf = 0;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 1;
            case  1:
                cdp = BSTData(bst, pdp->left);  cdp->bf = -1;
                cdp = BSTData(bst, pdp->right);  cdp->bf = 0; return 1;
            }
        }
        }
    } else { //r == 1;
        RotateLeft(bst, tptr);
        t = *tptr;
        pdp = BSTData(bst, t);
        cdp = BSTData(bst, pdp->left);
        cdp->bf = pdp->bf = 0; return 1;
    }
}

void MapBST(nodeFnT fn, bstADT bst, OrderT order, void *clientData)
{
    RecMapBST(fn, bst, bst->root, order, clientData);
}

static void RecMapBST(nodeFnT fn, bstADT bst, treeT t, OrderT order, void *clientData)
{
    bstDataT *dp;

    if (t != NULL) {
        dp = BSTData(bst, t);
        if (order == PreOrder) fn(t, clientData);
        RecMapBST(fn, bst, dp->left, order, clientData);
        if (order == InOrder) fn(t, clientData);
        RecMapBST(fn, bst, dp->right, order, clientData);
        if (order == PostOrder) fn(t, clientData);
    }
}

void *BSTRoot(bstADT bst)
{
    return (bst->root);
}

void *BSTLeftChild(bstADT bst, void *np)
{
    bstDataT *dp;

    if (np == NULL) Error("BSTLeftChild: Argument is NULL");
    dp = BSTData(bst, np);
    return (dp->left);
}

void *BSTRightChild(bstADT bst, void *np)
{
    bstDataT *dp;
    if (np == NULL) Error("BSTRightChild: Argument is NULL");
    dp = BSTData(bst, np);
    return (dp->right);
}

static bstDataT *BSTData(bstADT bst, treeT t)
{
    return  (bstDataT *) ((char *)t + bst->userSize );
}
