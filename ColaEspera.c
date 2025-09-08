#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura Auto
typedef struct { 
    char matricula[10];
    int hora_llegada;
    int hora_Salida;
} Auto;

#define MAX_COLA 35

// Definición de la estructura ColaEspera (cola circular)
typedef struct {
    Auto *autos;      // Array dinámico de autos
    int frente;       // Índice del frente de la cola
    int final;        // Índice del final de la cola
    int capacidad;    // Capacidad máxima de la cola
    int tamaño;       // Cantidad actual de elementos en la cola
} ColaEspera;

// Inicializa la cola con una capacidad dada
void inicializar_ColaEspera(ColaEspera *cola, int capacidad) {
    cola->autos = (Auto *)malloc(capacidad * sizeof(Auto));
    cola->frente = 0;
    cola->final = -1;
    cola->capacidad = capacidad;
    cola->tamaño = 0;
}

// Devuelve 1 si la cola está vacía, 0 en caso contrario
int esta_vacia(ColaEspera *cola) {
    return cola->tamaño == 0;
}

// Devuelve 1 si la cola está llena, 0 en caso contrario
int esta_llena(ColaEspera *cola) {
    return cola->tamaño == cola->capacidad;
}

// Agrega un auto al final de la cola si no está llena
void encolar(ColaEspera *cola, Auto a) {
    if (!esta_llena(cola)) {
        cola->final = (cola->final + 1) % cola->capacidad;
        cola->autos[cola->final] = a;
        cola->tamaño++;
    } else {
        printf("Cola llena, no se puede encolar.\n");
    }
}

// Elimina y devuelve el auto al frente de la cola si no está vacía
Auto desencolar(ColaEspera *cola) {
    Auto a = {"", -1, -1};
    if (!esta_vacia(cola)) {
        a = cola->autos[cola->frente];
        cola->frente = (cola->frente + 1) % cola->capacidad;
        cola->tamaño--;
    } else {
        printf("Cola vacía, no se puede desencolar.\n");
    }
    return a;
}

// Muestra todos los autos en la cola
void mostrar_cola(ColaEspera *cola) {
    if (esta_vacia(cola)) {
        printf("La cola está vacía.\n");
        return;
    }
    printf("Cola de espera:\n");
    for (int i = 0; i < cola->tamaño; i++) {
        int index = (cola->frente + i) % cola->capacidad;
        printf("Matricula: %s, Hora de llegada: %d, Hora de salida: %d\n", 
               cola->autos[index].matricula, 
               cola->autos[index].hora_llegada, 
               cola->autos[index].hora_Salida);
    }
}
