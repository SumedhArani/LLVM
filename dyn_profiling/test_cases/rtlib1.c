#include <stdio.h>

int a;
int b;

void count() 
{
	a++;
    printf("computed a: %d\n", a);
}

void newloop() 
{
	a = 0;
}

void newNestedLoop() 
{
	b = -1;
}

void countNested() 
{
	b++;
    printf("computed b: %d\n", b);
}