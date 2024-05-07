#include "../include/defs.h"

int main (int argc, char* argv[]){

    int pid = getpid();
    char pid_name[10];
    sprintf(pid_name, "%d", pid);

    if ((strcmp(argv[1], "status"))==0){
        
        mkfifo(pid_name, 0600);
        char output[400];

        int fd = open("server", O_WRONLY);
        Cmd status;
        status.tipo = 0;
        status.pid = pid;
        strcpy(status.cmd, "status");
        write(fd, &status, sizeof(Cmd));
        
        close(fd);

        fd = open(pid_name, O_RDONLY);

        int n;
        while ((n=(read(fd, &output, 300)))>0){
            write(1, output, n);
        }
        
        close(fd);

    }else{

        int fd = open("server", O_WRONLY);
        
        mkfifo(pid_name, 0600);

        Cmd message;
        message.tipo = 0;
        message.pid = pid;
        message.time = atoi(argv[2]);
        
        strcpy(message.cmd, argv[1]);
        
        strncpy(message.flag, argv[3], sizeof(message.flag));
        
        strcpy(message.args, argv[4]);

        write(fd, &message, sizeof(Cmd));

        close(fd);

        int tag;
        int answer_fd = open(pid_name, O_RDONLY);
        read(answer_fd, &tag, sizeof(int));

        char tag_name[30];
        sprintf(tag_name, "TASK no.%d received\n", tag);
        write(0, &tag_name, sizeof(tag_name));

        close(fd);
        close(answer_fd);
    }

    return 0;
}