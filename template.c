// Name(s): Adam Young, Jaylin Rashaw Ealy, Ethan Flanagan
// Description: Skeleton code for a simple shell implementation in C.

//All the includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT 1024 // Maximum input size

struct ShellCommand {
    char* command;          // The command itself
    char* args[100];           // Arguments for the command
    int argCount;          // Number of arguments
    char* inputRedirect;   // Input redirection file
    char* outputRedirect;  // Output redirection file
    int appendOutput;      // Flag for append mode (>>)
};

/* 
    A function that causes the prompt to 
    display in the terminal
*/

char* getInput() {
    // 1. buffer for user text
    char* buffer = malloc(MAX_INPUT);

    //2. Get the input
    if (buffer == NULL) {
        return NULL;
    }

    // 3. Strip the newline
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;
}

struct ShellCommand parseInput(char* input) {
    struct ShellCommand cmd;

    // Initialize structo to zero
    memset(&cmd, 0, sizeof(struct ShellCommand));

    int i = 0;

    // Start Tokenizing
    char* token = strtok(input, " ");
    while (token != NULL) {
        // Store the token in argument array
        cmd.args[i] = token;
        i++;

        // Get next token
        token = strtok(NULL, " ");
    }
    cmd.args[i] = NULL; // all args end NULL
    cmd.argCount = i;

    if (i > 0) {
        cmd.command = cmd.args[0]; 
    }
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
        cd is a toold provided by the shell, 
        so you WILL need to recreate the functionality of cd.
    4. Be sure to handle standard output redirect and standard input redirects here 
        That is, there symbols: > and <. 
        Pipe isn't required but could be a nice addition.
*/


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
	}

	exit(0);
}


void displayPrompt() {
    printf("$ ");
}

void executeCommand(struct ShellCommand cmd) {return 0;}
