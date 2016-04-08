#include<stdio.h>
#include"genlib.h"

typedef struct moveT{
  char start, finish, temp;
  int s, f, t;
}moveT;

void initMoveTower(moveT *);
void finishtask(moveT *);
void RecMove(moveT *);
void initia(moveT *tower, int numTower, char start, char finish, char temp);


int main()
{
  moveT *tower = New(moveT *);
  initMoveTower(tower);
  finishtask(tower);
	return 0;
}

void initMoveTower(moveT *tower)
{
  tower->start = 'A';
  tower->finish = 'B';
  tower->temp = 'C';

  printf("How many tower do you want to move:");
  scanf("%d", &tower->s);
  tower->f = 0;
  tower->t = 0;
}

void finishtask(moveT *tower)
{
  RecMove(tower);
}

void RecMove(moveT *tower)
{
  if(tower->s == 1){
    printf("%c -> %c\n", tower->start, tower->finish);
    FreeBlock(tower);
    return;
  }

 moveT *tower1 = New(moveT *);
 initia(tower1, tower->s-1, tower->start, tower->temp, tower->finish);
 RecMove(tower1);

 moveT *tower2 = New(moveT *);
 initia(tower2, 1, tower->start, tower->finish, tower->temp);
 RecMove(tower2);

 moveT *tower3 = New(moveT *);
 initia(tower3, tower->s-1, tower->temp, tower->finish, tower->start);
 RecMove(tower3);
}
 
void initia(moveT *tower, int numTower, char start, char finish, char temp)
{
  tower->s = numTower;
  tower->f = 0;
  tower->t = 0;

  tower->start = start;
  tower->finish = finish;
  tower->temp = temp;
}
