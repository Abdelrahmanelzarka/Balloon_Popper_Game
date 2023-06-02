
#include <stdio.h>

int myInt = 123;
char myString[10];

int main() {
  sprintf(myString, "%d", myInt);
  printf("myString = %s\n", myString);
  return 0;
}
