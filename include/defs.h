/* 
#define SERVER "fifo_server"
#define CLIENT "fifo_client" 
*/

typedef struct command{
    char* pid;
    int time;
    char** cmds;
} Cmd;
