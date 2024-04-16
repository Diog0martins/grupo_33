#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/defs.h"


int main (int argc, char* argv[]){

	//Abrir o FIFO que envia pedidos
	int fd = open("server", O_WRONLY);

	if (fd == -1){
        perror("Failed to open servidor FIFO");
        return 1;
    }

	int pid = getpid();
	char pid_name[10];
    sprintf(pid_name, "%d", pid);

	// Verificar se foi passado pelo menos um argumento
    if (argc < 2) {
        printf("Missing argument.\n");
        return 1; // Retornar 1 indica que houve um erro
    }

    // Procurar pela flag "-u"
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) {
            break; // Encontramos a flag "-u", sair do loop
        }
    }

    // Se não encontramos a flag "-u"
    if (i == argc) {
        printf("Flag '-u' not found.\n");
        return 1; // Retornar 1 indica que houve um erro
    }

    // Verificar se há argumentos após a flag "-u"
    if (i + 1 >= argc) {
        printf("No command specified after '-u'.\n");
        return 1; // Retornar 1 indica que houve um erro
    }

    // Construir o comando a partir dos argumentos após a flag "-u"
    char cmd[300] = "";
    for (int j = i + 1; j < argc; j++) {
        strcat(cmd, argv[j]);
        strcat(cmd, " ");
    }

    // Remover o último espaço
    cmd[strlen(cmd) - 1] = '\0';

    // Chamar a função parser com o comando construído
    char** args = parser(cmd);

    // Printar os argumentos parseados
    for (int k = 0; args[k] != NULL; k++) {
        printf("Argument %d: %s\n", k, args[k]);
    }

	// Criar mensagem a ser enviada
	Cmd* message = malloc(sizeof(Cmd));
	message->cmds = args;
	message->pid = pid_name;
	message->time = atoi(argv[1]);

	//Enviar a mensagem para o servidor
	write(fd, message, sizeof(Cmd));

    // Libertar a memória alocada para os argumentos
    free(args);

    return 0;
}


/* 	
	//-----CRIAR PIPE DE RECEÇÃO-----//

	char pipe_name[10];
    sprintf(pipe_name, "%d", pid);
	mkfifo(pipe_name, 0600);

	write(fd, MSG, sizeof(MSG));

	char pid_n[10]; 
	sprintf(pid_n, "%d", pid);
	

	int fd_2 = open(pid_n, O_RDONLY);
	
	/* 
	Msg* buff = malloc(sizeof(Msg)); 
	
	read(fd_2, buff, sizeof(Msg));

 */