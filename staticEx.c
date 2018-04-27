#include <stdio.h>

void increaseNumber()
{
  static int num =0;
  printf("%d\n", num);
  num++;
}

int main (void)
{
  increaseNumber();
  increaseNumber();
  increaseNumber();
  increaseNumber();

  return 0;
}
