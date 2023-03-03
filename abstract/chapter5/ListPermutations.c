#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "simpio.h"

 
static void RescursivePermute (string str, int k);
static ExchangCharacters(string str, int p1, int p2);
static void ListPermutations(string str);

int main()
{  
  string s;
  printf("Input a string:\n");
  s = GetLine();
  printf("List the permutation:\n");
  ListPermutations(s);
  return 0;
}

static void ListPermutations(string str){
  RescursivePermute(str, 0);
}


static void RescursivePermute(string str, int k)
{
  int i;

  if(k == (int)StringLength(str) ){
    printf("%s\n", str);
  }else{
    for (i=k; i< (int)StringLength(str); i++){
      ExchangCharacters(str, k, i);
      RescursivePermute(str, k+1);
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
