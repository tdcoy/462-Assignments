#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXCHAR 1024
#define MAXCMD 12

int getinput(char *inputstr, char *prompt)
{
    char *buffer;

    buffer = readline(prompt);

    // If there is input
    if (strlen(buffer) != 0)
    {
        strcpy(inputstr, buffer);
        return 0;
    }
    // If there was no input
    else
    {
        return 1;
    }
}

// Splits up input commands
void parsewhitespace(char *str, char **parsedstr)
{
    // Loop through commands
    for (int i = 0; i < MAXCMD; i++)
    {
        // Split each string between the whitespaces
        parsedstr[i] = strsep(&str, " ");

        // If there are no more commands exit
        if (parsedstr[i] == NULL)
        {
            break;
        }

        // If the command is empty decrement
        if (strlen(parsedstr[i]) == 0)
        {
            i--;
        }
    }
}

// Fork a new process to handle each command
void handlecmds(char **parsedstr)
{
    if(strcmp(parsedstr[0], "exit") < 1){
        exit(0);
    }

    // Fork
    pid_t pid = fork();

    // If this is the child
    if (pid == -1)
    {
        printf("\n Something went wrong with fork.");
        return;
    }

    // If this is the parent
    else if (pid == 0)
    {
        if (execvp(parsedstr[0], parsedstr) < 0)
        {
            printf("\n Something went wrong with executing command.");
        }
        exit(0);
    }

    else
    {
        wait(NULL);
        return;
    }
}

// argc = num of arguments passed including program name
// argv = array of arguments
int main(int argc, char *argv[])
{
    char inputstr[MAXCHAR];
    char prompt[MAXCHAR];
    char * parsedcmds[MAXCMD];
    int flag = 0;

    // Set shell prompt
    if (argc > 1)
    {
        // Set prompt to be ": "
        if (strcmp(argv[1], "-") < 1)
        {
            strcpy(prompt, ": ");
        }

        // Set prompt to be user input
        else
        {
            char *result = malloc(strlen(argv[1]) + strlen(": ") + 1);
            strcpy(result, argv[1]);
            strcat(result, ": ");
            strcpy(prompt, result);
            free(result);
        }
    }

    // Set prompt to be "jsh: "
    else
    {
        strcpy(prompt, "jsh: ");
    }

    // Input loop
    while (1)
    {
        // Get input
        if (getinput(inputstr, prompt))
        {
            continue;
        }

        // Parse input
        parsewhitespace(inputstr, parsedcmds);

        handlecmds(parsedcmds);
    }

    return 0;
}