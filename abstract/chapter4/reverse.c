#include<genlib.h>
#include<simpio.h>
#include"strlib.h"

char* reverse(char* str);
char* Exchange(char* str , int l , int r);


int main()
{
	char s[100];
	string str;

	printf("Input a string:");
	str = GetLine();

	printf("%s\n",reverse(str));
	return 0;
}

char* reverse(char* str)
{
	return Exchange(str, 0, StringLength(str)-1);
}

char* Exchange(char* str , int l , int r)
{
	char temp;

	if(l==r || (l-1)==r)
		return str;
	else{
		temp=str[r];
		str[r]=str[l];
		str[l]=temp;
		return Exchange(str, l+1, r-1);
	}
}
