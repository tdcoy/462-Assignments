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
        printf("Something went wrong with malloc.");
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
                printf("Reallocating buffer failed.");
                exit(0);
            }
        }
    }
}

// Parses arguments passed
char **handleargs(char *args)
{
    int pos = 0, strsize = MAXCMD;                       // Position of arguments/size of string
    char **parsedstr = malloc(strsize * sizeof(char *)); // parsed string of arguments
    char *token = strtok(args, " ");

    // Error checking
    if (!parsedstr)
    {
        printf("Something went wrong parsedstr malloc.");
        exit(0);
    }

    // Parse args between whitespace into parsed string
    while (token != NULL)
    {
        // Build command string
        parsedstr[pos] = token;
        pos++;

        // Resize parsed command string
        if (pos >= strsize)
        {
            parsedstr = realloc(parsedstr, strsize * sizeof(char *));
            if (!parsedstr)
            {
                printf("Realloc for parsedstr failed.");
                exit(0);
            }
        }

        token = strtok(NULL, " ");
    }

    parsedstr[pos] = NULL;
    return parsedstr;
}

int handlecmds(char **cmds)
{
    int status;
    pid_t pid, wpid;

    pid = fork();

    // If this is not the child or parent process
    if (pid < 0)
    {
        printf("\n Failed to fork a child process");
        exit(0);
    }

    // If this is the child call exec(), then exit, something went wrong...
    else if (pid == 0)
    {
        // execvp(cmd[0], cmd) -> cmd is a char**
        if (execvp(cmds[0], cmds) == -1)
        {
            printf("\n execvp failed");
        }
        exit(0);
    }

    // If this is the parent, wait for child to finish wait() -> (rv = wait(&satus))
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main(int argc, char **argv)
{
    // string input in command line
    char *input, **cmds;
    char prompt[MAXINPUT];
    int flag;

    // Set the shell prompt input by the user
    if (argc > 1)
    {
        //No prompt
        if (strcmp(argv[1], "-") < 1)
        {
            strcpy(prompt, ": ");
        }
        //Users prompt
        else
        {
            char *result = malloc(strlen(argv[1]) + strlen(": ") + 1);
            strcpy(result, argv[1]);
            strcat(result, ": ");
            strcpy(prompt, result);
            free(result);
        }
    }
    else
    {
        strcpy(prompt, "jsh: ");
    }

    // Loop for input
    while (flag)
    {
        // Print shell prompt
        printf("\n %s", prompt);

        // Get user input
        input = getinput();
        // Handle any arguments
        cmds = handleargs(input);
        flag = handlecmds(cmds);

        // check for "exit" command
        if (!strcmp("exit", input))
        {
            break;
        }

        free(input);
        free(cmds);
    }

    return (0);
}