#include "../include/defs.h"

//strace -ff ./exec

void execute(char output_folder[], char cmds[], int id){

        char* args[100];
        int i=0;
        while((args[i++] = strsep(&cmds, " \n\t")) != NULL)
                ;

        char output_name[10];
        sprintf(output_name, "%s/%d",output_folder, id); 

        int fd = open(output_name, O_CREAT|O_WRONLY, 0644);

        if (fd == -1) {
            perror("open");
            exit(1);
        }
        
        if (fork()==0){
            dup2(fd, 2);
            if (dup2(fd, 1) == -1){
                perror("dup2");
                exit(1);
            }

            execvp(args[0], args);
        }else{
            close(fd);
        }  
}


void execute_chain(char output_folder[], char cmds[], int id){


    char output_name[20];
    sprintf(output_name, "%s/%d",output_folder, id);
    int fd = open(output_name, O_CREAT | O_WRONLY, 0644);

    char *commands[100];
    int num_commands = 0;

    // Parse commands
    while ((commands[num_commands] = strsep(&cmds, "|")) != NULL) {
        // Trim leading and trailing spaces from commands
        while (isspace((unsigned char)*commands[num_commands]))
            commands[num_commands]++;
        if (*commands[num_commands] == 0)
            continue;
        char *end = commands[num_commands] + strlen(commands[num_commands]) - 1;
        while (end > commands[num_commands] && isspace((unsigned char)*end))
            end--;
        end[1] = '\0';
        num_commands++;
    }

    int pfd[2];
    int prev_fd = -1;
    dup2(fd, 2);

    for (int i = 0; i < num_commands; i++){
        pipe(pfd);

        if (fork() == 0) {
            
            if (i != 0){
                dup2(prev_fd, 0);
                close(prev_fd);
            }

            close(pfd[0]);
            if(i != num_commands - 1){
                dup2(pfd[1], 1);
            } else {
                dup2(fd, 1);
                close(pfd[1]);
            }

            char *args[100];
            int j = 0;
            char *s = commands[i];
            while ((args[j++] = strsep(&s, " \n\t")) != NULL);
            execvp(args[0], args);

            exit(1);
        
        } else {
            close(pfd[1]);
            if (prev_fd != -1) {
                close(prev_fd);
            }
            prev_fd = pfd[0];
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    close(fd);
    return;

}

int prepare_execute(char output_folder[], Cmd buff, int id){

    if (strcmp(buff.flag, "-u")==0){

        execute(output_folder, buff.args, id);

    }else if (strcmp(buff.flag, "-p")==0){
        
        execute_chain(output_folder, buff.args, id);

    }else{

        return 1;
    }

    int fd = open("server", O_WRONLY);

    Cmd message;
    message.tipo = 1;
    message.pid = id;
    strcpy(message.cmd, "");
    message.time = 0;
    strcpy(message.flag, "");
    strcpy(message.args, "");

    write(fd, &message, sizeof(Cmd));

    close(fd);

    return 0;

}


int treat_client(char output_folder[], Cmd buff, int id, int waiting){
    
    char client_name[10];
    sprintf(client_name, "%d", buff.pid);
    int client_fd = open(client_name, O_WRONLY);

    // Informar ao cliente o id da Task
    write(client_fd, &id, sizeof(int));
    //------|

    //----- Data Treatment -----
    if(waiting == 0){
        prepare_execute(output_folder, buff, id);
    }

    close(client_fd);
    return 0;
}


void status(int fd, int max_p, Registo executing[max_p], List waiting_list, int pid){

    char pid_name[10];
    sprintf(pid_name, "%d", pid);

    int sts = open(pid_name, O_WRONLY);
    int outputR = open("tmp/register", O_CREAT | O_RDONLY, 0644);

    //A executar
    if (isArrEmpty(executing, max_p)==0){
        write(sts, "Executing:\n", 11);
        for (int i = 0; i < max_p; i++){
            if (executing[i].id != -1){
                char toSend[400];
                sprintf(toSend, "%d %s\n", executing[i].id, executing[i].args);
                write(sts, toSend, strlen(toSend));
            } 
        }
    }else{
        write(sts, "Nothing Executing:\n", 20);
    }

    //Em espera
    if(!isEmpty(&waiting_list)){

        Node* aux = waiting_list.top;
        write(sts, "Scheduled:\n", 11);
        char toSend[400];
        while (aux != NULL){                        
            sprintf(toSend, "%d %s\n", aux->id, aux->command.args);
            write(sts, toSend, strlen(toSend));
            aux = aux->next;   
        }

    }else{
        write(sts, "Nothing Scheduled.\n", 20);
    }
    

    //Terminadas
    write(sts, "Completed:\n", 11);
    Registo buff;
    lseek(fd, 0, SEEK_SET);
    while ((read(outputR, &buff, sizeof(Registo))) > 0){
        if (buff.id != 0){
            char toSend[400];
            sprintf(toSend, "%d %s %Lf\n", buff.id, buff.args, buff.duration);
            write(sts, toSend, strlen(toSend));
        }
    }

    close(sts);
    close(outputR);

}

int main(int argc, char * argv[]){
    
    mkfifo("server", 0600);
    Registo lastRegisto;

    int outputR = open("tmp/register", O_CREAT | O_RDONLY, 0644);
    off_t fileSize = lseek(outputR, 0, SEEK_END); 
    
    int id;

    if (fileSize >= sizeof(Registo)){
        lseek(outputR, -sizeof(Registo), SEEK_END);
        read(outputR, &lastRegisto, sizeof(Registo));
        id = lastRegisto.id;
    }else{
        id = 0;
    } 
    close(outputR);
    
    int open_p = 0;
    int max_p = atoi(argv[2]);
    char politic[4]; //FCFS or SJF
    strcpy(politic, argv[3]);
    List waiting_list;
    initList(&waiting_list);
    
    Registo executing[max_p];
    initArray(executing, max_p);
    
    int output = open("tmp/register", O_CREAT | O_WRONLY, 0644);

    while(1){

        int fd = open("server", O_RDONLY);
        Cmd buff;
        int n;

    //----LEITURA
        while ((n = read(fd, &buff, sizeof(Cmd))) > 0){
                    
            struct timeval start, end; 
            gettimeofday(&start, NULL);
            buff.reception = start;

            if(buff.tipo == 0){ //cliente
                
                if (strcmp(buff.cmd, "status")==0){
                    
                    status(fd, max_p, executing, waiting_list, buff.pid);

                }else{

                    id++;
                    buff.id = id;
                    int w = 0;
                    
                    if (open_p == max_p){
                        w = 1;
                        if(strcmp(politic, "FCFS")==0)insertAtEnd(&waiting_list, id, buff);
                        if(strcmp(politic, "SJF")==0)insertInOrder(&waiting_list, id, buff);
                    
                    }else{
                        open_p++;
                    }

                    Registo task;
                    task.id = buff.id;
                    strcpy(task.args, buff.args);
                    task.duration = 0;
                    
                    if(w == 0) addElement(executing, max_p, task);

                    if (fork() == 0){

                        treat_client(argv[1], buff, id, w);
                        
                        if(w==0){
                            //Registar tempo
                            gettimeofday(&end, NULL);
                            long double diff = time_diff(buff.reception, end);                       
                            task.duration = diff;
 
                            lseek(output, (buff.id-1)*sizeof(Registo), SEEK_SET);
                            write(output, &task, sizeof(Registo));

                        }

                        exit(1);
                        
                    }
                }

            }else if(buff.tipo == 1){ //Aviso de filho terminado
                
                removeElement(executing, max_p, buff.pid);

                if (!isEmpty(&waiting_list)){
                    
                    int tobe_id = peekId(&waiting_list);
                                        
                    Cmd new_cmd = removeFromBeginning(&waiting_list);
                    
                    Registo finished;
                    finished.id = new_cmd.id;
                    strcpy(finished.args, new_cmd.args);
                    finished.duration = 0;
                        
                    addElement(executing, max_p, finished);

                    if (fork() == 0){

                        prepare_execute(argv[1], new_cmd, tobe_id); 

                        //Registar tempo
                        gettimeofday(&end, NULL);
                        long double diff = time_diff(new_cmd.reception, end);

                        finished.duration = diff;

                        lseek(output, (new_cmd.id-1)*sizeof(Registo), SEEK_SET);
                        write(output, &finished, sizeof(Registo));
                        
                        exit(1);                       
                    }
                    
                }else{

                    //A Stack está vazia
                    open_p--;

                }
                
            }

        close(fd);
        }            
    }

    destroyList(&waiting_list);
    return 0;
}