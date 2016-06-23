#include <stdio.h>

typedef struct stu
{
  int usn;
  int gpa;
  char* name;
  int marks[5];
}student;

int findgpa(student* rec);

int main()
{
  student s1[10];
  for(int i=0; i<10; i++)
    {
      scanf("%d" , &s1[i].usn);
      scanf("%s" , s1[i].name);
      for(int j =0; i<5; ++i)
	scanf("%d", &s1[i].marks[j]);
      s1[i].gpa = findgpa(&s1[i]);
    }  
  return 0;
}

int findgpa(student* rec)
{
  int sum =0;
  for(int i=0; i<5; ++i)
    {
      sum+=rec->marks[i];
    }
  return sum/5;
}


