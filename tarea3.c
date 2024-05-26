#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"

typedef struct {
    int square[3][3];  // Matriz 3x3 que representa el tablero
    int x;  // Posición x del espacio vacío
    int y;  // Posición y del espacio vacío
    int nActions;
} State;

typedef struct Node {
    State state;
    struct Node* parent;
} Node;

// Funciones de utilidad
int isValidMove(int x, int y) {
    return (x >= 0 && x < 3 && y >= 0 && y < 3);
}

int isFinal(const State* estado) {
    State finalState = {
        {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}},
        0, 0
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] != finalState.square[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void copyState(State original, State* new) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            new->square[i][j] = original.square[i][j];
        }
    }
    new->x = original.x;
    new->y = original.y;
    new->nActions = original.nActions;
}

// Funciones de movimiento y transición
void move(State* current, int choice) {
    int x = current->x;
    int y = current->y;
    int temp;
    switch (choice) {
        case 1:
            if (isValidMove(x, y - 1)) {
                temp = current->square[x][y - 1];
                current->square[x][y - 1] = current->square[x][y];
                current->square[x][y] = temp;
                current->y = y - 1;
            }
            break;
        case 2:
            if (isValidMove(x, y + 1)) {
                temp = current->square[x][y + 1];
                current->square[x][y + 1] = current->square[x][y];
                current->square[x][y] = temp;
                current->y = y + 1;
            }
            break;
        case 3:
            if (isValidMove(x - 1, y)) {
                temp = current->square[x - 1][y];
                current->square[x - 1][y] = current->square[x][y];
                current->square[x][y] = temp;
                current->x = x - 1;
            }
            break;
        case 4:
            if (isValidMove(x + 1, y)) {
                temp = current->square[x + 1][y];
                current->square[x + 1][y] = current->square[x][y];
                current->square[x][y] = temp;
                current->x = x + 1;
            }
            break;
    }
}

State* transition(State* oldState, int action) {
    State *newState = (State*) malloc(sizeof(State));
    copyState(*oldState, newState);
    move(newState, action);
    newState->nActions++;
    return newState;
}

// Funciones de impresión y menú
void mostrarMenuPrincipal() {
    limpiarPantalla();
    puts("========================================\n");
    puts("     Escoge método de búsqueda\n");
    puts("========================================\n");
    puts("1) Búsqueda en Profundidad");
    puts("2) Buscar en Anchura");
    puts("3) Buscar Mejor Primero");
    puts("4) Salir\n");
}

void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x ");
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}

List *getAdjNodes(Node* parentNode) {
    List *adjList = list_create();
    int x = parentNode->state.x;
    int y = parentNode->state.y;
    for (int i = 1; i <= 4; i++) {
        State *newState = transition(&parentNode->state, i);
        if (newState->x != x || newState->y != y) {
            Node *newNode = (Node*) malloc(sizeof(Node));
            newNode->state = *newState;
            newNode->parent = parentNode;
            list_pushBack(adjList, newNode);
        } else {
            free(newState);
        }
    }
    return adjList;
}

// Algoritmos de búsqueda
void dfs(State initialState, int count) {
    Stack* stack = stack_create(stack);
    printf("\nOriginal State:\n\n");
    imprimirEstado(&initialState);
    printf("\n");
    Node *root = (Node *) malloc(sizeof(Node));
    root->parent = NULL;
    root->state = initialState;
    stack_push(stack, root);
    while (list_size(stack) > 0) {
        Node *currentNode = (Node *) stack_top(stack);
        stack_pop(stack);
        if (currentNode->state.nActions >= 15)
            continue;
        if (isFinal(&currentNode->state)) {
            printf("Puzzle solved!\n");
            printf("Iterations: %d\n\n", count);
            Node *node = currentNode;
            int steps = currentNode->state.nActions;
            while (node != NULL) {
                if (steps == 0) 
                    printf("Initial State:\n\n");  
                else
                    printf("Step %d:\n\n", steps);                  
                imprimirEstado(&node->state);
                printf("\n");
                node = node->parent;
                steps--;
            }
            stack_clean(stack);
            free(node);
            return;
        }
        List *adjNodes = getAdjNodes(currentNode);
        void *aux = list_first(adjNodes);
        while (aux != NULL) {
            stack_push(stack, aux);
            aux = list_next(adjNodes);
        }
        list_clean(adjNodes);
        count++;
    }
    printf("The solution was not found within the limited range.\n");
    stack_clean(stack);
}

void bfs(State initialState, int count) {
    Queue* queue = queue_create(queue);
    printf("Initial State:\n\n");
    imprimirEstado(&initialState);
    printf("\n");
    Node *root = (Node *) malloc(sizeof(Node));
    root->parent = NULL;
    root->state = initialState;
    queue_insert(queue, root);
    while (list_size(queue) > 0) {
        Node *currentNode = (Node *) queue_front(queue);
        queue_remove(queue);
        if (count >= 15000000) {
            printf("Process terminated: Iteration limit reached (15.000.000)\n");
            return;
        }
        if (isFinal(&currentNode->state)) {
            printf("Puzzle solved!\n");
            printf("Iterations: %d\n\n", count);
            Node *node = currentNode;
            int steps = currentNode->state.nActions;
            while (node != NULL) {
                if (steps == 0) 
                    printf("Initial State:\n\n");  
                else
                    printf("Step %d:\n\n", steps);                  
                imprimirEstado(&node->state);
                printf("\n");
                node = node->parent;
                steps--;
            }
            queue_clean(queue);
            free(node);
            return;
        }
        List *adjNodes = getAdjNodes(currentNode);
        void *aux = list_first(adjNodes);
        while (aux != NULL) {
            queue_insert(queue, aux);
            aux = list_next(adjNodes);
        }
        list_clean(adjNodes);
        count++;
    }
    printf("La solucion no fue encontrada.\n");
    queue_clean(queue);
}

int main() {
    State estado_inicial = {
        {{0, 2, 8}, {1, 3, 4}, {6, 5, 7}},
        0, 0
    };
    estado_inicial.nActions = 0;
    int opcion;
    int cont = 0;
    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf(" %d", &opcion);
        switch (opcion) {
            case 1:
                dfs(estado_inicial, cont);
                break;
            case 2:
                bfs(estado_inicial, cont);
                break;
            case 3:
                //best_first(estado_inicial);
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();
    } while (opcion != 4);
    return 0;
}