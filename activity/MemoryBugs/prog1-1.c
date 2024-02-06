#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int i;
  int *a = malloc(sizeof(int) * 10);
  if (!a) return -1; 
  for (i = 0; i < 11; i++) { // 10 bytes allocated - a[10] accesses an 11th integer 
    a[i] = i;
  }
  free(a);
  return 0;
}
