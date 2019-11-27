#ifndef _GENERADOR_H_
#define _GENERADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODO -1

#define OP_SUMA 0
#define OP_RESTA 1
#define OP_MAYOR 2
#define OP_IGUAL 3
#define OP_SENTENCIAS_IF 4 // la parte true del if
#define OP_CUERPO_IF 5
#define OP_POR 6
#define OP_FOR 7
#define OP_MENOR 8
#define OP_MENOR_IGUAL 9
#define OP_MAYOR_IGUAL 10
#define OP_IGUAL_IGUAL 11
#define OP_DISTINTO 12
#define OP_DIV 13
#define OP_SWITCH 14
#define OP_OPCIONES_SWITCH 15
#define OP_OPCION_SWITCH 16
#define OP_IF 17
#define OP_INICIALIZA_IF 18
#define OP_INICIALIZA_FOR 19
#define OP_SENT_SWITCH 20
#define OP_INIT_SWITCH 21
#define OP_SENTENCIAS 22
#define OP_PRINT 23
#define OP_NEGACION 24

#define STATE_FREE 0
#define STATE_BUSY 1

#define CANT_REGS 4
#define MAX_CANT_VARS 10

union tipo_operacion {
   int operacion;
   char* nombre;
};

struct nodo {
   int tipo;
   union tipo_operacion operacion;
   struct nodo* der;
   struct nodo* izq;
};

struct regsState {
   int state;
   char* name;
};

char* gMas(char*,char*);
char* gMenos(char*,char*);
char* gMayor(char*,char*);
char* gMayorIgual(char*,char*);
char* gDistinto(char*,char*);
char* gIgualIgual(char*,char*);
char* gMenor(char*,char*);
char* gMenorIgual(char*,char*);
char* gIgual(char*,char*);
char* gEndTrueIf(char*,char*);
char* gEndIf(char*,char*);
char* gPor(char*,char*);
char* gDiv(char*,char*);
char* gFor(char*,char*);
char* gSwitch(char*,char*);
char* gOpcionesSwitch(char*,char*);
char* gOpcionSwitch(char*,char*);
char* gInicializaIf(char*,char*);
char* gInicializaFor(char*,char*);
char* gInicializaSwitch(char*,char*);
char* gComparadorSwitch(char*,char*);
char* gPrint(char*,char*);
char* gFactor (char*,char*,char*);
char* gTermino (char*,char*,char*);
char* gNegacion(char*,char*);

char* gCompara(char*,char*,char*);

void initGenerator ();

int esNumero (char*);
int esRegistro (char*);
int esVariable (char*);

char* getNextFreeReg ();
void freeReg (char*);

char* getNextFreeVar (); // si no hay ninguna libre, crea una nueva
void freeVar (char*); // libera una variable para poder reutilizarla

struct nodo* crea_nodo (int, struct nodo* izq, struct nodo* der);
struct nodo* crea_hoja (char*, int tipo);
struct nodo* muestra_arbol (struct nodo* raiz);
struct nodo* cuenta_case (struct nodo* raiz);
struct nodo* genera_codigo (struct nodo* raiz);
void genera_salida();
void genera_cabecera();
void genera_data();

#endif //_GENERADOR_H_
