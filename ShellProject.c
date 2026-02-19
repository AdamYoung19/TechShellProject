// Name(s): Ethan Flanagan, Adam Young, Jayline Ealy
// Description:
// A simple Unix-like shell called techshell.
// This shell repeatedly prompts the user for input, parses commands,
// supports built-in cd and exit, handles I/O redirection (< and >),
// and executes commands using fork() and execvp().

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_INPUT  255
#define MAX_ARGS 128
#define PATH_MAX 4096



struct ShellCommand 
{
    char *command;
    char *args[MAX_ARGS];
    int argCount;
    char *inputFile;
    char *outputFile;
};


//////// Some function ideas: ////////////
// Note: Some code is reflected in main that represents these functions,
// but it is up to you to determine how you want to organize your code.

/* 
    A function that causes the prompt to 
    display in the terminal
*/
void displayPrompt() 
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        printf("%s$ ", cwd);
    } else 
    {
        perror("getcwd");
    }

    fflush(stdout);
}


/*
    A function that takes input from the user.
    It may return return the input to the calling statement or 
    store it at some memory location using a pointer.
*/ 
char* getInput() 
{
    char *buffer = malloc(MAX_INPUT);
    if (!buffer) 
    {
        perror("malloc");
        exit(1);
    }

    if (fgets(buffer, MAX_INPUT, stdin) == NULL) 
    {
        free(buffer);
        exit(0);  // Exit on Ctrl+D
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline
    return buffer;
}


/*
    A function that parses through the user input.
    Consider having this function return a struct that stores vital
    information about the parsed instruction such as:
    - The command itself
    - The arguments that come after the command 
        Hint: When formatting your data, 
        look into execvp and how it takes in args.
    - Information about if a redirect was detected such as >, <, or |
    - Information about whether or not a new file 
        needs to be created and what that filename may be.
    

    Some helpful functions when doing this come from string.h and stdlib.h, such as
    strtok, strcmp, strcpy, calloc, malloc, realloc, free, and more

    Be sure to consider/test for situations when a backslash is used to escape the space char
    and when quotes are used to group together various tokens.
*/
struct ShellCommand parseInput(char *input) 
{

    struct ShellCommand cmd;
    cmd.argCount = 0;
    cmd.inputFile = NULL;
    cmd.outputFile = NULL;

    char *token = strtok(input, " ");

    while (token != NULL) 
    {

        if (strcmp(token, "<") == 0) 
        {
            token = strtok(NULL, " ");
            cmd.inputFile = token;
        }
        else if (strcmp(token, ">") == 0) 
        {
            token = strtok(NULL, " ");
            cmd.outputFile = token;
        }
        else 
        {
            cmd.args[cmd.argCount++] = token;
        }

        token = strtok(NULL, " ");
    }

    cmd.args[cmd.argCount] = NULL;

    if (cmd.argCount > 0)
        cmd.command = cmd.args[0];
    else
        cmd.command = NULL;

    return cmd;
}


/*
    A function that executes the command. 
    This function might take in a struct that represents the shell command.

    Be sure to consider each of the following:
    1. The execvp() function. 
        This can execute commands that already exist, that is, 
        you don't need to recreate the functionality of 
        the commands on your computer, just the shell.
        Keep in mind that execvp takes over the current process.
    2. The fork() function. 
        This can create a process for execvp to take over.
    3. cd is not a command like ls and mkdir. 
        cd is a tool provided by the shell, 
        so you WILL need to recreate the functionality of cd.
    4. Be sure to handle standard output redirect and standard input redirects here 
        That is, there symbols: > and <. 
        Pipe isn't required but could be a nice addition.
*/
void executeCommand(struct ShellCommand cmd)
{

    if (cmd.command == NULL)
    {
        return;
    }
        

    // Built-in exit
    if (strcmp(cmd.command, "exit") == 0) 
    {
        exit(0);
    }

    // Built-in cd
    if (strcmp(cmd.command, "cd") == 0)
    {

        if (cmd.argCount < 2)
        {
            char *home = getenv("HOME");
            if (chdir(home) != 0)
            {
                printf("Error %d (%s)\n", errno, strerror(errno));
            }
        } 
        else 
        {
            if (chdir(cmd.args[1]) != 0) 
            {
                printf("Error %d (%s)\n", errno, strerror(errno));
            }
        }
        return;
    }

    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork");
        return;
    }

    if (pid == 0) 
    {
        // Child process

        // Handle input redirection
        if (cmd.inputFile != NULL) 
        {
            int fd = open(cmd.inputFile, O_RDONLY);
            if (fd < 0) 
            {
                printf("Error %d (%s)\n", errno, strerror(errno));
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Handle output redirection
        if (cmd.outputFile != NULL) 
        {
            int fd = open(cmd.outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) 
            {
                printf("Error %d (%s)\n", errno, strerror(errno));
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd.command, cmd.args);

        // If exec fails
        printf("Error %d (%s)\n", errno, strerror(errno));
        exit(1);
    }
    else 
    {
        wait(NULL); // Parent waits for child
    }
}



int main() // MAIN
{
    char* input;
    struct ShellCommand command;
        
    // repeatedly prompt the user for input
    for (;;)
    {
        // display the prompt
        displayPrompt();

        // get the user's input
        input = getInput();
        
        // parse the command line
        command = parseInput(input);
        
        // execute the command
        executeCommand(command);

        free(input);
    }

    exit(0);
}
