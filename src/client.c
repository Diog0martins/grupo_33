#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/defs.h"

void printCmd2(const Cmd *cmd) {
    printf("PID: %d\n", cmd->pid);
    printf("Time: %d\n", cmd->time);
    printf("Command: %s\n", cmd->cmd);
    printf("Flag: %s\n", cmd->flag);
    printf("Arguments: %s\n", cmd->args);
}

int main (int argc, char* argv[]){

    printf("Número de argumentos: %d\n", argc);
    printf("Argumentos:\n");
    for (int i = 0; i < argc; ++i) {
        printf("argv[%d]: %s\n",i, argv[i]);
    }

	//Abrir o FIFO que envia pedidos
	int fd = open("server", O_WRONLY);
    

	if (fd == -1){
        perror("Failed to open servidor FIFO");
        return 1;
    }

	int pid = getpid();
	char pid_name[10];
    sprintf(pid_name, "%d", pid);
    
    mkfifo(pid_name, 0600);

	// Verificar se foi passado pelo menos um argumento
    if (argc < 2) {
        printf("Missing argument.\n");
        return 1; // Retornar 1 indica que houve um erro
    }

	Cmd message;
    message.pid = pid;
    strcpy(message.cmd, argv[1]);
    
    message.time = atoi(argv[2]);
    
    strncpy(message.flag, argv[3], sizeof(message.flag));
    
    strcpy(message.args, argv[4]);


    printCmd2(&message);

    // Send the serialized message to the server
    write(fd, &message, sizeof(Cmd));

    close(fd);

    int tag;
    int answer_fd = open(pid_name, O_RDONLY);
    int n = read(answer_fd, &tag, sizeof(int));

    char tag_name[10];
    sprintf(tag_name, "%d", tag);
    write(0, &tag_name, n);

    char newline = '\n';
    write(0, &newline, 1);

    return 0;
}