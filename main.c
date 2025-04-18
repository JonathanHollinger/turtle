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

  // Code to be executed. I want to add flags later but that's for later.
  shell();

  return EXIT_SUCCESS;
}