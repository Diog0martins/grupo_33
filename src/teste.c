#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> // for strcpy
#include <errno.h> // for errno
#include <sys/wait.h>
#include "../include/defs.h"

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

            if(buff.tipo)
            {
                /* code */
            }
            

        }
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

        if (buff.tipo=0){ //cliente
            
            


        }else if(buff.tipo=1){ //notificação de filho terminado
            
            if (/* FILA NÂO VAZIA PEGA E EXECUTA*/1)
            {
                /* code */
            }
            
        }
        

        }
        // Close the FIFO
        close(fd);
    }

    return 0;
}