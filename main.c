#include <stdio.h>
#include <shell.h>

int main(int argc, char *argv[]) {
  printf("Hello world!\n");

  if (argc == 1) {
    printf("One argument");
  } else {
    for (int i = 1; i < argc; i++) {
      printf("%s ", argv[i]);
    }
  }

  shell();
    // Code to be executed
    return EXIT_SUCCESS;
  }