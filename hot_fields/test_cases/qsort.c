#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student
{
	char usn[10];
	char name[10];
	float gpa;
}stu;

void display(stu* record, int n);
void quick_sort(stu* record, int start, int stop);
int partition(stu* record, int start, int stop);
int compare(stu* a, stu* b);

int main()
{
	int n;
	scanf("%d", &n);
	stu* record = (stu*) malloc(n*sizeof(stu));

	for (int i = 0; i < n; ++i)
	{
		scanf("%s", (record+i)->usn);
		scanf("%s", (record+i)->name);
		scanf("%f", &(record+i)->gpa);
	}
	//display(record, n);
	quick_sort(record, 0, n-1);
	display(record, n);
	free(record);
}

void quick_sort(stu* record, int start, int stop)
{
	int middle;
	if (start<stop)
	{
		middle = partition(record, start, stop);
		quick_sort(record, start, middle-1);
		quick_sort(record, middle+1, stop);
	}
}

int partition(stu* record, int start, int stop)
{
	stu* pivot = record+stop;
	int i = start-1;
	for(int j=start; j<stop; j++)
	{
		if(compare(record+j,pivot)==1)
		{	
			i++;
			stu temp;
			temp = *(record+j);
			*(record+j) = *(record+i);
			*(record+i) = temp;
		}
	}
	stu temp;
	temp = *(record+stop);
	*(record+stop) = *(record+i+1);
	*(record+i+1) = temp;
	return i+1;
}

int compare(stu* a, stu* b)
{
	int v= strcmp(a->name, b->name);
	if(v<0)
		return 1;
	else
		return 0;
}

void display(stu* record, int n)
{
	//printing the records
	for (int i = 0; i < n; ++i)
	{
		printf("%s ", (record+i)->usn);
		printf("%s ", (record+i)->name);
		printf("%f\n", (record+i)->gpa);
	}
}
