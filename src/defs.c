#include "../include/defs.h"

// Função para inicializar a pilha
void initStack(Stack* stack) {
    stack->top = NULL;
}

// Função para verificar se a pilha está vazia
int isEmpty(Stack* stack) {
    return (stack->top == NULL);
}

// Função para criar um novo nó da pilha
StackNode* createNode(int id, Cmd command) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para novo nó da pilha.\n");
        exit(EXIT_FAILURE);
    }
    newNode->id = id;
    newNode->command = command;
    newNode->next = NULL;
    return newNode;
}

// Função para empilhar um novo comando na pilha
void push(Stack* stack, int id, Cmd command) {
    StackNode* newNode = createNode(id, command);
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

int peekId(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Erro: A pilha está vazia.\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->id;
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

void printStack(Stack* stack){
    printf("+-----------------------+\n");
    printf("|      Stack Output     |\n");
    printf("+-----------------------+\n");

    StackNode* current = stack->top;
    while (current != NULL) {
        printf("|  ID: %-5d  |\n", current->id);
        printf("|  Tipo: %-3d |\n", current->command.tipo);
        printf("|  PID: %-4d |\n", current->command.pid);
        printf("|  Time: %-3d |\n", current->command.time);
        printf("|  Cmd: %-9s |\n", current->command.cmd);
        printf("|  Flag: %-3s |\n", current->command.flag);
        printf("|  Args: %-17s |\n", current->command.args);
        printf("+-----------------------+\n");
        current = current->next;
    }

    printf("Stack End\n");
}