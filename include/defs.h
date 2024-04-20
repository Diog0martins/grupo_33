/* 
#define SERVER "fifo_server"
#define CLIENT "fifo_client" 
*/

typedef struct command{
    int pid;
    int time;
    char cmd[10];
    char flag[3];
    char args[300];
} Cmd;
