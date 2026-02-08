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

// --- FIXED: Added Prototypes so main() knows these functions exist ---
void displayPrompt();
char* getInput();
struct ShellCommand parseInput(char* input);
void executeCommand(struct ShellCommand cmd);

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
    // FIXED: Use getcwd to match the rubric requirement
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s$ ", cwd);
    } else {
        perror("getcwd error");
    }
}

char* getInput() {
    char* buffer = malloc(MAX_INPUT);
    if (buffer == NULL) return NULL;

    // FIXED: Actually read the input!
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
    // FIXED: Removed "return 0" because this is a void function
    
    // Check if a command was actually entered
    if (cmd.command == NULL) return;

    // Temporary print to prove it works
    printf("Debug: You typed command '%s'\n", cmd.command);
}