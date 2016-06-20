int m=5;
int n=4;

int func1();

int func2();

int main()
{
  int res1;
  int res2;
  res1 = func1();
  res2 = func2();
  return 0;
}

int func1()
{
  int res;
  for(int i=0; i<m; i++)
  {
    for(int j=0; j<n; j++)
    {
      res += i+j;
    }
  }
  return res;
}

int func2()
{
  int res1;
  int res2;
  for(int i=0; i<m; i++)
  {
    for(int j=0; j<n; j++)
    {
      for(int k=i+j; k>0; k--)
      {
        res1 *= k;
      }
    }
  }
  for(int i=0; i<m; i++)
  {
    for(int j=0; j<n; j++)
    {
      res2 += i+j;
    }
  }
  return res1+res2;
}
