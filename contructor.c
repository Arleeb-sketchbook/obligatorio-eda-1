// Sistema de Gestión de Estacionamiento — Constructores / Inicialización
// ---------------------------------------------------------------
// Este archivo define las estructuras de datos (pila, cola, historial)
// y sus funciones "constructoras" (inicialización) para integrarlas con
// tu menú existente. Luego podrás implementar las operaciones (entrada,
// salida, visualización, etc.) usando estas estructuras.

// Sistema de Gestión de Estacionamiento — Versión con horas manuales
// ---------------------------------------------------------------
// Se reemplazó el uso de time() por lectura manual (input) del usuario
// con funciones seguras (fgets + eliminación de salto de línea).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARKING 100
#define MAX_COLA    35
#define MAX_HIST   1024

#define PLACA_MAX   16
#define HORA_MAX    9

typedef struct {
    char placa[PLACA_MAX];
    char horaEntrada[HORA_MAX];
    char horaSalida[HORA_MAX];
} Auto;

typedef struct {
    Auto datos[MAX_PARKING];
    int tope;
} PilaAutos;

typedef struct {
    Auto datos[MAX_COLA];
    int frente;
    int fondo;
    int tam;
} ColaAutos;

typedef struct {
    Auto datos[MAX_HIST];
    int tam;
} HistorialAutos;

typedef struct {
    PilaAutos estacionamiento;
    PilaAutos aux;
    ColaAutos espera;
    HistorialAutos historial;
} SistemaParking;

// =====================
// Utilidades de lectura segura
// =====================

// Lee una línea desde stdin y elimina '\n'
static char *read_line(char *buf, size_t size) {
    if (!fgets(buf, (int)size, stdin)) {
        if (size > 0) buf[0] = '\0';
        return buf;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    return buf;
}

static void prompt_read(const char *prompt, char *buf, size_t size) {
    if (prompt) printf("%s", prompt);
    fflush(stdout);
    read_line(buf, size);
}

// =====================
// Constructores
// =====================

void Pila_init(PilaAutos *p) { p->tope = -1; }
void Cola_init(ColaAutos *c) { c->frente = 0; c->fondo = -1; c->tam = 0; }
void Historial_init(HistorialAutos *h) { h->tam = 0; }
void Sistema_init(SistemaParking *s) {
    Pila_init(&s->estacionamiento);
    Pila_init(&s->aux);
    Cola_init(&s->espera);
    Historial_init(&s->historial);
}

// =====================
// Operaciones básicas
// =====================

int Pila_push(PilaAutos *p, Auto a) {
    if (p->tope >= MAX_PARKING - 1) return 0;
    p->datos[++(p->tope)] = a;
    return 1;
}
int Pila_pop(PilaAutos *p, Auto *out) {
    if (p->tope < 0) return 0;
    if (out) *out = p->datos[p->tope];
    p->tope--;
    return 1;
}
int Pila_vacia(const PilaAutos *p) { return p->tope < 0; }
int Pila_llena(const PilaAutos *p) { return p->tope >= MAX_PARKING - 1; }

int Cola_enqueue(ColaAutos *c, Auto a) {
    if (c->tam >= MAX_COLA) return 0;
    c->fondo = (c->fondo + 1) % MAX_COLA;
    c->datos[c->fondo] = a;
    c->tam++;
    return 1;
}
int Cola_dequeue(ColaAutos *c, Auto *out) {
    if (c->tam <= 0) return 0;
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
// Funciones para Autos
// =====================

Auto Auto_con_entrada(const char *placa) {
    Auto a;
    if (placa && placa[0] != '\0') {
        strncpy(a.placa, placa, PLACA_MAX);
        a.placa[PLACA_MAX - 1] = '\0';
    } else {
        prompt_read("Ingrese placa: ", a.placa, PLACA_MAX);
    }
    prompt_read("Ingrese hora de entrada (HH:MM:SS): ", a.horaEntrada, HORA_MAX);
    a.horaSalida[0] = '\0';
    return a;
}

void Auto_set_salida(Auto *a) {
    if (!a) return;
    prompt_read("Ingrese hora de salida (HH:MM:SS): ", a->horaSalida, HORA_MAX);
}

// =====================
// Instancia global
// =====================

SistemaParking gSistema;

void InicializarSistema() { Sistema_init(&gSistema); }

// =====================
// Registro de entrada/salida
// =====================

typedef enum { REG_OK = 1, REG_LLENO = 0, REG_ERR = -1 } RegRes;

RegRes RegistrarEntradaInteractive(void) {
    char placa[PLACA_MAX];
    prompt_read("Ingrese placa del vehículo: ", placa, PLACA_MAX);
    Auto a = Auto_con_entrada(placa);
    if (!Pila_llena(&gSistema.estacionamiento)) {
        return Pila_push(&gSistema.estacionamiento, a) ? REG_OK : REG_ERR;
    }
    if (!Cola_llena(&gSistema.espera)) {
        return Cola_enqueue(&gSistema.espera, a) ? REG_OK : REG_ERR;
    }
    return REG_LLENO;
}

RegRes RegistrarEntrada(const char *placa) {
    Auto a = Auto_con_entrada(placa);
    if (!Pila_llena(&gSistema.estacionamiento)) {
        return Pila_push(&gSistema.estacionamiento, a) ? REG_OK : REG_ERR;
    }
    if (!Cola_llena(&gSistema.espera)) {
        return Cola_enqueue(&gSistema.espera, a) ? REG_OK : REG_ERR;
    }
    return REG_LLENO;
}

int Pila_buscar_por_placa(const PilaAutos *p, const char *placa) {
    for (int i = p->tope; i >= 0; --i) {
        if (strcmp(p->datos[i].placa, placa) == 0) return i;
    }
    return -1;
}

RegRes RegistrarSalida(const char *placa) {
    int idx = Pila_buscar_por_placa(&gSistema.estacionamiento, placa);
    if (idx < 0) return REG_ERR;

    Auto tmp;
    while (gSistema.estacionamiento.tope > idx) {
        Pila_pop(&gSistema.estacionamiento, &tmp);
        Pila_push(&gSistema.aux, tmp);
    }
    Auto destino;
    Pila_pop(&gSistema.estacionamiento, &destino);

    Auto_set_salida(&destino);
    Historial_agregar(&gSistema.historial, &destino);

    while (!Pila_vacia(&gSistema.aux)) {
        Pila_pop(&gSistema.aux, &tmp);
        Pila_push(&gSistema.estacionamiento, tmp);
    }
    if (!Cola_vacia(&gSistema.espera)) {
        Auto queEntra;
        Cola_dequeue(&gSistema.espera, &queEntra);
        Pila_push(&gSistema.estacionamiento, queEntra);
    }
    return REG_OK;
}

// =====================
// Visualización
// =====================

void VerEstacionamientoActual() {
    printf("\n--- Estacionamiento (tope=%d) ---\n", gSistema.estacionamiento.tope);
    for (int i = gSistema.estacionamiento.tope; i >= 0; --i) {
        printf("[%2d] Placa:%s  In:%s\n", i,
               gSistema.estacionamiento.datos[i].placa,
               gSistema.estacionamiento.datos[i].horaEntrada);
    }
}

void VerColaEspera() {
    printf("\n--- Cola de espera (tam=%d) ---\n", gSistema.espera.tam);
    for (int k = 0; k < gSistema.espera.tam; ++k) {
        int idx = (gSistema.espera.frente + k) % MAX_COLA;
        printf("(%2d) Placa:%s  In:%s\n", k,
               gSistema.espera.datos[idx].placa,
               gSistema.espera.datos[idx].horaEntrada);
    }
}

void VerHistorial() {
    printf("\n--- Historial (tam=%d) ---\n", gSistema.historial.tam);
    for (int i = 0; i < gSistema.historial.tam; ++i) {
        printf("%4d) Placa:%s  In:%s  Out:%s\n",
               i + 1,
               gSistema.historial.datos[i].placa,
               gSistema.historial.datos[i].horaEntrada,
               gSistema.historial.datos[i].horaSalida[0]
                   ? gSistema.historial.datos[i].horaSalida
                   : "--:--:--");
    }
}

// Fin del archivo


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
