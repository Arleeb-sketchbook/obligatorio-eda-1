#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_AUTOS 100
#define MAX_COLA 35
#define MAX_HISTORIAL 200
#define MAX_ESPERA 5  

typedef struct {
    char nombreProp[50];
    char modelo[50];
    char matricula[8];
    char nacionalidad[30];
    int hora_llegada;
    int minutos_llegada;
    int hora_Salida;
    int minutos_Salida;
    int tiempo_espera; 
    int abandono;      
} Auto;

typedef struct {
    Auto datos[MAX_AUTOS];
    int tope;
} PilaAuto;

typedef struct {
    Auto datos[MAX_AUTOS];
    int tope;
} PilaAuxiliar;

typedef struct {
    Auto *autos;
    int frente;
    int final;
    int capacidad;
    int tamaño;
} ColaEspera;

typedef struct {
    Auto *autos;
    int cantidad;
    int capacidad;
} Historial;

Auto crearAuto(const char *nombreProp, const char *modelo, const char *matricula,
               const char *nacionalidad,
               int hora_llegada, int minutos_llegada, int hora_Salida, int minutos_Salida) {
    Auto p;
    strncpy(p.nombreProp, nombreProp, sizeof(p.nombreProp) - 1);
    p.nombreProp[sizeof(p.nombreProp) - 1] = '\0';

    strncpy(p.modelo, modelo, sizeof(p.modelo) - 1);
    p.modelo[sizeof(p.modelo) - 1] = '\0';

    strncpy(p.matricula, matricula, sizeof(p.matricula) - 1);
    p.matricula[sizeof(p.matricula) - 1] = '\0';

    strncpy(p.nacionalidad, nacionalidad, sizeof(p.nacionalidad) - 1);
    p.nacionalidad[sizeof(p.nacionalidad) - 1] = '\0';

    p.hora_llegada = hora_llegada;
    p.minutos_llegada = minutos_llegada;
    p.hora_Salida = hora_Salida;
    p.minutos_Salida = minutos_Salida;
    p.tiempo_espera = 0;
    p.abandono = 0;

    return p;
}

Auto crearAutoDesdeInput() {
    char nombreProp[50];
    char modelo[50];
    char matricula[8];
    char nacionalidad[30];
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

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Ingrese nacionalidad: ");
    fgets(nacionalidad, sizeof(nacionalidad), stdin);
    nacionalidad[strcspn(nacionalidad, "\n")] = 0;

    printf("Ingrese hora de llegada (hh): ");
    scanf("%d", &hora_llegada);
    printf("Ingrese minuto de llegada (mm): ");
    scanf("%d", &minutos_llegada);

    return crearAuto(nombreProp, modelo, matricula, nacionalidad, hora_llegada, minutos_llegada, -1, -1);
}

void mostrarAuto(Auto a) {
    printf("\n--- Vehículo ---\n");
    printf("Propietario: %s\n", a.nombreProp);
    printf("Modelo: %s\n", a.modelo);
    printf("Matrícula: %s\n", a.matricula);
    printf("Nacionalidad: %s\n", a.nacionalidad);
    printf("Hora de llegada: %02d:%02d\n", a.hora_llegada, a.minutos_llegada);
    if (a.hora_Salida != -1 && a.minutos_Salida != -1)
        printf("Hora de salida: %02d:%02d\n", a.hora_Salida, a.minutos_Salida);
    if (a.abandono)
        printf(" Este auto abandonó la cola por exceso de espera.\n");
    printf("------------------\n");
}

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
    Auto a = {"", "", "", "", -1, -1, -1, -1, 0, 0};
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
        printf("Propietario:%s, Modelo:%s, Matricula:%s, Nacionalidad:%s, Espera:%d min\n",
                 cola->autos[index].nombreProp,
                 cola->autos[index].modelo,
                 cola->autos[index].matricula,
                 cola->autos[index].nacionalidad,
                 cola->autos[index].tiempo_espera);
    }
}

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

int estacionamentoIsFull(PilaAuto* estacionamiento){
    return (estacionamiento->tope + 1 >= MAX_AUTOS);
}

int autoExiste(PilaAuto* estacionamiento, ColaEspera* cola, Auto nuevo) {
    for (int i = 0; i <= estacionamiento->tope; i++) {
        if (strcmp(estacionamiento->datos[i].matricula, nuevo.matricula) == 0 &&
            strcmp(estacionamiento->datos[i].nacionalidad, nuevo.nacionalidad) == 0) {
            return 1;
        }
    }
    for (int i = 0; i < cola->tamaño; i++) {
        int index = (cola->frente + i) % cola->capacidad;
        if (strcmp(cola->autos[index].matricula, nuevo.matricula) == 0 &&
            strcmp(cola->autos[index].nacionalidad, nuevo.nacionalidad) == 0) {
            return 1;
        }
    }
    return 0;
}

void actualizar_cola(ColaEspera *cola, Historial *historial) {
    int n = cola->tamaño;
    for (int i = 0; i < n; i++) {
        int index = (cola->frente + i) % cola->capacidad;
        cola->autos[index].tiempo_espera++;

        if (cola->autos[index].tiempo_espera >= MAX_ESPERA) {
            Auto queSeVa = cola->autos[index];
            queSeVa.abandono = 1;

            for (int j = i; j < n - 1; j++) {
                int idx1 = (cola->frente + j) % cola->capacidad;
                int idx2 = (cola->frente + j + 1) % cola->capacidad;
                cola->autos[idx1] = cola->autos[idx2];
            }
            cola->final = (cola->final - 1 + cola->capacidad) % cola->capacidad;
            cola->tamaño--;

            agregar_a_historial(historial, queSeVa);
            printf(" El auto de %s se fue de la cola por esperar demasiado.\n", queSeVa.nombreProp);
            n--;
            i--;
        }
    }
}

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
                                printf("El estacionamiento y la cola de espera estan llenas.\n");
                            } else {
                                Auto nuevo = crearAutoDesdeInput();
                                if (autoExiste(estacionamiento, &cola, nuevo)) {
                                    printf("Auto y nacionalidad ya registrada.\n");
                                } else {
                                    if (estacionamentoIsFull(estacionamiento)) {
                                        encolar(&cola, nuevo);
                                        printf("Estacionamiento lleno. Vehículo enviado a la cola.\n");
                                    } else {
                                        push(estacionamiento, nuevo);
                                        printf("Vehículo registrado correctamente.\n");
                                    }
                                }
                            }
                            actualizar_cola(&cola, &historial);
                            break;
                        }
                        case 2: {
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos para retirar.\n");
                            } else {
                                printf("\nQue auto desea retirar?\n");
                                int indice = estacionamiento->tope;
                                for (indice; indice >= 0; indice--) {
                                    printf("%d.Propietario: %s | Modelo: %s | Matrícula: %s | Nacionalidad: %s | Llegada: %02d:%02d\n",
                                           indice,
                                           estacionamiento->datos[indice].nombreProp,
                                           estacionamiento->datos[indice].modelo,
                                           estacionamiento->datos[indice].matricula,
                                           estacionamiento->datos[indice].nacionalidad,
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
                            actualizar_cola(&cola, &historial);
                            break;
                        }
                        case 3: {
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos en el estacionamiento.\n");
                            } else {
                                printf("\n--- Autos en el estacionamiento ---\n");
                                for (int i = estacionamiento->tope; i >= 0; i--) {
                                    printf("Propietario: %s | Modelo: %s | Matrícula: %s | Nacionalidad: %s | Llegada: %02d:%02d\n",
                                           estacionamiento->datos[i].nombreProp,
                                           estacionamiento->datos[i].modelo,
                                           estacionamiento->datos[i].matricula,
                                           estacionamiento->datos[i].nacionalidad,
                                           estacionamiento->datos[i].hora_llegada,
                                           estacionamiento->datos[i].minutos_llegada);
                                }
                            }
                            actualizar_cola(&cola, &historial);
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
                            actualizar_cola(&cola, &historial);
                            break;
                        }
                        default: printf("Opción inválida.\n");
                    }
                } while (1);
                break;
            }
            case 3: mostrar_historial(&historial); break;
            case 0: printf("Gracias por usar Autocar\n"); break;
            default: printf("Opción inválida\n");
        }
    } while (opcion != 0);

    free(cola.autos);
    free(estacionamiento);
    free(pilaAux);
    free(historial.autos);

    return 0;
}