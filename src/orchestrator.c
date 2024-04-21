#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> // for strcpy
#include <errno.h> // for errno
#include <sys/wait.h>
#include "../include/defs.h"

void printCmd(const Cmd *cmd) {
    printf("PID: %d\n", cmd->pid);
    printf("Time: %d\n", cmd->time);
    printf("Command: %s\n", cmd->cmd);
    printf("Flag: %s\n", cmd->flag);
    printf("Arguments: %s\n", cmd->args);
}

void execute(char cmds[], int id){

        char* args[100];
        int i=0;
        while((args[i++] = strsep(&cmds, " \n\t")) != NULL)
                ;

        char output_name[10];
        sprintf(output_name, "tmp/%d", id); 

        int fd = open(output_name, O_CREAT|O_WRONLY, 0644);

        if (fd == -1) {
            perror("open");
            exit(1);
        }
        
        if (fork()==0){

            if (dup2(fd, 1) == -1){
                perror("dup2");
                exit(1);
            }

            execvp(args[0], args);
        }else{
            close(fd);
        }
   
}

void execute_chain (char cmds[] , int id) {

    char * commands [100];
    int num_commands = 0;

    while((commands[num_commands] = strsep(&cmds, "|")) != NULL) {
        num_commands++;
    }
            
    for (int m = 0; m < num_commands; m++){
        
        if (commands[m][0] == ' '){
            for (int n = 0; n < strlen(commands[m]); n++){
                commands[m][n] = commands[m][n+1];
            }
        }
    }

    int fd[2];
    int input_fd = 0;

    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands-1) {
            // Criar um pipe para o próximo comando
            if (pipe(fd) == -1) {
                perror("pipe");
                exit(1);
            }
        }

        // Fork para executar o comando
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) { // Processo filho
            if (input_fd != -1) {
                // Redirecionar a entrada para input_fd
                if (dup2(input_fd, 0) == -1) {
                    perror("dup2 (input)");
                    exit(1);
                }
                close(input_fd);
            }

            if (i < num_commands-1) {
                // Redirecionar a saída para fd[1]
                if (dup2(fd[1], 1) == -1) {
                    perror("dup2 (output)");
                    exit(1);
                }
                close(fd[1]);
                close(fd[0]);
            }

            // Dividir o comando atual em argumentos
            char *args[100];
            int j = 0;
            while ((args[j++] = strsep(&commands[i], " \n\t")) != NULL)
                ;

            for (int k = 0; args[k] != NULL; k++) {
                printf("Argument %d: %s\n", k, args[k]);
            }
            // Executar o comando
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }

        // Processo pai
        if (input_fd != -1) {
            close(input_fd);
        }

        if (i < num_commands - 1) {
            input_fd = fd[0]; // O próximo comando usará esse como entrada
            close(fd[1]); // O processo pai não precisa da parte de escrita do pipe
        }
    }

    // Esperar pelos processos filhos
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

}
  

int main (int argc, char * argv[]){
    
    mkfifo("server", 0600);
    int id = 0;

    while(1){

        int fd = open("server", O_RDONLY);
        if (fd == -1) {
            perror("Failed to open servidor FIFO");
            return 1;
        }

        printf("Abri o servidor!\n");

        Cmd buff;
        int n;

    //----LEITURA
        while ((n = read(fd, &buff, sizeof(Cmd))) > 0){
            
            if (n <= 0){
                perror("Failed to read from FIFO");
                close(fd);
                return 1;
            }
            
            printCmd(&buff);


        //-----Open client FIFO

            char client_name[10];
            sprintf(client_name, "%d", buff.pid);
            int client_fd = open(client_name, O_WRONLY);
        
            if(client_fd == -1){
                fprintf(stderr, "Failed to open client FIFO %s: %s\n", client_name, strerror(errno));
                continue; // Handle the error and continue loop
            }

            // Write to client FIFO
            if (write(client_fd, &id, sizeof(int)) == -1) {
                fprintf(stderr, "Failed to write to client FIFO %s: %s\n", client_name, strerror(errno));
                close(client_fd); // Close the client FIFO
                continue; // Handle the error and continue loop
            }

        //------|


        //-----Data Treatment Station-----

            if (strcmp(buff.cmd, "execute")==0){
                
                if (strcmp(buff.flag, "-u")==0){

                    execute(buff.args,id);

                }else if (strcmp(buff.flag, "-p")==0){

                    printf ("chegou ao execute chain\n");
                    execute_chain(buff.args, id);

                }else{

                    //Invalid Flag
                
                }

            }else if (strcmp(buff.cmd, "status")==0){
                /* code */
            }else{
                //Comando inválido
            }
            

            // Close the client FIFO
            //close(client_fd);
            id++;
        }


        // Close the FIFO
        close(fd);
    }

    return 0;
}

/* 
for (int i = 0; i < n; i++){
    
    int pfd[2]; // pfd[0] -> saída do pipe; pfd[1] -> entrada do pipe
    pipe(pfd);

    if (fork()==0){

        close(pfd[0]);

            write(pfd[1], &num[i], sizeof(int));
            printf("Enviei %d\n", num[i]);
        
        _exit(1);

    }else{
        
        close(pfd[1]);
        int num;
        printf("Estou à espera de receber\n");
        //sleep(5);
        while(read(pfd[0], &num, sizeof(int))>0){
            printf("Já li o número %d\n", num);
        }

        wait(NULL);
    }
}

Queres ter aberto 

cat fich1 |

filho executa
Executa e envia para uma pipe. A pipe vai tar com dup2 para 

grep "palavra" | 

pai vem buscar o outpur anterior.

cria um novo filho para executar 
Tem de ir à pipe buscar o output


wc -l 
*/


//--------PARSAMENTO DE STRINGS----------
/* 
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


    printf("Flag -u em: %d\n", i);
  
    // Construir o comando a partir dos argumentos após a flag "-u"
    char cmd[300] = "";
    for (int j = i + 1; j < argc; j++){
        printf("%s\n", argv[j]);
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
 */