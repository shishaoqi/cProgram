#include<stdio.h>

double RaiseInntToPower(double n, int  k);

int main()
{
  double input1=0,output=0;
  int input2=0;

  scanf("%lf %d", &input1,&input2);
  output=RaiseInntToPower(input1,input2);
  printf("%lf\n",output);

  return 0;
}

double RaiseInntToPower(double n, int  k)
{
  if(k==0)
    return 1;

  if(k<0){
    n=1/n;
    k=-k;
  }
  return(n*RaiseInntToPower(n, k-1));
}
