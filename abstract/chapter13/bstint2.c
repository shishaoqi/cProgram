#include"genlib.h"
#include"cmpfn.h"
#include"bst.h"

void IntNodeInit(void *np, void *kp, void *clientData);
void display(void *np, void *clientData);
void write(void *np, void *clientData);
main()
{
  FILE *fp;
  int i=0,temp, second;
  int num[23] = {145, 51, 89, 4, 148, 23, 136, 126, 154, 33, 212, 221, 123, 
		 68, 47, 79, 53, 121, 25, 174, 184, 32, 232};
  bstADT bst;

  bst = NewBST(sizeof(int), IntCmpFn, IntNodeInit);
  while(i<23){
    temp = num[i++];
    InsertBSTNode(bst, &temp, NULL);
  }
  temp = 89;
  DeleteBSTNode(bst, &temp);  
  temp = 51;
  DeleteBSTNode(bst, &temp);  
  temp = 174;
  DeleteBSTNode(bst, &temp);  
  temp = 145;
  DeleteBSTNode(bst, &temp);

  temp = 167;
  InsertBSTNode(bst, &temp, NULL);
  temp = 142;
  InsertBSTNode(bst, &temp, NULL);
  temp = 150;
  InsertBSTNode(bst, &temp, NULL);
  temp = 93;
  InsertBSTNode(bst, &temp, NULL);

  temp = 136;
  DeleteBSTNode(bst, &temp);
  temp = 212;
  DeleteBSTNode(bst, &temp);

  temp = 27;
  InsertBSTNode(bst, &temp, NULL);
  temp = 29;
  InsertBSTNode(bst, &temp, NULL);
  
  /* second test */
  second = 47;
  DeleteBSTNode(bst, &second);
  second = 53;
  DeleteBSTNode(bst, &second);

  fp = fopen("/savefile/abstract/chapter13/int1", "w");
  if(fp == NULL) Error("Open file faile");
  MapBST(write, bst, PreOrder, fp);
}

void IntNodeInit(void *np, void *kp, void *clientData)
{
  *((int *)np) = *((int *)kp);
}

void display(void *np, void *clientData)
{
  int value;

  value = *(int *)np;
  printf("%d\n", value);
}
 
void write(void *np, void *fp)
{
  int value;
 
  value = *(int*)np;
  fprintf(fp, "%-4d", value);
}
