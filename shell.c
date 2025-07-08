#include "enums.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int shell() {
    //infinite loop
    while (1) {
        printf("$ ");


        char buf[MAX_LEN];
        fgets (buf, MAX_LEN, stdin);
        
        //Use this code to remove newlines and the \r character (weird trailing character from mac).
        buf[strcspn(buf, "\r\n")] = 0;

        if (strncasecmp(buf, QUIT, MAX_LEN) == 0) {
            break;
        }



        //Start executing externals, needs FSM to fully implement.


        char **args;


        printf("%s\n", buf);
        int pid = fork();

        //Might be failing, might not be finding ls on the path. Uncertain
        if (pid == 0) {
            execvp(buf, args); //Replace null later, should be args.
            printf("Command not found\n");
            exit(1);
        }
        
        wait(NULL);


        
        
    }

    return EXIT_SUCCESS;
}

