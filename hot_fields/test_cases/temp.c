typedef struct node
{
  int data;
  struct node* left;
  struct node* right;
}tnode;

int m;
int n;

int add()
{
  return m+n;
}

int sub()
{
  return m-n;
}

int mul()
{
  return m*n;
}

int main()
{
  int res1;
  int res2;
  int res3;
  m=8;
  n=4;
  res1 = add();
  res2 = mul();
  res3 = sub();

  return 0;
}
