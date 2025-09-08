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

// Función para crear un Auto desde la entrada del usuario
Auto crearAutoDesdeInput() {
    char nombreProp[50];
    char modelo[50];
    char matricula[8]; // 7 + '\0'
    int hora_llegada, minutos_llegada;

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

int main() {
    int opcion;
    Auto estacionamiento[MAX_AUTOS];
    int cantidadAutos = 0;

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
                                           estacionamiento[i].nombreProp,
                                           estacionamiento[i].modelo,
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
                    printf("Seleccione una opción: ");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break;

                    switch (subopcion) {
                        case 1:
                            printf("Funcionalidad de cola de espera aún no implementada.\n");
                            break;
                        case 2:
                            printf("Funcionalidad de registrar salida desde cola aún no implementada.\n");
                            break;
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
