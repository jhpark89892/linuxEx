#include <stdio.h>

struct Test1{
  char a;
  char c;
  char e;
  char f;
  int b;
  int d;
  double z;
};

int main()
{
  struct Test1 test;
  printf("%p, %p,%p,%p,%p, %p, %p \n",&(test.a), &(test.c),&(test.e),&(test.f),&(test.b),(&test.d),(&test.z) );
}
