#include<stdio.h>

long fib1=0,fib2=0;

long Fib1(long n);
long Fib2(long n);
long AdditiveSequence(long n, long t0, long t1);

main()
{
	long n;
	long count=50;
 
  printf("This program compares the preformance of two\n"
	  "algorithms to compite the Fibonacci sequence\n");
  printf("Number of calls:\n");
  printf(" N       Fib1       Fib2   \n"
	     " --      ----       ----  \n");
  for(n=0; n<=count; n++){
       fib1=0;
	   fib2=0;
      Fib1(n);
	  Fib2(n);
	  printf(" %d       %9d       %6d\n", n, fib1, fib2);
  }
	  

	return 0;
}
  
  long Fib1(long n)
  {   
	  fib1++;
	  if(n<2)
		  return n;
	 else 
		 return (Fib1(n-1)+Fib1(n-2));
  }


  long Fib2(long n)
  {
      fib2++;
	  return(AdditiveSequence(n,0,1));
  }

  long AdditiveSequence(long n, long t0, long t1)
  {   
	  fib2++;
	  if(n==0) return t0;
	  if(n==1) return t1;
	  return(AdditiveSequence(n-1, t1, t0+t1));
  }
