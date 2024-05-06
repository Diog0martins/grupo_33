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
    int time;
    char cmd[10];
    char flag[3];
    char args[300];
    int id;
    struct timeval reception;
} Cmd;

// Estrutura para um nó da pilha
typedef struct node {
    int id;
    Cmd command;
    struct node* next;
} Node;


typedef struct list{
    Node* top;
    Node* last;
} List;

typedef struct registo {
    int id; 
    char args[300];
    long double duration;
}Registo;

long double time_diff(struct timeval start, struct timeval end);

void initList(List* list);

int isEmpty(List* list);

Node* createNode(int id, Cmd command);

void insertInOrder(List* list, int id, Cmd command);

void insertAtEnd(List* list, int id, Cmd command);

Cmd removeFromBeginning(List* list);

int peekId(List* list);

void destroyList(List* list);

int addElement(Registo array[], int size, Registo new);

int removeElement(Registo array[], int size, int id);

int isArrEmpty(Registo array[], int size);

void initArray(Registo array[], int size);

void printCommand(Cmd command);

void printList(List* stack);

#endif