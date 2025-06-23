#include "enums.h"
#include <stdio.h>

int shell() {
    printf("Starting shell\n");
    //infinite loop
    while (1) {
        printf("$ ");


        char buf[MAX_LEN];
        fgets (buf, MAX_LEN, stdin);
        printf("%s\n", buf);
        
    }

    return EXIT_SUCCESS;
}