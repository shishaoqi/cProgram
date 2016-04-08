typedef struct nodeT{
  string key;
  struct nodeT *left, *right;
  int bf;
}nodeT, *treeT;

static void InsertNode(treeT *tptr, string key)
{
  (void) InsertAVL(tptr, key);
}

/* 
 * Function: InsertAVL
 * Usage: delta = InsertAVL(&t, key);
 * ----------------------------------
 * This function enters the key into the tree whose address is
 * passed as the first argument. The return value is the change
 * in depth in the tree, which is used to correct the balance
 * factors in ancestor nodes.
 */
static int InsertAVL(treeT *tptr, string key)
{
  treeT t;
  int sign, delta;

  t = *tptr;
  if(t == NULL){
    t = New(treeT);
    t->key = CopyString(key);
    t->bf = 0;
    t->left = t->right = NULL;
    *tptr = t;
    return (+1);
  }
  sign = StringCompare(key, t->key);
  if(sign == 0) return 0;
  if(sign < 0){
    delta = InsertAVL(&t->left, key);
    if(delta == 0) return 0;
    switch (t->bf){
    case 1: t->bf = 0; return 0;
    case 0: t->bf = -1; return 1;
    case -1: FixLeftImbalance (tptr); return 0;
    }
  }else{
    delta = InsertAVL(&t->right, key);
    if (delta == 0) return 0;
    switch (t->bf){
    case -1: t->bf = 0; return 0;
    case 0:  t->bf = 1; return 1;
    case 1:  FixRightImbalance (tptr); return 0;
    }
  }
}

/* 
 * Function: FixLeftImbalance
 * Usage: FixLeftImbalance(&t);
 * ----------------------------
 * This function is called when a node has been found that
 * is out of balance with the longer subtree on the left.
 * Defending on the balance factor of the left child, the
 * code performs a signle or double rotation.
 */

static void FixLeftImbalance(treeT *tptr)
{
  treeT t, parent, child, *cptr;
  int oldBF;

  parent = *tptr;
  cptr = &parent->left;
  child = *cptr;
  if(child->bf != parent->bf){
    oldBF = child->rignt->bf;
    RotateLeft(cptr);
    RotateRight(tptr);
    t = *tptr;
    t->bf = 0;
    switch(oldBF){
    case -1: t->left->bf = 0; t->right->bf = 1; break;
    case  0: t->left->bf = t->right->bf = 0; break;
    case  1: t->left->bf = -1; t->right->bf = 0; break;
    }
  }else{
    RotateRight(tptr);
    t = *tptr;
    t->right->bf = t->bf = 0;
  }
}

static void FixRightImbalance(treeT *tptr)
{
  treeT t, parent, child, *cptr;
  int oldBF;

  parent = *tptr;
  cptr = &parent->right;
  child = *cptr;
  if(child->bf != parent->bf){
    oldBF = child->left->bf;
    RotateRight(cptr);
    RotateLeft(tptr);
    t = *tptr;
    t->bf = 0;
    switch(oldBF){
    case -1: t->left->bf = 0; t->right->bf = 1; break;
    case  0: t->left->bf = t->right->bf = 0; break;
    case  1: t->left->bf = -1; t->right->bf = 0; break;
    }
  }else{
    RotateLeft(tptr);
    t = *tptr;
    t->left->bf = t->bf = 0;
  }
}

static void RotateLeft(treeT *tptr)
{
  treeT parent, child;

  parent = *tptr;
  child = parent->right;
  parent->right = child->left;
  child->left = parent;
  (*tptr) = child;
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
