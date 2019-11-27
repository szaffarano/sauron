#ifndef _PROCESOS_H_
#define _PROCESOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <y.tab.h>
#include <yylex.h>

#define NOT_SET -1

void initProcesos (void);

int fromHexa (char*);
int fromBin (char*);
int fromOct (char*);
int pow (int,int);

int initId (int);
int addId (int);
int endId (int);
int initCteDec (int);
int addCteDec (int);
int endCteDec (int);
int PosCte (int);         // Posible Constante Estado = 2
int initPosCteHexa (int);
int addCteHexa (int);
int endCteHexa (int);
int initPosCteBin (int);
int addCteBin (int);
int endCteBin (int);
int initPosCteOct (int);
int addCteOct (int);
int endCteOct (int);
int addMas (int);
int addMenos (int);
int addPor (int);
int addDiv (int);
int addParentesisPunto (int); // Parentesis y Punto y Coma
int addLlaves (int);
int initMenorMayor (int);
int addMenorMayor (int);
int initIgual (int);          // Igual e Igual_Igual --> '=' y '=='
int addIgual (int);
int initDistinto (int);       // Distinto y Negacion --> '!=' y '!'
int addDist (int);
int nada (int);
int error (int);

#endif //_PROCESOS_H_
