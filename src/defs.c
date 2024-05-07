#include "../include/defs.h"

//Função que calcula diferença entre dois momentos.
long double time_diff(struct timeval start, struct timeval end) {
    
    long double start_ms = (long double)(start.tv_sec * 1000LL + start.tv_usec / 1000.0);
    long double end_ms = (long double)(end.tv_sec * 1000LL + end.tv_usec / 1000.0);
    return end_ms - start_ms;
}

// Função para inicializar a lista
void initList(List* list) {
    list->top = NULL;
    list->last = NULL;
}

// Função para verificar se a lista está vazia
int isEmpty(List* list) {
    return (list->top == NULL);
}

// Função para criar um novo nó da lista
Node* createNode(int id, Cmd command) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para novo nó da lista.\n");
        exit(EXIT_FAILURE);
    }
    newNode->id = id;
    newNode->command = command;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node into the list in order of less time to more time
void insertInOrder(List* list, int id, Cmd command) {
    Node* newNode = createNode(id, command);
    
    // If the list is empty or the new element's time is less than the first element's time
    if (list->top == NULL || command.time < list->top->command.time) {
        newNode->next = list->top;
        list->top = newNode;
        if (list->last == NULL) { // Update last if it's the first element in the list
            list->last = newNode;
        }
        return;
    }

    // Traverse the list to find the correct position to insert the new element
    Node* current = list->top;
    while (current->next != NULL && command.time >= current->next->command.time) {
        current = current->next;
    }

    // Insert the new element
    newNode->next = current->next;
    current->next = newNode;
    if (newNode->next == NULL) { // Update last if the new element is inserted at the end
        list->last = newNode;
    }
}


// Função para inserir um novo comando no final da lista
void insertAtEnd(List* list, int id, Cmd command) {
    Node* newNode = createNode(id, command);
    if (isEmpty(list)) {
        list->top = newNode;
    } else {
        list->last->next = newNode;
    }
    list->last = newNode;
}

// Função para remover o primeiro comando da lista
Cmd removeFromBeginning(List* list) {
    if (isEmpty(list)) {
        printf("Erro: A lista está vazia.\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = list->top;
    Cmd removedCommand = temp->command;
    list->top = temp->next;
    if (list->top == NULL) {
        list->last = NULL;
    }
    free(temp);
    return removedCommand;
}

// Função para obter o ID do primeiro comando da lista
int peekId(List* list) {
    if (isEmpty(list)) {
        printf("Erro: A lista está vazia.\n");
        exit(EXIT_FAILURE);
    }
    return list->top->id;
}

// Função para liberar a memória ocupada pela lista
void destroyList(List* list) {
    while (!isEmpty(list)) {
        removeFromBeginning(list);
    }
}

// Function to add a new element to the array
int addElement(Registo array[], int size, Registo new) {
    int i;

    // Find the first empty element in the array or the end of the array
    for (i = 0; i < size && array[i].id != -1; i++);

    array[i] = new;

    return i;
}


int removeElement(Registo array[], int size, int id){
    int i;
    for(i = 0; array[i].id != id; i++){
        }
    

    // Clear the last element to indicate the end of the array
    array[i].id = -1; // Assuming -1 indicates an empty slot
    strcpy(array[i].args, "");
    array[i].duration = 0;

    return 0; // Success
}

int isArrEmpty(Registo array[], int size){
    for (int i = 0; i < size; i++) {
        if (array[i].id != -1) {
            return 0; // Not empty
        }
    }
    return 1; // Empty
}

void initArray(Registo array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i].id = -1;
        strcpy(array[i].args, "");
        array[i].duration = 0;    
    }
}



