#include <stdio.h>    // Biblioteca estándar para entrada y salida (printf, scanf, fgets)
#include <string.h>   // Biblioteca para manipulación de cadenas (strncpy, strlen, strcspn)
#include <stdlib.h>   // Biblioteca para funciones generales (malloc, free, exit)

// Definimos una constante para el máximo número de autos que puede almacenar la pila
#define MAX_AUTOS 100

// Definición de la estructura Auto, que almacena los datos de cada vehículo
typedef struct {
    char nombreProp[50];   // Cadena para el nombre del propietario (máximo 49 caracteres + '\0')
    char modelo[50];       // Cadena para el modelo del auto (máximo 49 caracteres + '\0')
    char matricula[8];     // Cadena para la matrícula, 7 caracteres + '\0' para terminar string
    int hora_llegada;      // Entero que guarda la hora de llegada (formato 24h)
    int minutos_llegada;   // Entero que guarda los minutos de llegada
    int hora_Salida;       // Entero que guarda la hora de salida (-1 si no registrada)
    int minutos_Salida;    // Entero que guarda los minutos de salida (-1 si no registrada)
} Auto;

// Definición de la estructura PilaAuto para manejar una pila de Autos
typedef struct {
    Auto datos[MAX_AUTOS]; // Arreglo estático donde se almacenan los autos
    int tope;              // Índice del último elemento agregado (top de la pila), -1 si está vacía
} PilaAuto;

// Función para crear dinámicamente una pila vacía de autos
PilaAuto* crearPilaAuto() {
    // Reservamos memoria en el heap para una PilaAuto
    PilaAuto* pila = (PilaAuto*)malloc(sizeof(PilaAuto)); 

    // Inicializamos el tope en -1 para indicar que la pila está vacía (sin elementos)
    pila->tope = -1; 

    // Retornamos el puntero a la pila recién creada
    return pila; 
}

// Función para insertar (push) un auto en la pila
void push(PilaAuto* pila, Auto autoObj) {
    // Primero verificamos que no se haya alcanzado la capacidad máxima
    if (pila->tope + 1 == MAX_AUTOS) { 
        printf("Pila llena\n");  // Informamos que la pila está llena y no se puede agregar más autos
        return; // Salimos sin agregar nada
    }

    // Incrementamos el tope y guardamos el auto en esa posición
    // Esto se hace en una sola línea: primero incrementamos tope con ++ y luego usamos ese índice
    pila->datos[++pila->tope] = autoObj; 
}

// Función para extraer (pop) un auto de la cima de la pila
Auto pop(PilaAuto* pila) {
    // Verificamos que la pila no esté vacía
    if (pila->tope == -1) { 
        printf("Pila vacía\n");  // No hay autos para extraer, informamos y terminamos el programa
        exit(1); 
    }
    // Retornamos el auto que está en el índice tope y luego decrementamos el tope (lo removemos)
    return pila->datos[pila->tope--]; 
}

// Función "constructor" que recibe los datos y crea un Auto estructurado
Auto crearAuto(const char *nombreProp, const char *modelo, const char *matricula,
               int hora_llegada, int minutos_llegada, int hora_Salida, int minutos_Salida) {
    Auto p; // Declaramos un Auto local que se retornará

    // Copiamos el nombre del propietario en el campo correspondiente, con límite para evitar overflow
    strncpy(p.nombreProp, nombreProp, sizeof(p.nombreProp) - 1);
    // Ponemos el último carácter en '\0' para asegurarnos de que la cadena esté terminada
    p.nombreProp[sizeof(p.nombreProp) - 1] = '\0'; 

    // Copiamos el modelo del auto de forma segura (igual que con el nombre)
    strncpy(p.modelo, modelo, sizeof(p.modelo) - 1);
    p.modelo[sizeof(p.modelo) - 1] = '\0';

    // Copiamos la matrícula de forma segura (máximo 7 caracteres + '\0')
    strncpy(p.matricula, matricula, sizeof(p.matricula) - 1);
    p.matricula[sizeof(p.matricula) - 1] = '\0';

    // Asignamos las horas y minutos de llegada y salida que se pasaron como parámetros
    p.hora_llegada = hora_llegada;
    p.minutos_llegada = minutos_llegada;
    p.hora_Salida = hora_Salida;
    p.minutos_Salida = minutos_Salida;

    return p; // Retornamos el Auto completo con todos sus datos
}

// Función que lee desde la entrada estándar los datos para crear un Auto nuevo
Auto crearAutoDesdeInput() {
    char nombreProp[50];   // Buffer temporal para almacenar el nombre
    char modelo[50];       // Buffer temporal para el modelo
    char matricula[8];     // Buffer para la matrícula (7 caracteres + '\0')
    int hora_llegada;      // Variable para la hora de llegada
    int minutos_llegada;   // Variable para los minutos de llegada

    getchar(); // Consumimos el salto de línea residual en el buffer para evitar problemas con fgets

    printf("Ingrese nombre del propietario: "); // Pedimos el nombre
    fgets(nombreProp, sizeof(nombreProp), stdin); 
    // Leemos la cadena, fgets añade el '\n' al final, que eliminamos a continuación
    nombreProp[strcspn(nombreProp, "\n")] = 0; // strcspn busca el índice del '\n' y lo reemplaza con '\0'

    printf("Ingrese modelo del auto: "); // Pedimos el modelo
    fgets(modelo, sizeof(modelo), stdin);
    modelo[strcspn(modelo, "\n")] = 0; // Eliminamos salto de línea al final

    // Ciclo para validar que la matrícula tenga exactamente 7 caracteres
    while (1) {
        printf("Ingrese matrícula (7 caracteres): "); // Pedimos matrícula
        fgets(matricula, sizeof(matricula), stdin);
        matricula[strcspn(matricula, "\n")] = 0; // Eliminamos salto de línea

        if (strlen(matricula) != 7) { // Verificamos que tenga 7 caracteres exactos
            printf("Error: la matrícula debe tener exactamente 7 caracteres.\n"); // Mensaje de error
        } else {
            break; // Si la longitud es correcta, salimos del ciclo
        }
    }

    printf("Ingrese hora de llegada (hh mm): "); // Pedimos hora y minutos separados por espacio
    scanf("%d %d", &hora_llegada, &minutos_llegada); // Leemos ambas variables

    // Llamamos a crearAuto con los datos ingresados y ponemos la hora de salida en -1 (no registrada)
    return crearAuto(nombreProp, modelo, matricula, hora_llegada, minutos_llegada, -1, -1);
}

// Función para mostrar la información de un Auto por pantalla
void mostrarAuto(Auto a) {
    printf("\n--- Vehículo ---\n"); // Encabezado para mejor visualización
    printf("Propietario: %s\n", a.nombreProp); // Imprime el nombre del propietario
    printf("Modelo: %s\n", a.modelo);           // Imprime el modelo del auto
    printf("Matrícula: %s\n", a.matricula);     // Imprime la matrícula
    printf("Hora de llegada: %02d:%02d\n", a.hora_llegada, a.minutos_llegada); 
    // Imprime hora y minutos de llegada, con dos dígitos para formato hh:mm

    // Si la hora y minutos de salida son distintos de -1, la salida está registrada
    if (a.hora_Salida != -1 && a.minutos_Salida != -1) {
        printf("Hora de salida: %02d:%02d\n", a.hora_Salida, a.minutos_Salida); 
        // Imprime hora y minutos de salida
    } else {
        printf("Hora de salida: No registrada\n"); // Mensaje si no hay hora de salida
    }
    printf("------------------\n"); // Separador para visualización clara
}

// Función principal que ejecuta el programa
int main() {
    int opcion; // Variable para guardar la opción del menú

    // Creamos una pila dinámica para manejar los autos en el estacionamiento
    PilaAuto* estacionamiento = crearPilaAuto();

    // Menú principal que se repite hasta que el usuario decide salir
    do {
        printf("\n=== Bienvenido al registro de vehículo ===\n");
        printf("1. Ver Estacionamiento\n");
        printf("2. Ver cola de espera\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion); // Leemos la opción elegida por el usuario

        // Evaluamos la opción con switch para ejecutar la acción correspondiente
        switch (opcion) {
            case 1: { // Menú para controlar el estacionamiento
                int subopcion; // Variable para opción del submenú

                // Bucle para mostrar el submenú repetidamente hasta salir (opción 0)
                do {
                    printf("\n=== Control de Estacionamiento ===\n");
                    printf("1. Registrar Entrada de Vehículo\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("3. Ver Estacionamiento actual\n");
                    printf("0. Volver al menú anterior\n");
                    printf("Seleccione una opción: ");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break; // Salimos del submenú

                    switch (subopcion) {
                        case 1: { // Registrar entrada de vehículo
                            if (estacionamiento->tope + 1 < MAX_AUTOS) { 
                                // Llamamos a crearAutoDesdeInput para pedir datos al usuario
                                Auto nuevoAuto = crearAutoDesdeInput();
                                // Insertamos el auto en la pila
                                push(estacionamiento, nuevoAuto);
                                printf("Vehículo registrado correctamente.\n");
                            } else {
                                printf("Estacionamiento lleno.\n"); // Ya no hay espacio
                            }
                            break;
                        }

                        case 2: { // Registrar salida de vehículo
                            if (estacionamiento->tope == -1) {
                                printf("No hay vehículos en el estacionamiento.\n");
                            } else {
                                // Sacamos el último auto registrado (pila LIFO)
                                Auto autoSalida = pop(estacionamiento);

                                // Pedimos la hora de salida al usuario
                                int h_salida, m_salida;
                                printf("Ingrese hora de salida (hh mm): ");
                                scanf("%d %d", &h_salida, &m_salida);

                                // Actualizamos el auto con la hora de salida ingresada
                                autoSalida.hora_Salida = h_salida;
                                autoSalida.minutos_Salida = m_salida;

                                // Mostramos la información completa del auto que salió
                                mostrarAuto(autoSalida);
                            }
                            break;
                        }

                        case 3: { // Ver todos los autos actualmente en el estacionamiento
                            if (estacionamiento->tope == -1) {
                                printf("No hay autos en el estacionamiento.\n");
                            } else {
                                printf("\n--- Autos en el estacionamiento ---\n");
                                // Recorremos la pila desde el fondo hasta el tope para mostrar todos
                                for (int i = 0; i <= estacionamiento->tope; i++) {
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

                        default:
                            printf("Opción inválida.\n"); // Validación de opciones incorrectas
                    }
                } while (1); // Repetimos hasta que el usuario salga (subopción 0)
                break;
            }

            case 2: { // Menú de la cola de espera (funcionalidad no implementada)
                int subopcion;
                do {
                    printf("\n=== Control de Cola de Espera ===\n");
                    printf("1. Ver Cola de espera\n");
                    printf("2. Registrar Salida de Vehículo\n");
                    printf("0. Volver al menú anterior\n");
                    printf("Seleccione una opción: ");
                    scanf("%d", &subopcion);

                    if (subopcion == 0) break; // Salir submenú

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
            }

            case 0:
                printf("Gracias por usar Autocar\n"); // Mensaje de despedida
                break;

            default:
                printf("Opción inválida. Intente nuevamente.\n"); // Validación menú principal
        }
    } while (opcion != 0); // Se repite hasta que el usuario elija salir

    // Liberamos la memoria de la pila para evitar fugas
    free(estacionamiento);

    return 0; // Fin del programa
}
