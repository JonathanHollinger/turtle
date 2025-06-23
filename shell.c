#include "enums.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int shell() {
    //infinite loop
    while (1) {
        printf("$ ");


        char buf[MAX_LEN];
        fgets (buf, MAX_LEN, stdin);

        if (strncasecmp(buf, QUIT, MAX_LEN) == 0) {
            break;
        }


        //Start executing externals, needs FSM to fully implement.


        char **args;
        int pid = fork();
        if (pid == 0) {
            execvp(buf, args); //Replace null later, should be args.
            exit(1);
        }
        
        wait(NULL);


        
        
    }

    return EXIT_SUCCESS;
}