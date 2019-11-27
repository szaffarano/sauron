#include <yylex.h>
#include <y.tab.h>
#include <procesos.h>

// Variables globales
int fdArchivoFuente; 
int flagTerminal = 0;
int lineaLeida = 1;
char buff[MAX_BUFF];
int buffPos;
int yylvalue;

int nuevoEstado[13][19]= {12,11,10,1,1,2,1,3,F,F,F,F,3,3,1,0,1,F,F,
                         F,F,F,1,1,1,1,1,F,F,F,F,1,1,1,F,1,F,F,
                         E,E,E,5,4,3,E,3,F,F,F,F,3,3,E,F,6,F,F,
                         F,F,F,F,F,3,F,3,F,F,F,F,3,3,F,F,F,F,F,
                         E,E,E,E,E,7,7,7,E,E,E,E,7,7,E,E,E,E,E,
                         E,E,E,E,E,8,E,E,E,E,E,E,E,8,E,E,E,E,E,
                         E,E,E,E,E,9,E,E,E,E,E,E,9,9,E,E,E,E,E,
                         F,F,F,F,F,7,7,7,F,F,F,F,7,7,F,F,F,F,F,
                         F,F,F,F,F,8,F,F,F,F,F,F,F,8,F,F,F,F,F,
                         F,F,F,F,F,9,F,F,F,F,F,F,9,9,F,F,F,F,F,
                         F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,
                         F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,
                         F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F,F};


int (*proceso[13][19])(int);

void initProcesos(void) {

proceso[0][0] = initDistinto;
proceso[0][1] = initIgual;
proceso[0][2] = initMenorMayor;
proceso[0][3] = initId;
proceso[0][4] = initId;
proceso[0][5] = PosCte;
proceso[0][6] = initId;
proceso[0][7] = initCteDec;
proceso[0][8] = addMas;
proceso[0][9] = addMenos;
proceso[0][10] = addPor;
proceso[0][11] = addDiv;
proceso[0][12] = initCteDec;
proceso[0][13] = initCteDec;
proceso[0][14] = initId;
proceso[0][15] = nada;
proceso[0][16] = initId;
proceso[0][17] = addParentesisPunto;
proceso[0][18] = addLlaves;

proceso[1][0] = endId;
proceso[1][1] = endId;
proceso[1][2] = endId;
proceso[1][3] = addId;
proceso[1][4] = addId;
proceso[1][5] = addId;
proceso[1][6] = addId;
proceso[1][7] = addId;
proceso[1][8] = endId;
proceso[1][9] = endId;
proceso[1][10] = endId;
proceso[1][11] = endId;
proceso[1][12] = addId;
proceso[1][13] = addId;
proceso[1][14] = addId;
proceso[1][15] = endId;
proceso[1][16] = addId;
proceso[1][17] = endId;
proceso[1][18] = endId;

proceso[2][0] = endCteDec;
proceso[2][1] = endCteDec;
proceso[2][2] = endCteDec;
proceso[2][3] = initPosCteBin;
proceso[2][4] = initPosCteHexa;
proceso[2][5] = addCteDec;
proceso[2][6] = error;
proceso[2][7] = addCteDec;
proceso[2][8] = endCteDec;
proceso[2][9] = endCteDec;
proceso[2][10] = endCteDec;
proceso[2][11] = endCteDec;
proceso[2][12] = addCteDec;
proceso[2][13] = addCteDec;
proceso[2][14] = error;
proceso[2][15] = endCteDec;
proceso[2][16] = initPosCteOct;
proceso[2][17] = endCteDec;
proceso[2][18] = endCteDec;

proceso[3][0] = endCteDec;
proceso[3][1] = endCteDec;
proceso[3][2] = endCteDec;
proceso[3][3] = error;
proceso[3][4] = error;
proceso[3][5] = addCteDec;
proceso[3][6] = error;
proceso[3][7] = addCteDec;
proceso[3][8] = endCteDec;
proceso[3][9] = endCteDec;
proceso[3][10] = endCteDec;
proceso[3][11] = endCteDec;
proceso[3][12] = addCteDec;
proceso[3][13] = addCteDec;
proceso[3][14] = error;
proceso[3][15] = endCteDec;
proceso[3][16] = error;
proceso[3][17] = endCteDec;
proceso[3][18] = endCteDec;

proceso[4][0] = error;
proceso[4][1] = error;
proceso[4][2] = error;
proceso[4][3] = error;
proceso[4][4] = error;
proceso[4][5] = addCteHexa;
proceso[4][6] = addCteHexa;
proceso[4][7] = addCteHexa;
proceso[4][8] = error;
proceso[4][9] = error;
proceso[4][10] = error;
proceso[4][11] = error;
proceso[4][12] = addCteHexa;
proceso[4][13] = addCteHexa;
proceso[4][14] = error;
proceso[4][15] = error;
proceso[4][16] = error;
proceso[4][17] = error;
proceso[4][18] = error;

proceso[5][0] = error;
proceso[5][1] = error;
proceso[5][2] = error;
proceso[5][3] = error;
proceso[5][4] = error;
proceso[5][5] = addCteBin;
proceso[5][6] = error;
proceso[5][7] = error;
proceso[5][8] = error;
proceso[5][9] = error;
proceso[5][10] = error;
proceso[5][11] = error;
proceso[5][12] = error;
proceso[5][13] = addCteBin;
proceso[5][14] = error;
proceso[5][15] = error;
proceso[5][16] = error;
proceso[5][17] = error;
proceso[5][18] = error;

proceso[6][0] = error;
proceso[6][1] = error;
proceso[6][2] = error;
proceso[6][3] = error;
proceso[6][4] = error;
proceso[6][5] = addCteOct;
proceso[6][6] = error;
proceso[6][7] = error;
proceso[6][8] = error;
proceso[6][9] = error;
proceso[6][10] = error;
proceso[6][11] = error;
proceso[6][12] = addCteOct;
proceso[6][13] = addCteOct;
proceso[6][14] = error;
proceso[6][15] = error;
proceso[6][16] = error;
proceso[6][17] = error;
proceso[6][18] = error;

proceso[7][0] = endCteHexa;
proceso[7][1] = endCteHexa;
proceso[7][2] = endCteHexa;
proceso[7][3] = error;
proceso[7][4] = error;
proceso[7][5] = addCteHexa;
proceso[7][6] = addCteHexa;
proceso[7][7] = addCteHexa;
proceso[7][8] = endCteHexa;
proceso[7][9] = endCteHexa;
proceso[7][10] = endCteHexa;
proceso[7][11] = endCteHexa;
proceso[7][12] = addCteHexa;
proceso[7][13] = addCteHexa;
proceso[7][14] = error;
proceso[7][15] = endCteHexa;
proceso[7][16] = error;
proceso[7][17] = endCteHexa;
proceso[7][18] = endCteHexa;

proceso[8][0] = endCteBin;
proceso[8][1] = endCteBin;
proceso[8][2] = endCteBin;
proceso[8][3] = error;
proceso[8][4] = error;
proceso[8][5] = addCteBin;
proceso[8][6] = error;
proceso[8][7] = error;
proceso[8][8] = endCteBin;
proceso[8][9] = endCteBin;
proceso[8][10] = endCteBin;
proceso[8][11] = endCteBin;
proceso[8][12] = error;
proceso[8][13] = addCteBin;
proceso[8][14] = error;
proceso[8][15] = endCteBin;
proceso[8][16] = error;
proceso[8][17] = endCteBin;
proceso[8][18] = endCteBin;

proceso[9][0] = endCteOct;
proceso[9][1] = endCteOct;
proceso[9][2] = endCteOct;
proceso[9][3] = error;
proceso[9][4] = error;
proceso[9][5] = addCteOct;
proceso[9][6] = error;
proceso[9][7] = endCteOct;
proceso[9][8] = endCteOct;
proceso[9][9] = endCteOct;
proceso[9][10] = endCteOct;
proceso[9][11] = endCteOct;
proceso[9][12] = addCteOct;
proceso[9][13] = addCteOct;
proceso[9][14] = error;
proceso[9][15] = endCteOct;
proceso[9][16] = error;
proceso[9][17] = endCteOct;
proceso[9][18] = endCteOct;

proceso[10][0] = addMenorMayor;
proceso[10][1] = addMenorMayor;
proceso[10][2] = addMenorMayor;
proceso[10][3] = addMenorMayor;
proceso[10][4] = addMenorMayor;
proceso[10][5] = addMenorMayor;
proceso[10][6] = addMenorMayor;
proceso[10][7] = addMenorMayor;
proceso[10][8] = addMenorMayor;
proceso[10][9] = addMenorMayor;
proceso[10][10] = addMenorMayor;
proceso[10][11] = addMenorMayor;
proceso[10][12] = addMenorMayor;
proceso[10][13] = addMenorMayor;
proceso[10][14] = addMenorMayor;
proceso[10][15] = addMenorMayor;
proceso[10][16] = addMenorMayor;
proceso[10][17] = addMenorMayor;
proceso[10][18] = addMenorMayor;

proceso[11][0] = addIgual;
proceso[11][1] = addIgual;
proceso[11][2] = addIgual;
proceso[11][3] = addIgual;
proceso[11][4] = addIgual;
proceso[11][5] = addIgual;
proceso[11][6] = addIgual;
proceso[11][7] = addIgual;
proceso[11][8] = addIgual;
proceso[11][9] = addIgual;
proceso[11][10] = addIgual;
proceso[11][11] = addIgual;
proceso[11][12] = addIgual;
proceso[11][13] = addIgual;
proceso[11][14] = addIgual;
proceso[11][15] = addIgual;
proceso[11][16] = addIgual;
proceso[11][17] = addIgual;
proceso[11][18] = addIgual;

proceso[12][0] = addDist;
proceso[12][1] = addDist;
proceso[12][2] = addDist;
proceso[12][3] = addDist;
proceso[12][4] = addDist;
proceso[12][5] = addDist;
proceso[12][6] = addDist;
proceso[12][7] = addDist;
proceso[12][8] = addDist;
proceso[12][9] = addDist;
proceso[12][10] = addDist;
proceso[12][11] = addDist;
proceso[12][12] = addDist;
proceso[12][13] = addDist;
proceso[12][14] = addDist;
proceso[12][15] = addDist;
proceso[12][16] = addDist;
proceso[12][17] = addDist;
proceso[12][18] = addDist;

}

char* palabrasReservadas[] = {"main","endmain","if","else","case","switch","for","break","print"};

char* tokenNames[] = { "ID", "MAS", "MENOS", "POR", "DIVIDIDO", "CONST_DEC", "CONST_HEX", "CONST_BIN", "CONST_OCT", "PARENTESIS_ABIERTA", "PARENTESIS_CERRADA", "PUNTO_COMA", "LLAVES_ABIERTA", "LLAVES_CERRADA", "MENOR", "MAYOR", "MENOR_IGUAL", "MAYOR_IGUAL", "IGUAL", "IGUAL_IGUAL", "DISTINTO", "NEGACION", "PALABRA_MAIN", "PALABRA_ENDMAIN", "PALABRA_IF", "PALABRA_ELSE", "PALABRA_CASE", "PALABRA_SWITCH", "PALABRA_FOR", "PALABRA_BREAK","DOS_PUNTOS","PALABRA_PRINT"};

int yylex(void) {
    static int estado;
    static int ultRead;
    static char terminal;
    static int evento;
    static int token;
    static int lexerOn;
    estado = 0;
    lexerOn = 1;
    if (flagTerminal == 0)
       ultRead = read(fdArchivoFuente,&terminal,sizeof(terminal));
    else
       flagTerminal = 0;
    if (evento == ERROR)
       evento = 0;
    while ( (estado != ESTADO_FINAL) && (ultRead > 0) && (evento != ERROR)) {
        evento = getEvento(terminal);
        if (evento != ERROR && evento != COMENTARIO && lexerOn != 0) {
           token = (*proceso[estado][evento])(terminal);
           if (token != ERROR) {
              estado = nuevoEstado[estado][evento];
              if (token == 0) {
                 ultRead = read(fdArchivoFuente,&terminal,sizeof(terminal));
              }
           }
           else
              evento = ERROR;
        }
        else {
           if ( evento == COMENTARIO ) {
              if ( lexerOn == 0 )
                lexerOn = 1;
              else
                lexerOn = 0;
           }
           ultRead = read(fdArchivoFuente,&terminal,sizeof(terminal));
        }
    }
    if (ultRead <= 0)
       return FIN_ARCHIVO;
    if (evento == ERROR)   
       return ERROR;
    return token;
}

// Funcion que devuelve la posicion de la columna en las matrices
int getEvento(char terminal) {
   if (terminal == '!')
      return 0;
   if (terminal == '=')
      return 1;
   if (terminal == '<' || terminal == '>')
      return 2;
   if (terminal == 'b')
      return 3;
   if (terminal == 'x')
      return 4;
   if (terminal == '0')
      return 5;
   if (terminal >= 'A' && terminal <= 'F')
      return 6;
   if (terminal == '8' || terminal == '9')
      return 7;
   if (terminal == '+')
      return 8;
   if (terminal == '-')
      return 9;
   if (terminal == '*')
      return 10;
   if (terminal == '/')
      return 11;
   if (terminal >= '2' && terminal <= '7')
      return 12;
   if (terminal == '1')
      return 13;
   if ((terminal == 'a') || (terminal >= 'c' && terminal <= 'n') ||
       (terminal >= 'p' && terminal <= 'w') || (terminal == 'y') || 
       (terminal == 'z') || (terminal >= 'G' && terminal <= 'Z'))
       return 14;
   if (terminal == '\t' || terminal == '\n' || terminal == ' ')
      return 15;
   if (terminal == 'o')
      return 16;
   if (terminal == '(' || terminal == ')' || terminal == ';' || terminal == ':')
      return 17;
   if (terminal == '{' || terminal == '}')
      return 18;
   if (terminal == '#')
      return COMENTARIO;
   // Si el terminal leido no es ninguno de los anteriores
   return ERROR;     
}
