#include <stdlib.h>
#include <stdio.h>

int main() {
  int c;
  FILE * inFile = fopen("in.txt","r");
  while((c = getc(inFile)) != EOF) {
    putchar(c);
  }
  fclose(inFile);
  return 0;
}
