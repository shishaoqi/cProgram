#include<stdio.h>

int GCD(int x, int y);

int main()
{
	int x,y;
        printf("Please input two integer:");
	scanf("%d %d",&x, &y);
	printf("%d\n", GCD(x,y));

	return 0;
}

int GCD(int x, int y)
{
	if(x%y==0)
		return y;
	else
		GCD(y,x%y);
}
