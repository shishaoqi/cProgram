#include<stdio.h>
#include<stdlib.h>
#include"genlib.h"
#include"strlib.h"
#include"iterator2.h"
#include"set.h"
 
static void RescursivePermute (string str, int k,int length, setADT set);
static ExchangCharacters(string str, int p1, int p2);
static void ListPermutations(string str);

int main()
{  
   string str;
   printf("Imput a line:");
   str =  GetLine();
   printf("\n");
   ListPermutations(str);
   return 0;
}

static void ListPermutations(string str){
  setADT set;
  iteratorADT iterator;
  StringBox box;
  int length;
    
  set = NewPtrSet(PtrSet, StringCmpFn2);
  length = (int)StringLength(str);
[[B  RescursivePermute(str, 0, length, set);
  printf("\n");

  iterator = NewIterator(set);
  while (StepIterator (iterator, box)){
    printf("%s\n", box->s);
  }
  FreeIterator(iterator);
}


static void RescursivePermute(string str, int k, int l, setADT set)
{
  int i, length;
  StringBox element;

	if(k == l){
	  printf("%s\n", str);
	  element->s = CopyString(str);
	  AddPtrElement (set, element);
	}else{
		for (i=k; i< l; i++){
			ExchangCharacters(str, k, i);
			RescursivePermute(str, k+1, l, set);
			ExchangCharacters(str, k, i);
		}
	}
}

static ExchangCharacters(string str, int p1, int p2)
{
	char tmp;

	tmp = str[p1];
	str[p1] = str[p2];
	str[p2] = tmp;
}

