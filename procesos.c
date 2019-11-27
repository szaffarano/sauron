#include <procesos.h>
#include <symbolTable.h>
#include <y.tab.h>
#include <yylex.h>

extern int fdArchivoFuente;
extern int flagTerminal;
extern int lineaLeida;
extern char* palabrasReservadas[];
extern char buff[MAX_BUFF];
extern int buffPos;
extern int yylvalue;

char buffTemporal[MAX_BUFF];

int initId (int terminal) {
   yylvalue = NOT_SET;
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = terminal;
   return 0;    
}

int addId (int terminal) {
   if ( buffPos < MAX_LENGTH_ID ) {
      buff[buffPos++] = terminal;
   }
   return 0;    
}

int endId (int terminal) {
   int encontroIgualdad = 0,palabraActual = 0;
   flagTerminal = 1;
   buff[buffPos] = '\0';
      
   // Reconocimiento de palabras reservadas
   while (palabraActual != 9 && !encontroIgualdad) {
      if ( strcmp (palabrasReservadas[palabraActual], buff) == 0 ) {
         encontroIgualdad = 1;
      }
      palabraActual++;
   }

   if (encontroIgualdad) {
      yylvalue = NOT_SET;
      switch (palabraActual-1) {
         case 0:
            return PALABRA_MAIN;
         case 1:
            return PALABRA_ENDMAIN;
         case 2:
            return PALABRA_IF;
         case 3:
            return PALABRA_ELSE;
         case 4:
            return PALABRA_CASE;
         case 5:
            return PALABRA_SWITCH;
         case 6:
            return PALABRA_FOR;
         case 7:
            return PALABRA_BREAK;
         case 8:
	    return PALABRA_PRINT;
      };
   }

   sprintf (buffTemporal, "v_%s", buff);
   yylvalue = addTableId (buffTemporal);
   return ID; 
}

int initCteDec (int terminal) {
   yylvalue = NOT_SET;
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = terminal;
   return 0;    
}

int addCteDec (int terminal) {
   if ( buffPos < MAX_LENGTH_CTE ) {
      buff[buffPos++] = terminal;
   }
   return 0;    
}

int endCteDec (int terminal) {
   flagTerminal = 1;
   buff[buffPos] = '\0';
   yylvalue = addTableConst (atoi (buff));
   return CONST_DEC; 
}

int PosCte (int terminal) {
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = terminal;
   return 0;
}

int initPosCteHexa (int terminal) {
   yylvalue = NOT_SET;
   buff[buffPos++] = terminal;
   return 0;
}

int addCteHexa (int terminal) {
   if ( buffPos < MAX_LENGTH_CTE_HEX ) {
      buff[buffPos++] = terminal;
   }
   return 0;    
}

int endCteHexa (int terminal) {
   flagTerminal = 1;
   yylvalue = addTableConst (fromHexa (buff));
   return CONST_HEX; 
}

int initPosCteBin (int terminal) {
   yylvalue = NOT_SET;
   buff[buffPos++] = terminal;
   return 0;
}

int addCteBin (int terminal) {
   if ( buffPos < MAX_LENGTH_CTE_BIN ) {
      buff[buffPos++] = terminal;
   }
   return 0;    
}

int endCteBin (int terminal) {
   flagTerminal = 1;
   yylvalue = addTableConst (fromBin (buff));
   return CONST_BIN;
}

int initPosCteOct (int terminal) {
   yylvalue = NOT_SET;
   buff[buffPos++] = terminal;
   return 0;
}

int addCteOct (int terminal) {
   if ( buffPos < MAX_LENGTH_CTE_OCT ) {
      buff[buffPos++] = (char)terminal;
   }
   return 0;    
}

int endCteOct (int terminal) {
   flagTerminal = 1;
   yylvalue = addTableConst (fromOct (buff));
   return CONST_OCT;
}

int addParentesisPunto (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   if (terminal == '(')
      return PARENTESIS_ABIERTA;
   if (terminal == ')')
      return PARENTESIS_CERRADA;
   if (terminal == ';')
      return PUNTO_COMA;
   if (terminal == ':')   
      return DOS_PUNTOS;
   return ERROR;
}

int addLlaves (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   if (terminal == '{')
      return LLAVES_ABIERTA;
   if (terminal == '}')
      return LLAVES_CERRADA;
   return ERROR;
}

int initMenorMayor (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 1;
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = (char)terminal;
   return 0;
}

int addMenorMayor (int terminal) {
   yylvalue = NOT_SET;
   if (terminal != '=') {
      flagTerminal = 1;
      if (buff[0] == '<')
         return MENOR;
      if (buff[0] == '>')
         return MAYOR;
   }
   else {
      flagTerminal = 0;
      if (buff[0] == '<')
         return MENOR_IGUAL;
      if (buff[0] == '>')
         return MAYOR_IGUAL;
   }
   return ERROR;
}

int initIgual (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 1;
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = terminal;
   return 0;
}

int addIgual (int terminal) {
   yylvalue = NOT_SET;
   if (terminal != '=') {
      flagTerminal = 1;
      return IGUAL;
   }
   else {
      flagTerminal = 0;
      return IGUAL_IGUAL;
   }
}

int initDistinto  (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 1;
   memset (buff, '\0', sizeof (buff));
   buffPos = 0;
   buff[buffPos++] = terminal;
   return 0;
}

int addDist (int terminal) {
   if (terminal != '=') {
      flagTerminal = 1;
      return NEGACION;
   }
   else {
      flagTerminal = 0;
      return DISTINTO;
   }
}

int addMas (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   return MAS; 
}

int addMenos (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   return MENOS;
}

int addPor (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   return POR; 
}

int addDiv (int terminal) {
   yylvalue = NOT_SET;
   flagTerminal = 0;
   return DIVIDIDO;
}

int nada (int terminal) {
   if (terminal == '\n')
      lineaLeida++;
   return 0;
}

int error (int terminal) {
    return ERROR;
}

int fromHexa (char* str) {
   int ret = 0;
   int i = 2;
   int len = strlen (str);
   for (; i < len; i++) {
      switch (str[i]) {
         case 'A':
         case 'B':
         case 'C':
         case 'D':
         case 'E':
         case 'F':
            ret += pow (len - i - 1, 16) * ( str[i] - 55 );
            break;
         default:
            ret += pow (len - i - 1, 16) * ( str[i] - 48 );
      } 
   }
   return ret;
}

int fromBin (char* str) {
   int ret = 0;
   int i = 2;
   int len = strlen (str);
   for (; i < len; i++)
      ret += pow (len - i - 1, 2) * ( str[i] - 48 );
   return ret;
}

int fromOct (char* str) {
   int ret = 0;
   int i = 2;
   int len = strlen (str);
   for (; i < len; i++)
      ret += pow (len - i - 1, 8) * ( str[i] - 48 );
   return ret;
}

int pow (int exp, int base) {
   int ret = 1;
   int i = 0;
   for ( i = 0; i < exp; i++)
      ret *= base;
   return ret;
}
