#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h> 
#include <unistd.h>
#include <string.h>

typedef struct command{
    int tipo; //0-cliente, 1-filho que acabou
    int pid;
    //struct timeval initial_time;
    int time;
    char cmd[10];
    char flag[3];
    char args[300];
} Cmd;


// Estrutura para um nó da pilha
typedef struct stackNode {
    int id;
    Cmd command;
    struct stackNode* next;
} StackNode;


typedef struct stack {
    StackNode* top;
} Stack;


typedef struct registo {
    int state;    
    int id; 
    char args[300];
}Registo;


void initStack(Stack* stack);

int isEmpty(Stack* stack);

StackNode* createNode(int id, Cmd command);

void push(Stack* stack, int id, Cmd command);

Cmd pop(Stack* stack);

int peekId(Stack* stack);

void destroyStack(Stack* stack);

void printCommand(Cmd command);

void printStack(Stack* stack);

#endif