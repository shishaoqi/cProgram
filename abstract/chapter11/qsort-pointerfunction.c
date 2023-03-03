#include <stdio.h>
#include "genlib.h"
#include "cmpfn.h"

void quicksort(void *array, int n, int elementSize, cmpFnT fn, exchFnT fun);
int partition(void *array, int n, int elementSize, cmpFnT fn, exchFnT fun);


int main()
{
  // string array[100]={"dkd", "yes", "indes", "believe", "hash", "symbol",
  // "return","exchange"};
int array[100]={4024,823,237,83,3742,10,4213,9,21,676,1221,789,120,1000,9320,321, 234,2134,321,42,5756,223,556,32355,5432,24567,243,1245,462,1467,1225,2164,24325,141221};
  int i, count;
  count=34;

  for(i=0;i<count;i++)
    printf("%d  ",array[i]);
  printf("\n\n\n");

  quicksort(array, count, sizeof(string), IntCmpFn, IntExchFn);

  for(i=0;i<count;i++)
    printf("%-7d%c",array[i],i%6==5 ? '\n':' ');
  puts("");

  return 0;
}

void quicksort(void *array, int n, int elementSize, cmpFnT fn, exchFnT fun)
{
  int boundary;

  if(n<2) return;
  boundary=partition(array,  n, elementSize, fn, fun);
  quicksort((void*)((char*)array), boundary, elementSize, fn, fun);
  quicksort((void*)((char*)array+(boundary+1)*elementSize), n-boundary-1, elementSize, fn, fun);
}

int partition(void *array, int n, int elementSize, cmpFnT fn, exchFnT exchange)
{
  int lh, rh;
  void *pivot;

  pivot=array;
  lh=1;
  rh=n-1;
  while(TRUE){
    while(lh<rh && fn(((char*)array+rh*elementSize), pivot) >=0 )
      rh--;
    while(lh<rh && fn((char*)array+(lh*elementSize), pivot) <0 )
      lh++;

    if(lh==rh)  break;

    exchange( (void*)((char*)array+lh*elementSize), (void*)((char*)array+rh*elementSize) );
  }

  if( fn((void*)((char*)array+lh*elementSize), (void*)((char*)pivot))>=0 )
    return 0;
      /* if( *((char*)array+(lh*elementSize)) >= *((char*)pivot))  return 0; */
    exchange( (void*)((char*)array), (void*)((char*)array+lh*elementSize) );
  
  return lh;
}
