#include"genlib.h"
#include"strlib.h"
#include"cmpfn.h"
#include"bst.h"

void IntNodeInit(void *np, void *kp, void *clientData);
void write(void *np, void *clientData);
main()
{
  FILE *fp;
  int i=0;
  StringBox temp, second;
  string num[10] = {"this", "record", "is", "allocated", "at", "the", "end", 
		    "of", "client", "structure"}; 
  bstADT bst;

  bst = NewBST(sizeof(StringBox), StringCmpFn2, IntNodeInit);
  while(i<10){
    temp->s = CopyString(num[i++]);
    InsertBSTNode(bst, temp, NULL);
  }

   fp = fopen("/savefile/abstract/string1", "w");
  if(fp == NULL) Error("Open file faile");
  MapBST(write, bst, InOrder, fp);
}

void IntNodeInit(void *np, void *kp, void *clientData)
{
  //StringBox node, knode;
  //node = (StringBox)np;
  //kp = (StringBox)kp;
  //node->s = CopyString(knode->s);
  (*(void**)np) = (*(void**)kp);
}
 
void write(void *np, void *fp)
{
  string str;
  StringBox temp;
  temp = (StringBox)np;
  fprintf(fp, "%s  ", temp->s);
}
