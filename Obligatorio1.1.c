#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constantes
#define MAX_AUTOS 100

// Definición de estructura
typedef struct {
    char nombreProp[50];
    char modelo[50];
    char matricula[7]; // 6 + '\0'
    int hora_llegada;
    int minutos_llegada;
    int hora_Salida;
    int minutos_Salida;
} Auto;

// "Constructor"
Auto crearAuto(const char *nombreProp, const char *modelo,const char *matricula,
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

// Función para crear un Auto desde la entrada del usuario
Auto crearAutoDesdeInput() {
    char matricula[8]; // 7 + '\0'
    int hora_llegada, minutos_llegada;
    char nombreProp[50];
    char modelo[50];
    printf("Ingrese nombre del propietario: ");
    getchar(); // Limpiar el buffer del teclado
    fgets(nombreProp, sizeof(nombreProp), stdin);
    nombreProp[strcspn(nombreProp, "\n")] = 0;

    printf("Ingrese modelo del auto: ");
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = 0;

    printf("Ingrese matrícula (7 caracteres): ");
    fgets(matricula, sizeof(matricula), stdin);
    matricula[strcspn(matricula, "\n")] = 0;

    printf("Ingrese hora de llegada (hh mm): ");
    scanf("%d %d", &hora_llegada, &minutos_llegada);

    return crearAuto(nombreProp, modelo, matricula, hora_llegada, minutos_llegada, -1, -1);
}

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
        if (cola->autos[index].hora_Salida == -1) {
            printf("Matricula: %s, Hora de llegada: %d\n",
                   cola->autos[index].matricula,
                   cola->autos[index].hora_llegada);
            }
    }
}

int main() {
    int opcion;
    Auto estacionamiento[MAX_AUTOS];
    int cantidadAutos = 0;
    ColaEspera cola;
    inicializar_ColaEspera(&cola, MAX_COLA);
    do {
        // Mostrar el menú
        printf("\n=== Bienvenido al registro de vehículo ===\n");
        printf("1. Ver Estacionamiento\n");
        printf("2. Ver cola de espera\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        // Evaluar la opción
        switch (opcion) {
            case 1:
                // Submenú de Estacionamiento
                do {
                    int subopcion;
                    printf("\n=== Control de Estacionamiento ===\n");
                    printf("1. Registrar Entrada de Vehículo\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("3. Ver Estacionamiento actual\n");
                    printf("0. Volver al menú anterior\n");
                    printf("Seleccione una opción: ");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break;

                    switch (subopcion) {
                        case 1:
                            if (cantidadAutos < MAX_AUTOS) {
                                estacionamiento[cantidadAutos] = crearAutoDesdeInput();
                                cantidadAutos++;
                                printf("Vehículo registrado correctamente.\n");
                            } else {
                                printf("Estacionamiento lleno.\n");
                            }
                            break;

                        case 2:
                            printf("Funcionalidad de registrar salida aún no implementada.\n");
                            break;

                        case 3:
                            if (cantidadAutos == 0) {
                                printf("No hay autos en el estacionamiento.\n");
                            } else {
                                printf("\n--- Autos en el estacionamiento ---\n");
                                for (int i = 0; i < cantidadAutos; i++) {
                                    printf("Propietario: %s | Modelo: %s | Matrícula: %s | Llegada: %02d:%02d\n",
                                           estacionamiento[i].matricula,
                                           estacionamiento[i].hora_llegada,
                                           estacionamiento[i].minutos_llegada);
                                }
                            }
                            break;

                        default:
                            printf("Opción inválida.\n");
                    }

                } while (1);
                break;

            case 2:
                // Submenú de Cola de Espera
                do {
                    int subopcion;
                    printf("\n=== Control de Cola de Espera ===\n");
                    printf("1. Ver Cola de espera\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("0. Volver al menú anterior\n");
                    printf("3.[DEBUG] Encolar vehículo de prueba\n");
                    printf("4.[DEBUG] Desencolar vehículo de prueba\n");
                    printf("Seleccione una opción: ");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break;

                    switch (subopcion) {
                        case 1:
                            mostrar_cola(&cola);
                            //printf("Funcionalidad de cola de espera aún no implementada.\n");
                            break;
                        case 2:
                            printf("Funcionalidad de registrar salida desde cola aún no implementada.\n");
                            break;
                        case 3: {
                            Auto a = crearAutoDesdeInput();
                            encolar(&cola, a);
                            break;
                        }
                        case 4: {
                            Auto a = desencolar(&cola);
                            if (a.hora_llegada != -1) {
                                printf("Desencolado: Matricula: %s, Hora de llegada: %d, Hora de salida: %d\n",
                                       a.matricula, a.hora_llegada, a.hora_Salida);
                            }
                            break;
                        }
                        default:
                            printf("Opción inválida.\n");
                    }
                } while (1);
                break;

            case 0:
                printf("Gracias por usar Autocar\n");
                break;

            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }

    } while (opcion != 0);

    return 0;
}
