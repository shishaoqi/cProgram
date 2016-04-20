/* 
* @Author: anchen
* @Date:   2016-04-20 04:49:35
* @Last Modified by:   anchen
* @Last Modified time: 2016-04-25 23:25:43
*/

#include <stdio.h>
#include"genlib.h"
#include"strlib.h"
#include"simpio.h"
#include"cmpfn.h"
#include"iterator2.h"
#include"set.h"

static void ListPermutations(string str);
static void RecursivePermute (string str, int k,setADT set);
static ExchangeCharacters(string str, int p1, int p2);

int main() {
    string s;
    printf("Input a string:\n");
    s = GetLine();
    printf("List the permutation:\n");
    ListPermutations(s);
    return 0;
}

static void ListPermutations(string str){
    setADT set;
    iteratorADT iterator;
    string s;

    set = NewPtrSet(StringCmpFn);
    RecursivePermute(str, 0, set);
    iterator = NewIterator(set);
    while(StepIterator(iterator, &s)){
        //printf("%s\n", s);
    }
    FreeIterator(iterator);
}

static void RecursivePermute(string str, int k, setADT set){
    int i;
    if(k == StringLength(str)){
        AddPtrElement(set, CopyString(str));
    }else{
        for(i=k; i<StringLength(str); i++){
            ExchangeCharacters(str, k, i);
            RecursivePermute(str, k+1, set);
            ExchangeCharacters(str, k, i);
        }
    }
}

static ExchangeCharacters(string str, int p1, int p2)
{
  char tmp;

  tmp = str[p1];
  str[p1] = str[p2];
  str[p2] = tmp;
}