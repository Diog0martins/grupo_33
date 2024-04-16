#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> // for strcpy
#include <errno.h> // for errno
#include "../include/defs.h"

int main (int argc, char * argv[]) {
    
    mkfifo("server", 0600);

    while(1) {
        int fd = open("server", O_RDONLY);
        if (fd == -1) {
            perror("Failed to open servidor FIFO");
            return 1;
        }

        printf("Abri o servidor!\n");

        Cmd *buff = malloc(sizeof(Cmd));
        if (buff == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }

        printf("I'll wait\n");
        int n;
        while ((n = read(fd, buff, sizeof(Cmd))) > 0) {
            printf("I will read\n");

            // Check if read was successful
            if (n != sizeof(Cmd)) {
                fprintf(stderr, "Incomplete read\n");
                continue; // Handle partial read
            }

            char* your_tag = strdup(buff->pid); // Duplicate pid string

            // Check if strdup succeeded
            if (your_tag == NULL) {
                perror("Failed to duplicate string");
                return 1;
            }

            // Open client FIFO
            
            /* int client_fd = open(your_tag, O_WRONLY);
            if (client_fd == -1) {
                fprintf(stderr, "Failed to open client FIFO %s: %s\n", your_tag, strerror(errno));
                free(your_tag); // Free memory allocated by strdup
                continue; // Handle the error and continue loop
            }

            // Write to client FIFO
            if (write(client_fd, your_tag, sizeof(int)) == -1) {
                fprintf(stderr, "Failed to write to client FIFO %s: %s\n", your_tag, strerror(errno));
                close(client_fd); // Close the client FIFO
                free(your_tag); // Free memory allocated by strdup
                continue; // Handle the error and continue loop
            }
 */
            // Data treatment Station
            printf("Li a Mensagem: %s, %d, %s %s\n", buff->pid, buff->time, buff->cmds[0], buff->cmds[1]);

            // Free memory allocated by strdup
            free(your_tag);

            // Close the client FIFO
            //close(client_fd);
        }

        // Free memory allocated by malloc
        free(buff);

        // Close the FIFO
        close(fd);
    }

    return 0;
}

