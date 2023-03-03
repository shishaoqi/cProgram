#include <stdio.h>

int input(int n);
int Cannonball(int n, int t1, int t2);

int main(void)
{
	int value=0;
	int i=0;
	int n=0;
	int count=4;
	printf("what count do you want to input:");
	scanf("%d", &count);
	for(i=0; i<=count; i++)
	{
		n=i;
		value=input(n);
		//	for(i=0;i<=value;i++)
		printf("\nyou input n=%d,value=%d\n",n,value);
	}


	return 0;
}

int input(int n)
{
	return (Cannonball(n,1,4));
}


int Cannonball(int n,int t1 , int t2)
{
	if(n==0) return (t1);
	if(n==1) return (t2);
	return  Cannonball(n-1, t2,t1+t2+(2*n));
}