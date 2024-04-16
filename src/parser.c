#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** parser(char* cmd) {
    char* s = strdup(cmd); // Duplicate the input string to avoid modifying the original
    
    // Allocate memory dynamically for the array to store command and arguments
    char** command_args = malloc(100 * sizeof(char*));
    if (command_args == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Extract the command
    command_args[0] = strsep(&s, " \n\t");

    command_args[1] = strsep(&s, "\n");

    return command_args;
}