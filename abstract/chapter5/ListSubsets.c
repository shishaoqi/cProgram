#include<stdio.h>
#include"genlib.h"
#include"strlib.h"
#include"simpio.h"

void ListSubsets (string str);
void creatsub(string str, int k);
string SubString(string s, int p1, int p2);

int main()
{
  string str;
  int length;

  str = GetLine();
  length = StringLength(str);
  creatsub(str, length);

  return 0;
}

void creatsub(string str, int k)
{   
  int i=0,j=0;
  int delet;

  delet=k;


  for(i=k; i>0 && k>2; i--){
    printf("{");
    for(j=0; j<=k; j++)
    if(j!=delet)  printf("%c",str[j]);
    printf("}\n");
    delet--;
  }

    if(k==2)  printf("{%s}\n", str);

    printf("{%c}\n\n", str[0]);
    str = SubString(str, 1, StringLength(str));
    if(*str)  creatsub(str,  StringLength(str));
}


