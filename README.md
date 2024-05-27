# Codigo buscar distintas soluciones 8-puzzle

<br>
<br>

Para ejecutar el codigo hay que usar estos comandos:
````console
gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
````

Y luego ejecutar:
````console
./tarea3
````

<br>

<hr>

El codigo te abre un menu donde tu eliges un metodo de busqueda para resolver el puzzle.

````bash
  1) Búsqueda en Profundidad
  2) Buscar en Anchura
  3) Buscar Mejor Primero
  4) Salir
````

Se crea un struct al principio donde se almacena las coordenadas del espacio vacio, el estado actual de puzzle y el numero de movimientos realizados.

````c
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
````

<br>

### Prototipos de funciones principales


`int isValidMove(int x, int y);` Validar si se puede realizar un movimiento o no.

`int isFinal(const State* estado);` Comprueba si el estado es el estado que se requiere.

`void copyState(State original, State* new);` Crea una copia del estado original para poder hacerle cambios.

`void move(State* current, int choice);` Establece que movimientos se pueden realizar. | 1 = arriba, 2 = abajo, 3 = izquierda, 4 = derecha |.

`State* transition(State* oldState, int action);` Funcion que crea un nuevo estado y llama a la funcion move. (En pocas palabras combina las funciones para poder llamar al move). 

`void mostrarMenuPrincipal();` Muestra un menu seleccionable.

`void imprimirEstado(const State *estado);` Imprime el estado deseado.

`List *getAdjNodes(Node* parentNode);` Obtener los nodos adyacentes al espacio vacio.

`void dfs(State initialState, int count);` Crea una pila y una raiz, a partir de esa raiz busca la opcion para que se complete el puzzle usando busqueda de profundidad.

`void bfs(State initialState, int count);` Crea una cola donde guarda los estados que no han sido testeados y las revisa en orden hasta conseguir el estado deseado usando busqueda de anchura.

<br>

<hr>

### Observaciones
- Caso 3 no funciona ya que se hizo solitario.
- Para cambiar el estado inicial se cambia en el main.
- Cuando el programa supera las 15.000.000 iteraciones se detiene automaticamente.
  
<br>

<br>

###### 💻 | Development By Sarwak
