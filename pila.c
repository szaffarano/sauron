#include <pila.h>

struct nodo_ejecucion* nuevoNodo () {
   struct nodo_ejecucion* aux = (struct nodo_ejecucion*)malloc(sizeof(struct nodo_ejecucion));  
   return aux;
}

int push (struct nodo_ejecucion** pt, char* v) {
  struct nodo_ejecucion* aux = nuevoNodo();
  if (aux == NULL)
     return -1;
  aux->valor = strdup(v);
  aux->ptAnt = *pt;
  *pt = aux;
  return 1;
}

char* pop (struct nodo_ejecucion** pt) {
    struct nodo_ejecucion* aux;
    char* valor;
    if (*pt != NULL) {
       aux = *pt;
       valor = strdup(aux->valor);
       *pt = (*pt)->ptAnt;
       free (aux);
       return valor;
    }
    return NULL;
}
