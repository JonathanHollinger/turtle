#include "enums.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char* remove_newline(char *buf) {

    char ret[strlen(buf)];
    strncpy(ret, buf, strlen(buf));
    ret[strlen(buf)] = '\0';
    return ret;
}

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

        //Might be failing, might not be finding ls on the path. Uncertain
        if (pid == 0) {
            execvp(buf, args); //Replace null later, should be args.
            exit(1);
        }
        
        wait(NULL);


        
        
    }

    return EXIT_SUCCESS;
}

