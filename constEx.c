#include <stdio.h>
int max(const int *p);

void main()
{
  int a[] = {1,2,3,4,5,6,7,8,9,10};
  int m, i;
  m = max(a);
  for(i=0;i<10;i++)
  {
      printf("%d\n", a[i]);
  }

}

int max(const int *p)
{
  int i, tmp = -10000;
  for(i=0;i<10;i++)
  {
      if(tmp < p[i])
        tmp = p[i];
  }
   return tmp;
}
