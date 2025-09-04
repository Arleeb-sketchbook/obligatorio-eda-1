#include <stdio.h>
#include <stdlib.h>

int main() {
    int opcion;

    do {
        // Mostrar el menú
        printf("\n=== Bienvenido al registro de vehículo ===\n");
        printf("1. Ver Estacionamiento\n");
        printf("2. Ver cola de espera\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        // Evaluar la opción
        switch(opcion) {
            case 1:
    do {
        int subopcion;
        printf("\n=== Bienvenido al Control de estacionamiento ===\n");
        printf("1. Registrar Entrada de Vehículo\n");
        printf("2. Registrar Salida de Vehículo\n");
        printf("3. Ver Estacionamiento actual");
        printf("0. Volver al menú anterior\n");
        printf("Seleccione una opción: ");
        scanf("%d", &subopcion);

        if (subopcion == 0) break;  // Esta línea permite volver

        switch(subopcion) {
            case 1:
                printf("Ejecutando Acción A...\n");
                break;
            case 2:
                printf("Ejecutando Acción B...\n");
                break;
            case 3:
                printf("Ejecutando Acción c...\n");
                break;
            default:
                printf("Opción inválida.\n");
        }

    } while (1);  // Repite hasta que el usuario elija volver
    break;

            case 2:
                   do {
        int subopcion;
        printf("\n=== Bienvenido al Control de Cola de espera ===\n");
        printf("1. Ver Cola de espera\n");
        printf("2. Registrar Salida de Vehículo\n");
        printf("0. Volver al menú anterior\n");
        printf("Seleccione una opción: ");
        scanf("%d", &subopcion);

        if (subopcion == 0) break;  // Esta línea permite volver

        switch(subopcion) {
            case 1:
                printf("Esta es la cola de espera\n");
                break;
            case 2:
                printf("Registre Vehiculo\n");
                break;
            default:
                printf("Opción inválida.\n");
        }

    } while (1);  // Repite hasta que el usuario elija volver
    break;
            case 3:
                printf("Has seleccionado la Opción 3\n");
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }

    } while(opcion != 0); // Repetir mientras no se elija salir
return 0;
}