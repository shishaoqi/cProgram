#include"genlib.h"
#include"cmpfn.h"
#include"bst.h"

void IntNodeInit(void *np, void *kp, void *clientData);
void display(void *np, void *clientData);

main()
{
  int i=0,temp;
  int num[16] = {84, 69, 28 , 6, 105, 51, 89, 4, 148, 16, 46, 54, 30,
		 48, 47, 49};
  bstADT bst;

  bst = NewBST(sizeof(int), IntCmpFn, IntNodeInit);
  while(i<16){
    temp = num[i++];
    InsertBSTNode(bst, &temp, NULL);
  }
  /*
  temp = 30;
  DeleteBSTNode(bst, &temp);
  temp = 47;
  DeleteBSTNode(bst, &temp);
  temp = 49;
  DeleteBSTNode(bst, &temp);
  temp = 69;
  DeleteBSTNode(bst, &temp);
  temp = 53;
  InsertBSTNode(bst, &temp, NULL);
  temp = 52;
  InsertBSTNode(bst, &temp, NULL);
  */
  temp = 30;
  DeleteBSTNode(bst, &temp);
  temp = 47;
  DeleteBSTNode(bst, &temp);
  temp = 69;
  DeleteBSTNode(bst, &temp);
  temp = 46;
  DeleteBSTNode(bst, &temp);

  MapBST(display, bst, PreOrder, NULL);
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
