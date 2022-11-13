#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXINPUT 1000
#define MAXCMD 100

/***********************************************************************
This is a shell that will involve calling six key system call wrappers: fork(), execvp(), pipe(), dup2(), open(), and close().
This shell will support pipes ('|'), file redirection ('<', '>', '>>'), and a request to run a program in the background ('&').
*/

/***********************************************************************
Part 1:
In a loop, present a prompt to the user and accept a line of input from them (terminated by a newline).
This loop should continue until the line of user input is the string "exit" or there is an error in getting the next line.
(The latter case would typically occur when end-of-file (EOF) is reached.)

After each line of input other than "exit", for now just print each whitespace-separated word of input on a line by itself.
*/

// Gets the users input
char *getinput(void)
{
    // Create buffer
    char *buffer = malloc(sizeof(char) * MAXINPUT);
    // Current Character/Buffer position/Buffer Size
    int c, pos = 0, buffersize = MAXINPUT;

    // Error checking
    if (!buffer)
    {
        fprintf("Something went wrong with malloc.");
        exit(0);
    }

    // Loop reading std input
    while (1)
    {
        // Read each char
        c = getchar();

        // Check for the end-of-file integer
        if (c == '\n' || c == EOF)
        {
            // Set null terminator
            buffer[pos] = '\0';
            return buffer;
        }
        else
        {
            // put current char in the string
            buffer[pos] = c;
        }
        pos++;

        // Resize buffer
        if (pos >= buffersize)
        {
            buffersize += MAXINPUT;
            buffer = realloc(buffer, buffersize);
            if (!buffer)
            {
                fprintf("Reallocating buffer failed.");
                exit(0);
            }
        }
    }
}

// Parses arguments passed
handleargs(char *args)
{
    int pos = 0, strsize = MAXCMD;             // Position of arguments/size of string
    char **parsedstr = malloc(sizeof(char *)); // parsed string of arguments

    // Error checking
    if (!parsedstr)
    {
        fprintf("Something went wrong parsedstr malloc.");
        exit(0);
    }

    char* token = strtok(args, " ");

    // Parse args between whitespace into parsed string
    while (token != NULL)
    {
        // Build command string
        parsedstr[pos] = token;
        pos++;

        // Resize parsed command string
        if (pos >= maxsize)
        {
            parsedstr = realloc(parsedstr, sizeof(char *) * strsize);
            if (!parsedstr)
            {
                fprintf("Realloc for parsedstr failed.");
                exit(0);
            }
        }

        token = strtok(NULL, " ");
    }

    token[pos] = NULL;

    //Handle Commands from parsed string
    pid_t pid = fork();

    // If this is not the child or parent process
    if (pid == -1)
    {
        printf("\n Failed to fork a child process");
        return;
    }

    // If this is the child call exec(), then exit, something went wrong...
    else if (pid == 0)
    {
        // execvp(cmd[0], cmd) -> cmd is a char**
        if (execvp(parsedstr[0], parsedstr) < 0)
        {
            printf("\n execvp failed");
        }
        exit(0);
    }

    // If this is the parent, wait for child to finish wait() -> (rv = wait(&satus))
    else
    {
        wait(NULL);
        return;
    }
}

int main(int argc, char **argv)
{
    // string input in command line
    char inputStr, **args;
    char prompt;
    int flag;

    // Set the shell prompt input by the user
    if (argc > 1)
    {
        if (strcmp(argv[1], "-"))
        {
            // hyphen
            strcpy(prompt, "");
        }
        else
        {
            strcpy(prompt, argv[1]);
        }
    }
    else
    {
        strcpy(prompt, "jsh: ");
    }

    // Loop for input
    while (1)
    {
        // Print shell prompt
        print("\n %s", prompt);

        // Get user input
        inputStr = getinput();
        // Handle any arguments
        handleargs(inputStr);

        // check for "exit" command
        if (!strcmp("exit", inputStr))
        {
            break;
        }
    }

    return (0);
}