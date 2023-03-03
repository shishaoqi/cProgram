#include<stdio.h>

void MoveSingleDisk(char start, char finish);
void MoveTower(int n, char start, char finish, char temp);

int main()
{
  int n=0;
  char a,b,c;
  a='A';
  b='B';
  c='C';

  printf("Input the number of the tower:");
  while(scanf("%d",&n) != EOF) {
    MoveTower(n, a, b, c);
  }
  return 0;
}


void MoveSingleDisk(char start, char finish){
  printf("%c -> %c\n", start, finish);
	
}

void MoveTower(int n, char start, char finish, char temp){
  if(n==1)
    MoveSingleDisk(start, finish);
  else{
    MoveTower(n-1, start, temp, finish);
    MoveSingleDisk (start, finish);
    MoveTower(n-1, temp, finish, start);
  }
}

