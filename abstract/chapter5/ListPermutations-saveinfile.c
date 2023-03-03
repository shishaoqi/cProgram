#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "genlib.h"


FILE *pfile =NULL;
char *filename = "./listPermutation.txt";

static void RescursivePermute (string str, int k);
static ExchangCharacters(string str, int p1, int p2);
static void ListPermutations(string str);

int main() {  
    string s;
	string out;

	if(!(pfile = fopen(filename, "w"))) {
		printf("Error opening %s for writing. Program rerminated.", filename);
		exit(1);
	}

	s = (string) GetBlock(100);
	out = (string) malloc(100);
	gets(s);
	printf("\n");
	ListPermutations(s);
	fclose(pfile);

	if (!(pfile = fopen(filename, "r"))){
		printf("Error opening %s for writing. Program terminated.", filename);
		exit(1);
	}

	while (fgets(out,strlen(s)+1, pfile))	printf("%s", out);

	fclose(pfile);
	return 0;
}

static void ListPermutations(string str){
	RescursivePermute(str, 0);
}


static void RescursivePermute(string str, int k)
{
	int i;

	if(k == (int)strlen(str) ){
		fputs(str, pfile);
		fputs("\n",pfile);
	}else{
		for (i=k; i< (int)strlen(str); i++){
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
