#include<stdio.h>
void quicksort(int array[], int n);
int partition(int array[], int n );


int main()
{
	int array[1000]={45,34,53,14,21,87,9,136,543,419,876,456,732,42,79,6,543,805,891,703,201,176};
int i, count;
count=22;
//scanf("%d\n",&count);

/*for(i=0;i<count;i++)
{
	scanf("%d",&array[i]);
}*/
for(i=0;i<count;i++)
printf("%5d",array[i]);
printf("\n\n\n");

quicksort(array, count);

for(i=0;i<count;i++)
printf("%5d%c",array[i],i%5==4 ? '\n':' ');
puts("");

return 0;
}

void quicksort(int array[], int n)
{
	int boundary;

	if(n<2)
		return;
	boundary=partition(array,  n);
	quicksort(array, boundary);
	quicksort(array+boundary+1, n-boundary-1);
}

int partition(int array[], int n )
{
	int lh, rh, pivot, temp;
	pivot=array[0];
	lh=1;
	rh=n-1;
	while(true)
	{
		while(lh<rh && array[rh]>=pivot)  rh--;
		
		while(lh<rh && array[lh]<pivot)   lh++;

		if(lh==rh)  break;
		temp=array[lh];
		array[lh]=array[rh];
		array[rh]=temp;	
	}

	if(array[lh]>=pivot)
		return 0;
	array[0]=array[lh];
	array[lh]=pivot;
	return lh;
}

