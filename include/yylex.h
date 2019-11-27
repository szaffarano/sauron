#ifndef _YYLEX_H_
#define _YYLEX_H_

#include <procesos.h>

int yylex(void);
int getEvento(char);

#define ERROR_ARGUMENTOS "Error en los argumentos!\nModo de uso: %s archivo a compilar \n"

// Constantes para el manejo de errores
#define FIN_ARCHIVO        -1
#define ERROR              -2
#define COMENTARIO         -3
#define ESTADO_FINAL       13
#define F                  13
#define E                  14
#define MAX_BUFF           255
#define MAX_LENGTH_ID      15
#define MAX_LENGTH_CTE     12
#define MAX_LENGTH_CTE_HEX 8
#define MAX_LENGTH_CTE_BIN 32
#define MAX_LENGTH_CTE_OCT 12
#define OK                 1

#endif //_YYLEX_H_
