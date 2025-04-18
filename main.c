#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("Hello world!\n");
  if (argc == 1) {
    printf("One argument");
  } else {
    for (int i = 1; i < argc; i++) {
      printf("%s ", argv[i]);
    }
  }
    // Code to be executed
    return 0;
  }