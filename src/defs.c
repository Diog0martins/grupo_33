#include "./include/defs.h"


// Função para inicializar a pilha
void initStack(Stack* stack) {
    stack->top = NULL;
}

// Função para verificar se a pilha está vazia
int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

// Função para criar um novo nó da pilha
StackNode* createNode(Cmd command) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para novo nó da pilha.\n");
        exit(EXIT_FAILURE);
    }
    newNode->command = command;
    newNode->next = NULL;
    return newNode;
}

// Função para empilhar um novo comando na pilha
void push(Stack* stack, Cmd command) {
    StackNode* newNode = createNode(command);
    newNode->next = stack->top;
    stack->top = newNode;
}

// Função para desempilhar um comando da pilha
Cmd pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Erro: A pilha está vazia.\n");
        exit(EXIT_FAILURE);
    }
    StackNode* temp = stack->top;
    Cmd poppedCommand = temp->command;
    stack->top = temp->next;
    free(temp);
    return poppedCommand;
}

// Função para liberar a memória ocupada pela pilha
void destroyStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}

// Função de exemplo para imprimir um comando
void printCommand(Cmd command) {
    printf("Tipo: %d, PID: %d, Time: %d\n", command.tipo, command.pid, command.time);
    printf("Cmd: %s, Flag: %s, Args: %s\n", command.cmd, command.flag, command.args);
}