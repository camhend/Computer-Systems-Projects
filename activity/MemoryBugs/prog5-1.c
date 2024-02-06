#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  char *str = malloc(10);
  gets(str);
  printf("%s\n",str);

  return 0;
}