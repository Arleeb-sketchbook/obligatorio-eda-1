#include <stdio.h>   // Librería estándar para entrada/salida (printf, scanf, etc.)
#include <string.h>  // Librería para manejar cadenas (strncpy, strlen, etc.)
#include <stdlib.h>  // Librería estándar para memoria dinámica (malloc, free, exit, etc.)

// Constantes que definen los tamaños máximos de estructuras
#define MAX_AUTOS 100        // Capacidad máxima del estacionamiento
#define MAX_COLA 35          // Capacidad máxima de la cola de espera
#define MAX_HISTORIAL 200    // Cantidad máxima de autos guardados en el historial

// --------- ESTRUCTURA PRINCIPAL PARA REPRESENTAR UN AUTO ---------
typedef struct {
    char nombreProp[50];   // Nombre del propietario
    char modelo[50];       // Modelo del auto
    char matricula[8];     // Matrícula (exactamente 7 caracteres + terminador '\0')
    int hora_llegada;      // Hora de llegada al estacionamiento
    int minutos_llegada;   // Minutos de llegada
    int hora_Salida;       // Hora de salida (si no salió aún, vale -1)
    int minutos_Salida;    // Minutos de salida (si no salió aún, vale -1)
} Auto;

// --------- PILA PRINCIPAL (ESTACIONAMIENTO) ---------
typedef struct {
    Auto datos[MAX_AUTOS]; // Arreglo que guarda los autos apilados
    int tope;              // Índice del último auto en la pila (-1 si está vacía)
} PilaAuto;

// --------- PILA AUXILIAR (USADA PARA MOVER AUTOS TEMPORALMENTE) ---------
typedef struct {
    Auto datos[MAX_AUTOS]; // Arreglo de autos temporal
    int tope;              // Índice del último auto en la pila
} PilaAuxiliar;

// --------- COLA DE ESPERA (CUANDO EL ESTACIONAMIENTO ESTÁ LLENO) ---------
typedef struct {
    Auto *autos;     // Arreglo dinámico que guarda autos en espera
    int frente;      // Índice del primer auto en la cola
    int final;       // Índice del último auto en la cola
    int capacidad;   // Capacidad máxima de la cola
    int tamaño;      // Cantidad actual de autos en la cola
} ColaEspera;

// --------- HISTORIAL (AUTOS QUE YA SALIERON) ---------
typedef struct {
    Auto *autos;     // Arreglo dinámico para guardar autos que salieron
    int cantidad;    // Cantidad de autos registrados
    int capacidad;   // Capacidad máxima del historial
} Historial;

// --------- FUNCIONES DE CREACIÓN Y MANEJO DE AUTOS ---------

// Crear un auto con parámetros específicos
Auto crearAuto(const char *nombreProp, const char *modelo, const char *matricula,
               int hora_llegada, int minutos_llegada, int hora_Salida, int minutos_Salida) {
    Auto p;

    // Copiar strings en los campos de la estructura de manera segura
    strncpy(p.nombreProp, nombreProp, sizeof(p.nombreProp) - 1);
    p.nombreProp[sizeof(p.nombreProp) - 1] = '\0';

    strncpy(p.modelo, modelo, sizeof(p.modelo) - 1);
    p.modelo[sizeof(p.modelo) - 1] = '\0';

    strncpy(p.matricula, matricula, sizeof(p.matricula) - 1);
    p.matricula[sizeof(p.matricula) - 1] = '\0';

    // Guardar los horarios de llegada y salida
    p.hora_llegada = hora_llegada;
    p.minutos_llegada = minutos_llegada;
    p.hora_Salida = hora_Salida;
    p.minutos_Salida = minutos_Salida;

    return p;
}

// Crear un auto ingresando datos por teclado
Auto crearAutoDesdeInput() {
    char nombreProp[50];
    char modelo[50];
    char matricula[8];
    int hora_llegada, minutos_llegada;

    getchar(); // Limpia el buffer de entrada por si quedó un salto de línea

    // Pedir nombre del propietario
    printf("Ingrese nombre del propietario: ");
    fgets(nombreProp, sizeof(nombreProp), stdin);
    nombreProp[strcspn(nombreProp, "\n")] = 0; // Eliminar el salto de línea final

    // Pedir modelo del auto
    printf("Ingrese modelo del auto: ");
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = 0;

    // Validar matrícula (debe tener exactamente 7 caracteres)
    while (1) {
        printf("Ingrese matrícula (7 caracteres): ");
        fgets(matricula, sizeof(matricula), stdin);
        matricula[strcspn(matricula, "\n")] = 0;

        if (strlen(matricula) != 7) {
            printf("Error: la matrícula debe tener exactamente 7 caracteres.\n");
        } else break;
    }

    // Pedir hora de llegada
    printf("Ingrese hora de llegada (hh): ");
    scanf("%d", &hora_llegada);
    printf("Ingrese minuto de llegada (mm): ");
    scanf("%d", &minutos_llegada);

    // Crear el auto con salida = -1 (porque aún no salió)
    return crearAuto(nombreProp, modelo, matricula, hora_llegada, minutos_llegada, -1, -1);
}

// Mostrar un auto en pantalla
void mostrarAuto(Auto a) {
    printf("\n--- Vehículo ---\n");
    printf("Propietario: %s\n", a.nombreProp);
    printf("Modelo: %s\n", a.modelo);
    printf("Matrícula: %s\n", a.matricula);
    printf("Hora de llegada: %02d:%02d\n", a.hora_llegada, a.minutos_llegada);
    if (a.hora_Salida != -1 && a.minutos_Salida != -1) // Solo mostrar salida si es válida
        printf("Hora de salida: %02d:%02d\n", a.hora_Salida, a.minutos_Salida);
    printf("------------------\n");
}

// --------- FUNCIONES PARA MANEJAR LA PILA (ESTACIONAMIENTO) ---------

// Crear pila vacía
PilaAuto* crearPilaAuto() {
    PilaAuto* pila = (PilaAuto*)malloc(sizeof(PilaAuto));
    pila->tope = -1; // Inicialmente vacía
    return pila;
}

// Insertar auto en la pila
void push(PilaAuto* pila, Auto autoObj) {
    if (pila->tope + 1 == MAX_AUTOS) {
        printf("Pila llena\n");
        return;
    }
    pila->datos[++pila->tope] = autoObj;
}

// Retirar auto de la pila
Auto pop(PilaAuto* pila) {
    if (pila->tope == -1) {
        printf("Pila vacía\n");
        exit(1);
    }
    return pila->datos[pila->tope--];
}

// --------- FUNCIONES PARA MANEJAR LA PILA AUXILIAR ---------
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

// --------- FUNCIONES PARA MANEJAR LA COLA DE ESPERA ---------

// Inicializar cola
void inicializar_ColaEspera(ColaEspera *cola, int capacidad) {
    cola->autos = (Auto *)malloc(capacidad * sizeof(Auto));
    cola->frente = 0;
    cola->final = -1;
    cola->capacidad = capacidad;
    cola->tamaño = 0;
}

// Verificar si la cola está vacía
int esta_vacia(ColaEspera *cola) {
    return cola->tamaño == 0;
}

// Verificar si la cola está llena
int esta_llena(ColaEspera *cola) {
    return cola->tamaño == cola->capacidad;
}

// Encolar auto en la cola de espera
void encolar(ColaEspera *cola, Auto a) {
    if (!esta_llena(cola)) {
        cola->final = (cola->final + 1) % cola->capacidad;
        cola->autos[cola->final] = a;
        cola->tamaño++;
    } else {
        printf("Cola llena, no se puede encolar.\n");
    }
}

// Desencolar auto de la cola
Auto desencolar(ColaEspera *cola) {
    Auto a = {"", "", "", -1, -1, -1, -1}; // Auto vacío si no hay nada
    if (!esta_vacia(cola)) {
        a = cola->autos[cola->frente];
        cola->frente = (cola->frente + 1) % cola->capacidad;
        cola->tamaño--;
    }
    return a;
}

// Mostrar autos en la cola
void mostrar_cola(ColaEspera *cola) {
    if (esta_vacia(cola)) {
        printf("La cola está vacía.\n");
        return;
    }
    printf("Cola de espera:\n");
    for (int i = 0; i < cola->tamaño; i++) {
        int index = (cola->frente + i) % cola->capacidad;
        if (cola->autos[index].hora_Salida == -1) { // Solo mostrar si aún no salió
            printf("Propietario:%s, Modelo:%s, Matricula: %s, Hora de llegada: %02d:%02d\n",
                     cola->autos[index].nombreProp,
                     cola->autos[index].modelo,
                   cola->autos[index].matricula,
                   cola->autos[index].hora_llegada,
                   cola->autos[index].minutos_llegada);
        }
    }
}

// --------- FUNCIONES PARA MANEJAR EL HISTORIAL ---------

// Inicializar historial
void inicializar_historial(Historial *historial, int capacidad) {
    historial->autos = (Auto *)malloc(capacidad * sizeof(Auto));
    historial->cantidad = 0;
    historial->capacidad = capacidad;
}

// Agregar auto al historial
void agregar_a_historial(Historial *historial, Auto a) {
    if (historial->cantidad < historial->capacidad) {
        historial->autos[historial->cantidad++] = a;
    } else {
        printf("Historial lleno, no se puede registrar más autos.\n");
    }
}

// Mostrar historial de autos
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

// --------- FUNCIONES DE APOYO ---------

// Verificar si el estacionamiento está lleno
int estacionamentoIsFull(PilaAuto* estacionamiento){
    return (estacionamiento->tope + 1 >= MAX_AUTOS);
}

// --------- PROGRAMA PRINCIPAL ---------
int main() {
    int opcion;

    // Inicializar estructuras
    ColaEspera cola;
    inicializar_ColaEspera(&cola, MAX_COLA);

    PilaAuxiliar* pilaAux = crearPilaAuxiliar();
    PilaAuto* estacionamiento = crearPilaAuto();

    Historial historial;
    inicializar_historial(&historial, MAX_HISTORIAL);

    // Menú principal
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
                // Submenú de estacionamiento
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
                            // Registrar entrada
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
                            // Registrar salida
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos para retirar.\n");
                            } else {
                                printf("\nQue auto desea retirar?\n");

                                // Mostrar autos con índice
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

                                // Pedir índice
                                indice = estacionamiento->tope;
                                printf("Ingrese el índice del auto a retirar (0 a %d): ", indice);
                                scanf("%d", &indice);

                                if (indice < 0 || indice > estacionamiento->tope) {
                                    printf("Índice inválido.\n");
                                    break;
                                } else {
                                    // Mover autos a la pila auxiliar hasta llegar al elegido
                                    while (estacionamiento->tope != indice) {
                                        Auto temp = pop(estacionamiento);
                                        push_aux(pilaAux, temp);
                                    }
                                }

                                // Registrar salida del auto
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
                                
                                // Volver a poner los autos en su lugar
                                while (pilaAux->tope != -1) {
                                    Auto temp = pop_aux(pilaAux);
                                    push(estacionamiento, temp);
                                }

                                // Si queda lugar, pasar auto desde la cola
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
                            // Ver estacionamiento
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
                // Submenú de cola de espera
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
                            // Retirar auto directamente desde la cola
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