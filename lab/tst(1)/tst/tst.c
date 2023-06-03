/* Exemplo básico de programa ("Ola' Mundo"). -- CRA
*/
#include <stdio.h>

int v1 = 10;
int v2 = 20;

void func1()
{
  int v1 = 11;
  
  printf("func1: v1=%d v2=%d\n", v1, v2);
  v2 = 21;
}
  
  
int main()
{

  printf("main: Ola' Mundo!\n");
  printf("main: v1=%d v2=%d\n", v1, v2);
  func1();
  printf("main: v1=%d v2=%d\n", v1, v2);

  return 0;
}
