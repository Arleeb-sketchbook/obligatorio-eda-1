#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_AUTOS 100

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

// Prototipos
PilaAuto* crearPilaAuto();
void push(PilaAuto* pila, Auto autoObj);
Auto pop(PilaAuto* pila);
void mostrarAuto(Auto a);

// Constructor
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

// Crear auto desde input
Auto crearAutoDesdeInput() {
    char nombreProp[50];
    char modelo[50];
    char matricula[8]; // 7 + '\0'
    int hora_llegada, minutos_llegada;

    getchar(); // Limpiar buffer
    printf("Ingrese nombre del propietario: ");
    fgets(nombreProp, sizeof(nombreProp), stdin);
    nombreProp[strcspn(nombreProp, "\n")] = 0;

    printf("Ingrese modelo del auto: ");
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = 0;

    // Validar matrícula
    while (1) {
        printf("Ingrese matrícula (7 caracteres): ");
        fgets(matricula, sizeof(matricula), stdin);
        matricula[strcspn(matricula, "\n")] = 0;

        if (strlen(matricula) != 7) {
            printf("Error: la matrícula debe tener exactamente 7 caracteres.\n");
        } else {
            break;
        }
    }

    printf("Ingrese hora de llegada (hh mm): ");
    scanf("%d %d", &hora_llegada, &minutos_llegada);

    return crearAuto(nombreProp, modelo, matricula, hora_llegada, minutos_llegada, -1, -1);
}

// Crear pila
PilaAuto* crearPilaAuto() {
    PilaAuto* pila = (PilaAuto*)malloc(sizeof(PilaAuto));
    pila->tope = -1;
    return pila;
}

// Push
void push(PilaAuto* pila, Auto autoObj) {
    if (pila->tope + 1 == MAX_AUTOS) {
        printf("Pila llena\n");
        return;
    }
    pila->datos[++pila->tope] = autoObj;
}

// Pop
Auto pop(PilaAuto* pila) {
    if (pila->tope == -1) {
        printf("Pila vacía\n");
        exit(1);
    }
    return pila->datos[pila->tope--];
}

// Mostrar auto
void mostrarAuto(Auto a) {
    printf("\n--- Vehículo ---\n");
    printf("Propietario: %s\n", a.nombreProp);
    printf("Modelo: %s\n", a.modelo);
    printf("Matrícula: %s\n", a.matricula);
    printf("Hora de llegada: %02d:%02d\n", a.hora_llegada, a.minutos_llegada);
    if (a.hora_Salida != -1 && a.minutos_Salida != -1) {
        printf("Hora de salida: %02d:%02d\n", a.hora_Salida, a.minutos_Salida);
    } else {
        printf("Hora de salida: No registrada\n");
    }
    printf("------------------\n");
}

// MAIN
int main() {
    int opcion;

    // Usar pila dinámica para el estacionamiento
    PilaAuto* estacionamiento = crearPilaAuto();

    do {
        printf("\n=== Bienvenido al registro de vehículo ===\n");
        printf("1. Ver Estacionamiento\n");
        printf("2. Ver cola de espera\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

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
                            if (estacionamiento->tope + 1 < MAX_AUTOS) {
                                Auto nuevo = crearAutoDesdeInput();
                                push(estacionamiento, nuevo);
                                printf("Vehículo registrado correctamente.\n");
                            } else {
                                printf("Estacionamiento lleno.\n");
                            }
                            break;

                        case 2:
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos para retirar.\n");
                            } else {
                                printf("Ingrese hora de salida (hh mm): ");
                                int hs, ms;
                                scanf("%d %d", &hs, &ms);

                                Auto autoSaliente = pop(estacionamiento);
                                autoSaliente.hora_Salida = hs;
                                autoSaliente.minutos_Salida = ms;
                                printf("Vehículo retirado:\n");
                                mostrarAuto(autoSaliente);
                            }
                            break;

                        case 3:
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

                        default:
                            printf("Opción inválida.\n");
                    }

                } while (1);
                break;

            case 2:
                // Cola de espera (no implementada aún)
                printf("Funcionalidad de cola de espera aún no implementada.\n");
                break;

            case 0:
                printf("Gracias por usar Autocar\n");
                break;

            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }

    } while (opcion != 0);

    // Liberar memoria
    free(estacionamiento);
    return 0;
}
