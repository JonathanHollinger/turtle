#include "enums.h"
#include "effects.h"
#include "model.h"
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

        fsm_t *command = cmdline_init();



        //Start executing externals, needs FSM to fully implement.


        char *token = strtok (buf, " ");
      
        while (token != NULL)
          {
            event_t event = lookup (token);
            command->current_token = token;
            handle_event (command, event);

            token = strtok (NULL, " ");;
            if (strcmp(command->args[0], "quit") == 0) 
                {
                    printf("\n");
                    exit(EXIT_SUCCESS);
                }
          }
        

        // Parse the command


        execute(command);
        
        wait(NULL);


        
        
    }

    return EXIT_SUCCESS;
}

