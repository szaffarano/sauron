#ifndef _PILA_H_
#define _PILA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo_ejecucion {
   char* valor;
   struct nodo_ejecucion* ptAnt;
};

struct nodo_ejecucion* nuevoNodo ();
int push (struct nodo_ejecucion** pt, char*);
char* pop (struct nodo_ejecucion** pt);

#endif //_PILA_H_
