// Name(s): Adam Young, Jaylin Rashaw Ealy, Ethan Flanagan
// Description: Skeleton code for a simple shell implementation in C.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT 1024 

struct ShellCommand {
    char* command;
    char* args[100];           
    int argCount;
    char* inputRedirect;
    char* outputRedirect;
    int appendOutput;
};

// --- Function Prototypes ---
void displayPrompt();
char* getInput();
struct ShellCommand parseInput(char* input);
void executeCommand(struct ShellCommand cmd);

// -- Main Function ---
int main() {
    char* input;
    struct ShellCommand command;
        
    // repeatedly prompt the user for input
    for (;;) {
        // display the prompt
        displayPrompt();

        // get the user's input
        input = getInput();
        
        // Safety check: if input is NULL (e.g. user hits Ctrl+D), exit
        if (input == NULL) break;

        // parse the command line
        command = parseInput(input);
        
        // execute the command
        executeCommand(command);
        
        // Note: You should technically free(input) here to avoid memory leaks!
        free(input); 
    }

    exit(0);
}

// --- Function Definitions ---

void displayPrompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
    } else {
        perror("getcwd error");
    }
}

char* getInput() {
    // Allocate a buffer to hold the user's input
    char* buffer = malloc(MAX_INPUT);
    if (buffer == NULL) return NULL;

    // Read a line of input from the user
    if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
        free(buffer);
        return NULL;
    }

    // Strip the newline
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

struct ShellCommand parseInput(char* input) {
    struct ShellCommand cmd;
    memset(&cmd, 0, sizeof(struct ShellCommand));

    int i = 0;
    char* token = strtok(input, " ");
    while (token != NULL) {
        cmd.args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    cmd.args[i] = NULL; 
    cmd.argCount = i;

    if (i > 0) {
        cmd.command = cmd.args[0]; 
    }
    return cmd;
}

void executeCommand(struct ShellCommand cmd) {
    
    // Check if a command was actually entered
    if (cmd.command == NULL) return;

    // Handle built-in commands like 'exit'
    if(strcmp(cmd.command, "exit") == 0) {
        exit(0);
    }

    // Handle built-in commands like 'cd'
    if(strcmp(cmd.command, "cd") == 0) {
        // if argument provided
        if (cmd.argCount > 1) {
            if (chdir(cmd.args[1]) != 0) {
                perror("You can't do that lol");
            }
            // if no argument provided, change to home directory
            
        }
        else {
                chdir(getenv("HOME"));
            }
        return;
    }

    // External Commands
    pid_t pid = fork();

    // In case of error
    if (pid < 0) {
        perror("YOU AERE A FAILURE");
        exit(1);
    }

    // Child process
    else if (pid == 0) {
        // execvp looks for another command
        execvp(cmd.command, cmd.args);


        // Just in case execvp fails
        printf("Error Command '%s' not found\n", cmd.command);
        exit(1);
    }

    // Parent process
    else {
        int status;

        //waits for the child process to finish
        waitpid(pid, &status, 0);
    }

}