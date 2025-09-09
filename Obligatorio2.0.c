#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_AUTOS 100
#define MAX_COLA 35
#define MAX_HISTORIAL 200

// ================== ESTRUCTURAS ==================
// Estructura Auto
typedef struct {
    char nombreProp[50];
    char modelo[50];
    char matricula[8]; // 7 + '\0'
    int hora_llegada;
    int minutos_llegada;
    int hora_Salida;
    int minutos_Salida;
} Auto;

// Estructura de pila
typedef struct {
    Auto datos[MAX_AUTOS];
    int tope;
} PilaAuto;
typedef struct {
    Auto datos[MAX_AUTOS];
    int tope;
} PilaAuxiliar;

// Estructura de cola circular
typedef struct {
    Auto *autos;
    int frente;
    int final;
    int capacidad;
    int tamaño;
} ColaEspera;

// Estructura Historial
typedef struct {
    Auto *autos;
    int cantidad;
    int capacidad;
} Historial;

// ================== FUNCIONES AUTO ==================
Auto crearAuto(const char *nombreProp, const char *modelo, const char *matricula,
               int hora_llegada, int minutos_llegada, int hora_Salida, int minutos_Salida) {
    Auto p;
    strncpy(p.nombreProp, nombreProp, sizeof(p.nombreProp) - 1);
    p.nombreProp[sizeof(p.nombreProp) - 1] = '\0';

    strncpy(p.modelo, modelo, sizeof(p.modelo) - 1);
    p.modelo[sizeof(p.modelo) - 1] = '\0';

    strncpy(p.matricula, matricula, sizeof(p.matricula) - 1);
    p.matricula[sizeof(p.matricula) - 1] = '\0';

    p.hora_llegada = hora_llegada;
    p.minutos_llegada = minutos_llegada;
    p.hora_Salida = hora_Salida;
    p.minutos_Salida = minutos_Salida;

    return p;
}

Auto crearAutoDesdeInput() {
    char nombreProp[50];
    char modelo[50];
    char matricula[8];
    int hora_llegada, minutos_llegada;

    getchar();
    printf("Ingrese nombre del propietario: ");
    fgets(nombreProp, sizeof(nombreProp), stdin);
    nombreProp[strcspn(nombreProp, "\n")] = 0;

    printf("Ingrese modelo del auto: ");
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = 0;

    while (1) {
        printf("Ingrese matrícula (7 caracteres): ");
        fgets(matricula, sizeof(matricula), stdin);
        matricula[strcspn(matricula, "\n")] = 0;

        if (strlen(matricula) != 7) {
            printf("Error: la matrícula debe tener exactamente 7 caracteres.\n");
        } else break;
    }

    printf("Ingrese hora de llegada (hh): ");
    scanf("%d", &hora_llegada);
    printf("Ingrese minuto de llegada (mm): ");
    scanf("%d", &minutos_llegada);

    return crearAuto(nombreProp, modelo, matricula, hora_llegada, minutos_llegada, -1, -1);
}

void mostrarAuto(Auto a) {
    printf("\n--- Vehículo ---\n");
    printf("Propietario: %s\n", a.nombreProp);
    printf("Modelo: %s\n", a.modelo);
    printf("Matrícula: %s\n", a.matricula);
    printf("Hora de llegada: %02d:%02d\n", a.hora_llegada, a.minutos_llegada);
    if (a.hora_Salida != -1 && a.minutos_Salida != -1)
        printf("Hora de salida: %02d:%02d\n", a.hora_Salida, a.minutos_Salida);
    printf("------------------\n");
}

// ================== FUNCIONES PILA ==================
PilaAuto* crearPilaAuto() {
    PilaAuto* pila = (PilaAuto*)malloc(sizeof(PilaAuto));
    pila->tope = -1;
    return pila;
}

void push(PilaAuto* pila, Auto autoObj) {
    if (pila->tope + 1 == MAX_AUTOS) {
        printf("Pila llena\n");
        return;
    }
    pila->datos[++pila->tope] = autoObj;
}

Auto pop(PilaAuto* pila) {
    if (pila->tope == -1) {
        printf("Pila vacía\n");
        exit(1);
    }
    return pila->datos[pila->tope--];
}

//=================== Funciones pila auxiliar ===================
PilaAuxiliar* crearPilaAuxiliar() {
    PilaAuxiliar* pila = (PilaAuxiliar*)malloc(sizeof(PilaAuxiliar));
    pila->tope = -1;
    return pila;
}

void push_aux(PilaAuxiliar* pila, Auto autoObj) {
    if (pila->tope + 1 == MAX_AUTOS) {
        printf("Pila llena\n");
        return;
    }
    pila->datos[++pila->tope] = autoObj;
}

Auto pop_aux(PilaAuxiliar* pila) {
    if (pila->tope == -1) {
        printf("Pila vacía\n");
        exit(1);
    }
    return pila->datos[pila->tope--];
}


// ================== FUNCIONES COLA ==================
void inicializar_ColaEspera(ColaEspera *cola, int capacidad) {
    cola->autos = (Auto *)malloc(capacidad * sizeof(Auto));
    cola->frente = 0;
    cola->final = -1;
    cola->capacidad = capacidad;
    cola->tamaño = 0;
}

int esta_vacia(ColaEspera *cola) {
    return cola->tamaño == 0;
}

int esta_llena(ColaEspera *cola) {
    return cola->tamaño == cola->capacidad;
}

void encolar(ColaEspera *cola, Auto a) {
    if (!esta_llena(cola)) {
        cola->final = (cola->final + 1) % cola->capacidad;
        cola->autos[cola->final] = a;
        cola->tamaño++;
    } else {
        printf("Cola llena, no se puede encolar.\n");
    }
}

Auto desencolar(ColaEspera *cola) {
    Auto a = {"", "", "", -1, -1, -1, -1};
    if (!esta_vacia(cola)) {
        a = cola->autos[cola->frente];
        cola->frente = (cola->frente + 1) % cola->capacidad;
        cola->tamaño--;
    }
    return a;
}

void mostrar_cola(ColaEspera *cola) {
    if (esta_vacia(cola)) {
        printf("La cola está vacía.\n");
        return;
    }
    printf("Cola de espera:\n");
    for (int i = 0; i < cola->tamaño; i++) {
        int index = (cola->frente + i) % cola->capacidad;
        if (cola->autos[index].hora_Salida == -1) {
            printf("Matricula: %s, Hora de llegada: %02d:%02d\n",
                   cola->autos[index].matricula,
                   cola->autos[index].hora_llegada,
                   cola->autos[index].minutos_llegada);
        }
    }
}

// ================== FUNCIONES HISTORIAL ==================
void inicializar_historial(Historial *historial, int capacidad) {
    historial->autos = (Auto *)malloc(capacidad * sizeof(Auto));
    historial->cantidad = 0;
    historial->capacidad = capacidad;
}

void agregar_a_historial(Historial *historial, Auto a) {
    if (historial->cantidad < historial->capacidad) {
        historial->autos[historial->cantidad++] = a;
    } else {
        printf("Historial lleno, no se puede registrar más autos.\n");
    }
}

void mostrar_historial(Historial *historial) {
    if (historial->cantidad == 0) {
        printf("El historial está vacío.\n");
        return;
    }
    printf("\n=== Historial de Vehículos ===\n");
    for (int i = 0; i < historial->cantidad; i++) {
        mostrarAuto(historial->autos[i]);
    }
}

// ================== Estacionamento lleno? ==================
int estacionamentoIsFull(PilaAuto* estacionamiento){
    return (estacionamiento->tope + 1 >= MAX_AUTOS);
}



// ================== MAIN ==================
int main() {
    int opcion;
    ColaEspera cola;
    inicializar_ColaEspera(&cola, MAX_COLA);
    PilaAuxiliar* pilaAux = crearPilaAuxiliar();
    PilaAuto* estacionamiento = crearPilaAuto();
    Historial historial;
    inicializar_historial(&historial, MAX_HISTORIAL);

    do {
        printf("\n=== Bienvenido al registro de vehículo ===\n");
        printf("1. Ver Estacionamiento\n");
        printf("2. Ver cola de espera\n");
        printf("3. Ver historial de vehículos\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                int subopcion;
                do {
                    printf("\n=== Control de Estacionamiento ===\n");
                    printf("1. Registrar Entrada de Vehículo\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("3. Ver Estacionamiento actual\n");
                    printf("0. Volver\n");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break;

                    switch (subopcion) {
                        case 1: {
                            if (estacionamentoIsFull(estacionamiento) && esta_llena(&cola)) {
                                printf("El estacionamiento y la cola de espera estan llenas, no se pueden registrar autos.\n");
                                
                            } else {
                            Auto nuevo = crearAutoDesdeInput();
                            if (estacionamentoIsFull(estacionamiento)) {
                                encolar(&cola, nuevo);
                                printf("Estacionamiento lleno. Vehículo enviado a la cola de espera.\n");
                            } else {
                                push(estacionamiento, nuevo);
                                printf("Vehículo registrado correctamente.\n");
                            }
                        }
                            break;
                        }
                        case 2: {
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos para retirar.\n");
                            } else {

                                printf("\nQue auto desea retirar?\n");
                                int indice = estacionamiento->tope;
                                for (indice; indice >= 0; indice--) {
                                    printf("%d.Propietario: %s | Modelo: %s | Matrícula: %s | Llegada: %02d:%02d\n",
                                           indice,
                                           estacionamiento->datos[indice].nombreProp,
                                           estacionamiento->datos[indice].modelo,
                                           estacionamiento->datos[indice].matricula,
                                           estacionamiento->datos[indice].hora_llegada,
                                           estacionamiento->datos[indice].minutos_llegada);
                                }
                                indice = estacionamiento->tope;
                                printf("Ingrese el índice del auto a retirar (0 a %d): ", indice);
                                scanf("%d", &indice);
                                if (indice < 0 || indice > estacionamiento->tope) {
                                    printf("Índice inválido.\n");
                                    break;
                                } else {
                                    while (estacionamiento->tope != indice) {
                                        Auto temp = pop(estacionamiento);
                                        push_aux(pilaAux, temp);
                                    }
                                }
                                    // Ahora el auto deseado está en la cima de la pila principal



                                int hs, ms;
                                printf("Ingrese hora de salida (hh): ");
                                scanf("%d", &hs);
                                printf("Ingrese minuto de salida (mm): ");
                                scanf("%d", &ms);

                                Auto autoSaliente = pop(estacionamiento);
                                autoSaliente.hora_Salida = hs;
                                autoSaliente.minutos_Salida = ms;
                                mostrarAuto(autoSaliente);
                                agregar_a_historial(&historial, autoSaliente);
                                
                                while (pilaAux->tope != -1) {
                                    Auto temp = pop_aux(pilaAux);
                                    push(estacionamiento, temp);
                                }

                                if (!estacionamentoIsFull(estacionamiento) && !esta_vacia(&cola)) {
                                    Auto desdeCola = desencolar(&cola);
                                    push(estacionamiento, desdeCola);
                                    printf("Vehículo de la cola ingresó al estacionamiento:\n");
                                    mostrarAuto(desdeCola);
                                }
                            }
                            break;
                        }
                        case 3: {
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos en el estacionamiento.\n");
                            } else {
                                printf("\n--- Autos en el estacionamiento ---\n");
                                for (int i = estacionamiento->tope; i >= 0; i--) {
                                    printf("Propietario: %s | Modelo: %s | Matrícula: %s | Llegada: %02d:%02d\n",
                                           estacionamiento->datos[i].nombreProp,
                                           estacionamiento->datos[i].modelo,
                                           estacionamiento->datos[i].matricula,
                                           estacionamiento->datos[i].hora_llegada,
                                           estacionamiento->datos[i].minutos_llegada);
                                }
                            }
                            break;
                        }
                        default: printf("Opción inválida.\n");
                    }
                } while (1);
                break;
            }
            case 2: {
                int subopcion;
                do {
                    printf("\n=== Control de Cola de Espera ===\n");
                    printf("1. Ver Cola de espera\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("0. Volver\n");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break;

                    switch (subopcion) {
                        case 1: mostrar_cola(&cola); break;
                        case 2: {
                            if (!esta_vacia(&cola)) {
                                Auto a = desencolar(&cola);
                                if (a.hora_llegada != -1) {
                                    int hs, ms;
                                    printf("Ingrese hora de salida (hh): ");
                                    scanf("%d", &hs);
                                    printf("Ingrese minuto de salida (mm): ");
                                    scanf("%d", &ms);
                                    a.hora_Salida = hs;
                                    a.minutos_Salida = ms;
                                    printf("Salida de la cola:\n");
                                    mostrarAuto(a);
                                    agregar_a_historial(&historial, a);
                                }
                            } else {
                                printf("La cola está vacía.\n");
                            }
                            break;
                        }
                        default: printf("Opción inválida.\n");
                    }
                } while (1);
                break;
            }
            case 3: mostrar_historial(&historial); break;
            case 0: printf("Gracias por usar Autocar\n"); break;
            default: printf("Opción inválida.\n");
        }
    } while (opcion != 0);

    free(estacionamiento);
    free(cola.autos);
    free(historial.autos);
    return 0;
}

/* 
======================================================
        SISTEMA DE ESTACIONAMIENTO Y COLA
======================================================

MANUAL DE EJEMPLOS DE EJECUCIÓN:

Ejemplo 1: Registrar entrada de un vehículo
------------------------------------------
Menú principal:
1. Ver Estacionamiento
2. Ver cola de espera
3. Ver historial de vehículos
0. Salir
Seleccione una opción: 1

Submenú de estacionamiento:
1. Registrar Entrada de Vehículo
2. Registrar Salida de Vehículo
3. Ver Estacionamiento actual
0. Volver
Seleccione una opción: 1

Ingrese nombre del propietario: Juan Perez
Ingrese modelo del auto: Toyota Corolla
Ingrese matrícula (7 caracteres): ABC1234
Ingrese hora de llegada (hh): 8
Ingrese minuto de llegada (mm): 30

Salida en pantalla:
"Vehículo registrado correctamente."


Ejemplo 2: Registrar salida de un vehículo
------------------------------------------
Menú principal -> Opción 1 (Estacionamiento)
Submenú -> Opción 2 (Registrar Salida)

Ingrese hora de salida (hh): 10
Ingrese minuto de salida (mm): 15

Salida en pantalla:
--- Vehículo ---
Propietario: Juan Perez
Modelo: Toyota Corolla
Matrícula: ABC1234
Hora de llegada: 08:30
Hora de salida: 10:15
------------------

El vehículo queda guardado en el HISTORIAL.


Ejemplo 3: Cola de espera
-------------------------
Si el estacionamiento está lleno, al registrar un nuevo vehículo:
Salida en pantalla:
"Estacionamiento lleno. Vehículo enviado a la cola de espera."

Para ver los autos en espera:
Menú principal -> Opción 2 (Cola de espera)
Submenú -> Opción 1 (Ver Cola de espera)


Ejemplo 4: Registrar salida desde la cola
-----------------------------------------
Menú principal -> Opción 2 (Cola de espera)
Submenú -> Opción 2 (Registrar Salida)

Ingrese hora de salida (hh): 9
Ingrese minuto de salida (mm): 45

Salida en pantalla:
Salida de la cola:
--- Vehículo ---
Propietario: Ana López
Modelo: Ford Fiesta
Matrícula: XYZ5678
Hora de llegada: 08:50
Hora de salida: 09:45
------------------

El vehículo queda guardado en el HISTORIAL.


Ejemplo 5: Consultar historial
------------------------------
Menú principal -> Opción 3 (Ver historial de vehículos)

Salida en pantalla (ejemplo):
=== Historial de Vehículos ===
--- Vehículo ---
Propietario: Juan Perez
Modelo: Toyota Corolla
Matrícula: ABC1234
Hora de llegada: 08:30
Hora de salida: 10:15
------------------
--- Vehículo ---
Propietario: Ana López
Modelo: Ford Fiesta
Matrícula: XYZ5678
Hora de llegada: 08:50
Hora de salida: 09:45
------------------

======================================================
NOTA: Este sistema gestiona:
- Estacionamiento (pila).
- Cola de espera (cola circular).
- Historial de vehículos (lista dinámica).
======================================================
*/
