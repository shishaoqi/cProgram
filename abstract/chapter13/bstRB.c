#include "genlib.h"

#define BLACK 'b'
#define RED 'r'

typedef struct nodeT{
  int key;
  char color;
  struct nodeT *parent, *left, *right;
}nodeT, *treeT;

typedef enum { InOrder, PreOrder, PostOrder } OrderT;
typedef void (*nodeFnT) (int np, void *clientData);

static void InsertRB(treeT *tptr, treeT parent, int key);
static void RotateLeft(treeT *tptr);
static void RotateRight(treeT *tptr);
void insert_case1(treeT n);
void insert_case2(treeT n);
void insert_case3(treeT n);
void insert_case4(treeT n);
void insert_case5(treeT n);
static void RecMapRBT(nodeFnT fn, treeT root, OrderT order, void *clientData);

void InsertNode(treeT *tptr, int key)
{ 
  InsertRB(tptr, NULL, key);
}

static void InsertRB(treeT *tptr, treeT parent, int key)
{
  treeT *t;
  int sign;

  t = tptr;
  if(t == NULL){
    t = New(treeT*);
    t->key = key;
	t->parent = parent;
	/*if(sign == -1)
      parent->left = t;
	else if(sign == 1)
	  parent->right = t;
	  */
    t->left = t->right = NULL;
	insert_case1(t);
    tptr = t;
    return;
  }
  sign = (key<t->key) ? -1:1;
  if(sign == 0) return;
  if(sign < 0)
    InsertRB(&t->left, t, key);
  else
    InsertRB(&t->right, t, key);
}

treeT grandparent(treeT n){
  return n->parent->parent;
}

treeT uncle(treeT n){
  if(n->parent == grandparent(n)->left)
         return grandparent(n)->right;
     else
         return grandparent(n)->left;
}

void insert_case1(treeT n){
     if (n->parent == NULL)
         n->color = BLACK;
     else
         insert_case2(n);
}

void insert_case2(treeT n) {
     if (n->parent->color == BLACK)
         return; /* Ê÷ÈÔ¾ÉÓÐÐ§ */
     else
         insert_case3(n);
}

void insert_case3(treeT n){
     if (uncle(n) != NULL && uncle(n)->color == RED) {
         n->parent->color = BLACK;
         uncle(n)->color = BLACK;
         grandparent(n)->color = RED;
         insert_case1(grandparent(n));
     }
     else
         insert_case4(n);
}

void insert_case4(treeT n){
     if (n == n->parent->right && n->parent == grandparent(n)->left){
         RotateLeft(&n->parent);
         n = n->left;
     } else if (n == n->parent->left && n->parent == grandparent(n)->right){
         RotateRight(&n->parent);
         n = n->right;
     }
     insert_case5(n);
}

void insert_case5(treeT n){
     treeT temp;
     n->parent->color = BLACK;
     grandparent(n)->color = RED;
     if (n == n->parent->left && n->parent == grandparent(n)->left) {
     temp = grandparent(n);         
     RotateRight(&temp);
     } else {
        /* Here, n == n->parent->right && n->parent == grandparent(n)->right */
       temp = grandparent(n);     
       RotateLeft(&temp);
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

void MapRBT(nodeFnT fn, treeT root, OrderT order, void *clientData)
{
	RecMapRBT(fn, root, order, clientData);
}

static void RecMapRBT(nodeFnT fn, treeT root, OrderT order, void *clientData)
{

	if(root != NULL){
		if(order == PreOrder) fn(root->key, clientData);
		RecMapRBT(fn, root->left, order, clientData);
		if(order == InOrder) fn(root->key, clientData);
		RecMapRBT(fn, root->right, order, clientData);
		if(order == PostOrder) fn(root->key, clientData);
	}
}

void display(int np, void *clientData)
{
  printf("%d\n", np);
}

void write(void *np, void *fp)
{
  int value;
 
  value = *(int*)np;
  fprintf(fp, "%-4d", value);
}


main()
{
  FILE *fp;
  int i=0,temp;
  int num[23] = {145, 51, 89, 4, 148, 23, 136, 126, 154, 33, 212, 221, 123, 
		 68, 47, 79, 53, 121, 25, 174, 184, 32, 232};
  treeT *root;

  //  root = New(treeT);
  root = NULL;
  while(i<23){
    temp = num[i++];
    InsertNode(root, temp);
  }
  fp = fopen("/savefile/abstract/chapter13/rb1", "w");
  if(fp == NULL) Error("Open file faile");
  MapBST(write, root, PreOrder, fp);
}
