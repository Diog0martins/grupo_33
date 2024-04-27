#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


typedef struct command{
    int tipo; //0-cliente, 1-filho que acabou
    int pid;
    struct timeval initial_time;
    int time;
    char cmd[10];
    char flag[3];
    char args[300];
} Cmd;

// Estrutura para um nó da pilha
typedef struct stackNode {
    Cmd command;
    struct stackNode* next;
} StackNode;

// Estrutura da pilha
typedef struct stack {
    StackNode* top;
} Stack;


typedef struct registo{
    int state;
    int id;
    char args[300];
}Registo;