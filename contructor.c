// Sistema de Gestión de Estacionamiento — Constructores / Inicialización
// ---------------------------------------------------------------
// Este archivo define las estructuras de datos (pila, cola, historial)
// y sus funciones "constructoras" (inicialización) para integrarlas con
// tu menú existente. Luego podrás implementar las operaciones (entrada,
// salida, visualización, etc.) usando estas estructuras.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// =====================
// Parámetros del sistema
// =====================
#define MAX_PARKING 100   // Capacidad de la pila del estacionamiento
#define MAX_COLA    35    // Capacidad de la cola de espera
#define MAX_HIST   1024   // Tamaño máximo del historial del día

#define PLACA_MAX   16    // Largo máximo de la matrícula (incluye terminador '\0')
#define HORA_MAX    9     // "HH:MM:SS" => 8 + '\0'

// =====================
// Tipos de datos
// =====================

typedef struct {
    char placa[PLACA_MAX];
    char horaEntrada[HORA_MAX];
    char horaSalida[HORA_MAX];
} Auto;

// Pila para el estacionamiento (y para la pila auxiliar)
typedef struct {
    Auto datos[MAX_PARKING];
    int tope; // -1 vacía; [0..MAX_PARKING-1] índice del último
} PilaAutos;

// Cola circular para la espera
typedef struct {
    Auto datos[MAX_COLA];
    int frente; // índice del primer elemento
    int fondo;  // índice del último elemento
    int tam;    // cantidad actual de elementos
} ColaAutos;

// Historial
typedef struct {
    Auto datos[MAX_HIST];
    int tam; // cantidad de registros almacenados
} HistorialAutos;

// Estructura principal que agrupa todo

typedef struct {
    PilaAutos estacionamiento; // pila principal
    PilaAutos aux;             // pila auxiliar para maniobras de salida
    ColaAutos espera;          // cola de espera
    HistorialAutos historial;  // array de historial
} SistemaParking;

// =====================
// Utilidades de tiempo (opcional)
// =====================
static void hora_actual_str(char destino[HORA_MAX]) {
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    if (lt) {
        snprintf(destino, HORA_MAX, "%02d:%02d:%02d",
                 lt->tm_hour, lt->tm_min, lt->tm_sec);
    } else {
        strncpy(destino, "--:--:--", HORA_MAX);
        destino[HORA_MAX - 1] = '\0';
    }
}

// =====================
// "Constructores" (inicializadores)
// =====================

void Pila_init(PilaAutos *p) {
    p->tope = -1;
}

void Cola_init(ColaAutos *c) {
    c->frente = 0;
    c->fondo  = -1;
    c->tam    = 0;
}

void Historial_init(HistorialAutos *h) {
    h->tam = 0;
}

void Sistema_init(SistemaParking *s) {
    Pila_init(&s->estacionamiento);
    Pila_init(&s->aux);
    Cola_init(&s->espera);
    Historial_init(&s->historial);
}

// =====================
// Stubs de operaciones (para que puedas compilar y completar luego)
// =====================

// Devuelven 1 si OK, 0 si no (por capacidad), -1 para errores
int Pila_push(PilaAutos *p, Auto a) {
    if (p->tope >= MAX_PARKING - 1) return 0; // llena
    p->datos[++(p->tope)] = a;
    return 1;
}

int Pila_pop(PilaAutos *p, Auto *out) {
    if (p->tope < 0) return 0; // vacía
    if (out) *out = p->datos[p->tope];
    p->tope--;
    return 1;
}

int Pila_vacia(const PilaAutos *p) { return p->tope < 0; }
int Pila_llena(const PilaAutos *p) { return p->tope >= MAX_PARKING - 1; }

int Cola_enqueue(ColaAutos *c, Auto a) {
    if (c->tam >= MAX_COLA) return 0; // llena
    c->fondo = (c->fondo + 1) % MAX_COLA;
    c->datos[c->fondo] = a;
    c->tam++;
    return 1;
}

int Cola_dequeue(ColaAutos *c, Auto *out) {
    if (c->tam <= 0) return 0; // vacía
    if (out) *out = c->datos[c->frente];
    c->frente = (c->frente + 1) % MAX_COLA;
    c->tam--;
    return 1;
}

int Cola_vacia(const ColaAutos *c) { return c->tam == 0; }
int Cola_llena(const ColaAutos *c) { return c->tam >= MAX_COLA; }

int Historial_agregar(HistorialAutos *h, const Auto *a) {
    if (h->tam >= MAX_HIST) return 0;
    h->datos[h->tam++] = *a;
    return 1;
}

// =====================
// Helpers para crear un Auto con hora actual
// =====================

Auto Auto_con_entrada(const char *placa) {
    Auto a;
    strncpy(a.placa, placa, PLACA_MAX);
    a.placa[PLACA_MAX - 1] = '\0';
    hora_actual_str(a.horaEntrada);
    a.horaSalida[0] = '\0';
    return a;
}

void Auto_set_salida(Auto *a) {
    if (!a) return;
    hora_actual_str(a->horaSalida);
}

// =====================
// Integración mínima con tu main existente
// (ejemplo de uso del "constructor")
// =====================

// Crea una instancia global (o pásala por referencia donde la necesites)
SistemaParking gSistema;

// Llama a esta función al inicio de tu main, antes del bucle del menú.
void InicializarSistema() {
    Sistema_init(&gSistema);
}

// Ejemplos de funciones que luego puedes colgar del menú
typedef enum { REG_OK = 1, REG_LLENO = 0, REG_ERR = -1 } RegRes;

RegRes RegistrarEntrada(const char *placa) {
    Auto a = Auto_con_entrada(placa);

    // Si hay lugar en el estacionamiento, apilar
    if (!Pila_llena(&gSistema.estacionamiento)) {
        return Pila_push(&gSistema.estacionamiento, a) ? REG_OK : REG_ERR;
    }

    // Si no, intentar encolar en la espera
    if (!Cola_llena(&gSistema.espera)) {
        return Cola_enqueue(&gSistema.espera, a) ? REG_OK : REG_ERR;
    }

    return REG_LLENO; // lleno pila y cola
}

// Búsqueda lineal en la pila para ubicar matrícula (devuelve índice o -1)
int Pila_buscar_por_placa(const PilaAutos *p, const char *placa) {
    for (int i = p->tope; i >= 0; --i) {
        if (strcmp(p->datos[i].placa, placa) == 0) return i;
    }
    return -1;
}

RegRes RegistrarSalida(const char *placa) {
    int idx = Pila_buscar_por_placa(&gSistema.estacionamiento, placa);
    if (idx < 0) return REG_ERR; // no encontrado

    // Mover a auxiliar hasta llegar al auto
    Auto tmp;
    int movimientos = 0;
    while (gSistema.estacionamiento.tope > idx) {
        Pila_pop(&gSistema.estacionamiento, &tmp);
        Pila_push(&gSistema.aux, tmp);
        movimientos++;
    }

    // Extraer el auto destino y setear hora de salida
    Auto destino;
    Pila_pop(&gSistema.estacionamiento, &destino);
    Auto_set_salida(&destino);
    Historial_agregar(&gSistema.historial, &destino);

    // Regresar autos movidos
    while (!Pila_vacia(&gSistema.aux)) {
        Pila_pop(&gSistema.aux, &tmp);
        Pila_push(&gSistema.estacionamiento, tmp);
    }

    // Si hay autos esperando, entra el primero automáticamente
    if (!Cola_vacia(&gSistema.espera)) {
        Auto queEntra;
        Cola_dequeue(&gSistema.espera, &queEntra);
        // Mantener horaEntrada original de la cola (ya estaba puesta al encolarse)
        Pila_push(&gSistema.estacionamiento, queEntra);
    }

    (void)movimientos; // por si quieres imprimirlo después
    return REG_OK;
}

// Visualizaciones mínimas
void VerEstacionamientoActual() {
    printf("\n--- Estacionamiento (tope=%d) ---\n", gSistema.estacionamiento.tope);
    for (int i = gSistema.estacionamiento.tope; i >= 0; --i) {
        printf("[%2d] Placa:%s  In:%s\n", i, gSistema.estacionamiento.datos[i].placa,
               gSistema.estacionamiento.datos[i].horaEntrada);
    }
}

void VerColaEspera() {
    printf("\n--- Cola de espera (tam=%d) ---\n", gSistema.espera.tam);
    for (int k = 0; k < gSistema.espera.tam; ++k) {
        int idx = (gSistema.espera.frente + k) % MAX_COLA;
        printf("(%2d) Placa:%s  In:%s\n", k, gSistema.espera.datos[idx].placa,
               gSistema.espera.datos[idx].horaEntrada);
    }
}

void VerHistorial() {
    printf("\n--- Historial (tam=%d) ---\n", gSistema.historial.tam);
    for (int i = 0; i < gSistema.historial.tam; ++i) {
        printf("%4d) Placa:%s  In:%s  Out:%s\n", i + 1,
               gSistema.historial.datos[i].placa,
               gSistema.historial.datos[i].horaEntrada,
               gSistema.historial.datos[i].horaSalida[0] ? gSistema.historial.datos[i].horaSalida : "--:--:--");
    }
}

/* =====================
 * Cómo integrarlo con tu main existente
 * ---------------------
 * 1) Declara `extern SistemaParking gSistema;` en el archivo del menú (si separas en .h/.c),
 *    o simplemente pega este bloque encima de tu `main()` y llama a `InicializarSistema();` una vez.
 * 2) En la opción "Registrar Entrada" del submenú, pide la placa y llama a `RegistrarEntrada(placa)`.
 * 3) En "Registrar Salida", pide la placa y llama a `RegistrarSalida(placa)`.
 * 4) En "Ver Estacionamiento actual", llama a `VerEstacionamientoActual()`.
 * 5) En el menú de la cola, usa `VerColaEspera()` para listar.
 * 6) (Opcional) Agrega una opción para `VerHistorial()`.
 * ===================== */
