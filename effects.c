#define _POSIX_C_SOURCE 200809L // needed for strdup extension

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <spawn.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "effects.h"
#include "model.h"

void prepend(char* s, const char* prefix) {
    size_t len_prefix = strlen(prefix);
    size_t len_s = strlen(s);

    // Ensure enough space (consider reallocating if needed)
    // For this example, it's assumed that 's' has sufficient capacity

    memmove(s + len_prefix, s, len_s + 1); // Shift original string
    memcpy(s, prefix, len_prefix);         // Copy prefix
}

static const char *builtins[] = {
    "echo",
    "pwd",
    "cd",
    "export",
    "unset",
    "which"};

bool contains(const char *str)
{
    for (int i = 0; i < 6; i++)
    {
        if (strcmp(builtins[i], str) == 0)
        {
            return true;
        }
    }
    return false;
}

char *join_strings(char *arr[], int size)
{
    // Calculate the total length of the resulting string
    int total_length = 0;
    for (int i = 1; i < size; i++)
    {
        total_length += strlen(arr[i]) + 1; // +1 for the space or null terminator
    }

    // Allocate memory for the result string
    char *result = (char *)malloc(total_length * sizeof(char));
    if (result == NULL)
    {
        return NULL; // Memory allocation failed
    }

    // Initialize the result string
    result[0] = '\0'; // Start with an empty string

    // Concatenate each string in the array, adding a space between them
    for (int i = 1; i < size; i++)
    {
        strcat(result, arr[i]);
        if (i < size - 1)
        {
            strcat(result, " "); // Add space between words
        }
    }

    return result;
}

/* Executed when starting to process a new command line. The fsm_t
   should have been updated to include a pointer to the current token. */
void start_command(fsm_t *cmdmodel)
{
    cmdmodel->command = cmdmodel->current_token;
    cmdmodel->nargs = 1;
    cmdmodel->args = calloc(MAX_ARGUMENTS, sizeof(char *));
    cmdmodel->args[0] = cmdmodel->current_token;
}

/* Executed when processing a token after the command name. */
void append(fsm_t *cmdmodel)
{
    if (cmdmodel->nargs >= MAX_ARGUMENTS)
        return;

    assert(cmdmodel->args != NULL);
    cmdmodel->args[cmdmodel->nargs] = cmdmodel->current_token;
    cmdmodel->nargs++;
}

/* Executed when either a NL or | (pipe) is encountered. */
void execute(fsm_t *cmdmodel)
{
    assert(cmdmodel->args != NULL);

    char *cmd = cmdmodel->args[0];

    // Old code for version that used builtins that ran without exec
    // if (strcmp(cmd, "echo") == 0) {
    //     char *remains = join_strings(cmdmodel->args, cmdmodel->nargs);
    //     echo(remains);
    // }

    // if (strcmp(cmd, "export") == 0) {
    //     char *remains = join_strings(cmdmodel->args, cmdmodel->nargs);
    //     export(remains);
    // }

    // if (strcmp(cmd, "pwd") == 0) {
    //     if (cmdmodel->nargs == 1) {
    //         pwd();
    //     }
    // }

    // if (strcmp(cmd, "cd") == 0) {
    //     char *remains = join_strings(cmdmodel->args, cmdmodel->nargs);
    //     cd(remains);
    // }

    // if (strcmp(cmd, "which") == 0) {
    //     char *remains = join_strings(cmdmodel->args, cmdmodel->nargs);
    //     which(remains);
    // }

    // if (strcmp(cmd, "unset") == 0) {
    //     char *remains = join_strings(cmdmodel->args, cmdmodel->nargs);
    //     unset(remains);
    // }
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        free(cmdmodel->args);
        cmdmodel->args = NULL;
        cmdmodel->nargs = 0;
        cmdmodel->current_token = NULL;
        return;
    }

    // posix_spawnp requires a special struct to handle process creation
    pid_t pid;
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attr;

    // Initialize file actions and spawn attributes
    posix_spawn_file_actions_init(&actions);
    posix_spawnattr_init(&attr);

    // // Child process setup
    posix_spawn_file_actions_addclose(&actions, pipefd[0]);               // Close read end of the pipe in the child
    posix_spawn_file_actions_adddup2(&actions, STDOUT_FILENO, pipefd[1]); // Redirect stdout to pipe
    close(pipefd[1]);                                                     // Close the write end of the pipe in the parent, after adding it to actions

    for (size_t i = 0; i < cmdmodel->nargs; i++) {
        printf("%s\n", cmdmodel->args[i]);
    }


    if (posix_spawnp(&pid, cmd, &actions, &attr, cmdmodel->args, NULL) != 0)
    {
        printf("Command not found\n");
        close(pipefd[0]);
        free(cmdmodel->args);
        cmdmodel->args = NULL;
        cmdmodel->nargs = 0;
        cmdmodel->current_token = NULL;
        return;
    }

    // Parent process: Close the write end of the pipe
    close(pipefd[1]);

    // Wait for the child process to finish
    int child_status;
    waitpid(pid, &child_status, 0);

    // Close the read end of the pipe in the parent process
    close(pipefd[0]);

    // Clean up spawn attributes and actions
    posix_spawnattr_destroy(&attr);
    posix_spawn_file_actions_destroy(&actions);

    // Clean up the arguments array
    free(cmdmodel->args);
    cmdmodel->args = NULL;
    cmdmodel->nargs = 0;
    cmdmodel->current_token = NULL;
}

void link_commands(fsm_t *cmdmodel)
{
    printf("Set up pipe\n");
    execute(cmdmodel);
}

void error_pipe(fsm_t *cmdmodel)
{
    printf("ERROR: Received token %s while in state %s\n",
           cmdmodel->current_token, state_name(cmdmodel->state));
}

void error_newline(fsm_t *cmdmodel)
{
    printf("ERROR: Received token %s while in state %s\n",
           cmdmodel->current_token, state_name(cmdmodel->state));
}
