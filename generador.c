#include <generador.h>
#include <pila.h>
#include <symbolTable.h>
#include <stdlib.h>

extern int countSymbols;
extern int countSwitch;
struct nodo_ejecucion *ptPila = NULL;
struct nodo_ejecucion *ptIf = NULL;
static int labelCountIf;
static int labelCountFor;
static int labelCountSwitch;
static int varsCount;

FILE* fdArchOut;

char* idSwitch = NULL;

char* (*genera[25])(char*,char*);
char* nombreOperacion[25];

struct regsState regsDescriptors[CANT_REGS];
struct regsState varsDescriptors[MAX_CANT_VARS];

void initGenerator () {
   genera[OP_SUMA] = gMas;
   genera[OP_RESTA] = gMenos;
   genera[OP_MAYOR] = gMayor;
   genera[OP_MAYOR_IGUAL] = gMayorIgual;
   genera[OP_DISTINTO] = gDistinto;
   genera[OP_IGUAL_IGUAL] = gIgualIgual;
   genera[OP_MENOR] = gMenor;
   genera[OP_MENOR_IGUAL] = gMenorIgual;
   genera[OP_IGUAL] = gIgual;
   genera[OP_SENTENCIAS_IF] = gEndTrueIf;
   genera[OP_CUERPO_IF] = gEndIf;
   genera[OP_POR] = gPor;
   genera[OP_DIV] = gDiv;
   genera[OP_FOR] = gFor;
   genera[OP_SWITCH] = gSwitch;
   genera[OP_OPCIONES_SWITCH] = gOpcionesSwitch;
   genera[OP_OPCION_SWITCH] = gOpcionSwitch;
   genera[OP_INICIALIZA_IF] = gInicializaIf;
   genera[OP_INICIALIZA_FOR] = gInicializaFor;
   genera[OP_SENT_SWITCH] = gComparadorSwitch;
   genera[OP_INIT_SWITCH] = gInicializaSwitch;
   genera[OP_PRINT] = gPrint;
   genera[OP_NEGACION] = gNegacion;

   nombreOperacion[OP_SUMA] = strdup ("+");
   nombreOperacion[OP_RESTA] = strdup ("-");
   nombreOperacion[OP_MAYOR] = strdup (">");
   nombreOperacion[OP_IGUAL] = strdup ("=");
   nombreOperacion[OP_IF] = strdup ("IF");
   nombreOperacion[OP_DIV] = strdup ("/");
   nombreOperacion[OP_POR] = strdup ("*");
   nombreOperacion[OP_SENTENCIAS] = strdup ("sen");
   nombreOperacion[OP_MENOR] = strdup ("<");
   nombreOperacion[OP_MENOR_IGUAL] = strdup ("<=");
   nombreOperacion[OP_MAYOR_IGUAL] = strdup (">=");
   nombreOperacion[OP_IGUAL_IGUAL] = strdup ("==");
   nombreOperacion[OP_DISTINTO] = strdup ("!=");
   nombreOperacion[OP_CUERPO_IF] = strdup ("cuerpo_if");
   nombreOperacion[OP_SWITCH] = strdup ("switch");
   nombreOperacion[OP_OPCIONES_SWITCH] = strdup ("opciones_switch");
   nombreOperacion[OP_OPCION_SWITCH] = strdup ("opcion_switch");
   nombreOperacion[OP_FOR] = strdup ("for");
   nombreOperacion[OP_SENTENCIAS_IF] = strdup ("sen_if");
   nombreOperacion[OP_INICIALIZA_IF] = strdup ("ini_if");
   nombreOperacion[OP_INICIALIZA_FOR] = strdup ("ini_for");
   nombreOperacion[OP_INIT_SWITCH] = strdup ("init_switch");
   nombreOperacion[OP_SENT_SWITCH] = strdup ("sent_switch");
   nombreOperacion[OP_PRINT] = strdup ("print");
   nombreOperacion[OP_NEGACION] = strdup ("negacion");

   regsDescriptors[0].name = strdup ("%eax");
   regsDescriptors[0].state = STATE_FREE;
   regsDescriptors[1].name = strdup ("%ebx");
   regsDescriptors[1].state = STATE_FREE;
   regsDescriptors[2].name = strdup ("%ecx");
   regsDescriptors[2].state = STATE_FREE;
   regsDescriptors[3].name = strdup ("%edx");
   regsDescriptors[3].state = STATE_FREE;
}

char* getNextFreeReg () {
   int i = 0;

   for (; i < CANT_REGS; i++) {
      if ( regsDescriptors[i].state == STATE_FREE ) {
         regsDescriptors[i].state = STATE_BUSY;
         return regsDescriptors[i].name;
      }
   } 
   return NULL;
}

void freeReg (char* regTmp) {
   int i = 0;
   for (; i < CANT_REGS; i++) {
      if ( strcmp (regsDescriptors[i].name, regTmp) == 0 ) {
         regsDescriptors[i].state = STATE_FREE;
         break;
      } 
   } 
}

char* getNextFreeVar () {
   int i = 0;
   char buffTemp[10];

   if ( varsCount == MAX_CANT_VARS )
      return NULL; // podriamos reallocar el arreglo para aceptar mas variables

   for (; i < varsCount; i++) {
      if ( varsDescriptors[i].state == STATE_FREE ) {
         varsDescriptors[i].state = STATE_BUSY;
         return varsDescriptors[i].name;
      }
   } 
   sprintf (buffTemp, "v_number%d", ++varsCount);
   varsDescriptors[i].name = strdup ( buffTemp );
   varsDescriptors[i].state = STATE_BUSY;
   return varsDescriptors[i].name;
}

void freeVar (char* regTmp) {
   int i = 0;
   for (; i < varsCount; i++) {
      if ( strcmp (varsDescriptors[i].name, regTmp) == 0 ) {
         varsDescriptors[i].state = STATE_FREE;
         break;
      } 
   } 
}

struct nodo* crea_nodo (int op, struct nodo* izq, struct nodo* der) {
   struct nodo* aux = (struct nodo*) malloc ( sizeof (struct nodo) );

   if ( aux == NULL )
      return aux;

   #ifdef DEBUG
      if ( der == NULL )
         printf ("der es nulo en %d\n", op);
      if ( izq == NULL )
         printf ("izq es nulo en %d\n", op);
   #endif

   fflush (fdArchOut);
   aux->der = der;
   aux->izq = izq;

   aux->operacion.operacion = op;
   aux->tipo = NODO;
 
   return aux;
}

struct nodo* crea_hoja (char* nombre, int tipo) {
   struct nodo* aux = (struct nodo*) malloc ( sizeof (struct nodo) );

   if ( aux == NULL )
      return aux;

   aux->der = NULL;
   aux->izq = NULL;
   aux->operacion.nombre = strdup (nombre);
   aux->tipo = tipo;

   return aux; 
}

struct nodo* muestra_arbol (struct nodo* raiz) {
   static struct nodo* cab = NULL;
   if (raiz != NULL) {
      cab = muestra_arbol (raiz->izq);
      cab = muestra_arbol (raiz->der);
      if ( raiz->tipo == NODO ) {
         printf ("%s ", nombreOperacion [raiz->operacion.operacion]);
      } else
         printf ("%s ", raiz->operacion.nombre);
   }
   return cab;
}

struct nodo* cuenta_case (struct nodo* raiz) {
   static struct nodo* cab = NULL;
   if (raiz != NULL && raiz->operacion.operacion != OP_SWITCH ) {
      cab = cuenta_case (raiz->izq);
      cab = cuenta_case (raiz->der);
      if ( raiz->operacion.operacion == OP_OPCION_SWITCH )
         countSwitch++;
   }
   return cab;
}

struct nodo* genera_codigo (struct nodo* raiz) {
   static char* op1;
   static char* op2;
   static char result[255];
   static struct nodo* cab = NULL;
   if (raiz != NULL) {
      cab = genera_codigo (raiz->izq);
      cab = genera_codigo (raiz->der);
      if ( raiz->tipo != NODO ) {
         push (&ptPila, raiz->operacion.nombre);
      /**
       ** OP_SENTENCIAS es un nodo del cual "cuelgan" otras sentencias
       ** pero que no es nada en si mismo, por lo que se debe obviar
       ** igual que el cuerpo del IF o del SWITCH
       */
      } else if ( raiz->operacion.operacion != OP_SENTENCIAS 
                  && raiz->operacion.operacion != OP_IF ) {
         op1 = pop (&ptPila); 
         op2 = pop (&ptPila); 
         if ( (op1 != NULL && op2 != NULL) || 
              raiz->operacion.operacion == OP_INICIALIZA_IF || 
              raiz->operacion.operacion == OP_SENTENCIAS_IF || 
              raiz->operacion.operacion == OP_CUERPO_IF || 
              raiz->operacion.operacion == OP_INICIALIZA_FOR || 
              raiz->operacion.operacion == OP_OPCION_SWITCH || 
              raiz->operacion.operacion == OP_SENT_SWITCH || 
              raiz->operacion.operacion == OP_NEGACION || 
              raiz->operacion.operacion == OP_INIT_SWITCH ) 
         {
            #ifdef DEBUG
               printf ("Operacion: %s %s %s\n", op2, nombreOperacion[raiz->operacion.operacion], op1);
            #endif
            strcpy( result, (*genera[raiz->operacion.operacion])(op2, op1) );  
            fprintf (fdArchOut, "\n");
            if ( result != NULL )
               push(&ptPila, result);   
         } else {
            printf ("\noperacion rechazada: %d\n", raiz->operacion.operacion);
         } 
      } 
   }
   return cab;
}

void genera_data() {
   int i;

   // Generacion de las variables en assembler

   fprintf (fdArchOut, "//Declaracion de variables\n");
   fprintf(fdArchOut,".data\n");

   for (i = 0 ; i < countSymbols ; i++) {
      if ( symbols[i][0] == 'v' ) {
         fprintf(fdArchOut,"%s:\n",symbols[i]);
         fprintf(fdArchOut,"   .long 0\n");
      }
   }

   for (i = 0 ; i < varsCount ; i++) {
      fprintf(fdArchOut,"%s:\n",varsDescriptors[i].name);
      fprintf(fdArchOut,"   .long 0\n");
   }

   fprintf(fdArchOut,"newline:\n");
   fprintf(fdArchOut,".string \"\\n\"\n");
   fprintf(fdArchOut,"buff:\n");
   fprintf(fdArchOut,".string \"             \\0\"\n");
}

void genera_cabecera() {

   fprintf(fdArchOut,".text\n");
   fprintf(fdArchOut,".globl _start\n");
   fprintf(fdArchOut,"\n// Macro write: (fd) donde escribir, (buff) que escribir, (len) longitud\n");
   fprintf(fdArchOut,".macro write fd, buff, len\n");
   // como mando la longitud en %eax, es lo primero que hay que setear
   fprintf(fdArchOut,"movl \\len,%%edx\n"); 
   fprintf(fdArchOut,"movl $4,%%eax\n");
   fprintf(fdArchOut,"movl \\fd,%%ebx\n");
   fprintf(fdArchOut,"movl \\buff, %%ecx\n");
   fprintf(fdArchOut,"int  $0x80\n");
   fprintf(fdArchOut,"movl $4,%%eax\n");
   fprintf(fdArchOut,"movl \\fd,%%ebx\n");
   fprintf(fdArchOut,"movl $10, %%ecx\n");
   fprintf(fdArchOut,"movl $2,%%edx\n");
   fprintf(fdArchOut,"int  $0x80\n");
   fprintf(fdArchOut,".endm\n\n");

   fprintf(fdArchOut,"_start:\n");
   fprintf (fdArchOut, "\n");
}

void genera_salida () {
   fprintf (fdArchOut, "//llamada a la sys call exit\n");
   fprintf (fdArchOut, "movl $1, %%eax\n");
   fprintf (fdArchOut, "movl $0, %%ebx\n");
   fprintf (fdArchOut, "int $0x80\n");
}

char* gMas(char* op1, char* op2) {
   return gTermino ("addl", op1, op2);
}

char* gMenos(char* op1,char* op2) {
   return gTermino ("subl", op1, op2);
}

char* gInicializaIf(char* op1,char* op2) {
   char buffTemp[20];
   sprintf (buffTemp, "endIf%d", labelCountIf);
   push (&ptIf, buffTemp);
   sprintf (buffTemp, "else%d", labelCountIf);
   push (&ptIf, buffTemp);
   sprintf (buffTemp, "endIf%d", labelCountIf);
   push (&ptIf, buffTemp);
   sprintf (buffTemp, "else%d", labelCountIf++);
   push (&ptIf, buffTemp);
   return (char*) "inicializaIf";
}

char* gInicializaFor(char* op1,char* op2) {
   char buffTemp[20];
   sprintf (buffTemp, "begFor%d", labelCountFor);
   fprintf (fdArchOut, "%s:\n", buffTemp);

   sprintf (buffTemp, "endFor%d", labelCountFor);
   push (&ptIf, buffTemp);

   sprintf (buffTemp, "begFor%d", labelCountFor);
   push (&ptIf, buffTemp);

   sprintf (buffTemp, "endFor%d", labelCountFor++);
   push (&ptIf, buffTemp);

   return (char*) "inicializaFor";
}

char* gIgual(char* op1,char* op2) {
   char* regAux = NULL;
   if ( strcmp (op1, op2) != 0 ) {
      // si es un numero va con '$'
      // agregado: si es numero negativo!!!!!!!!!
      if ( (op2[0] >= '0' && op2[0] <= '9') || op2[0] == '-' ) { 
         fprintf (fdArchOut, "movl $%s, %s\n", op2, op1);
         regAux = strdup (" ");
      } else if ( op2[0] != '%' ) { // si es una variable en memoria
         if ( op1[0] == '%' ) {
            fprintf (fdArchOut, "movl %s, %s\n", op2, op1);
         } else {
            regAux = getNextFreeReg ();
            if ( regAux == NULL ) {
               fprintf (fdArchOut, "pushl %%eax\n");
               fprintf (fdArchOut, "movl %s, %%eax\n", op2);
               fprintf (fdArchOut, "movl %%eax, %s\n", op1);
               fprintf (fdArchOut, "popl %%eax\n");
            } else {
               fprintf (fdArchOut, "movl %s, %s\n", op2, regAux);
               fprintf (fdArchOut, "movl %s, %s\n", regAux, op1);
               freeReg (regAux);
            }
         }
      } else {
         fprintf (fdArchOut, "movl %s, %s\n", op2, op1);
      }
   }
   if ( op2[0] == '%' )
      freeReg (op2);
   else
      freeVar (op2);
   if ( regAux != NULL ) { // ???
      if ( op2[0] == '%' )
         freeReg (regAux);
      else
         freeVar (regAux);
   }
   return op1;
}

char* gEndTrueIf(char* op1,char* op2) {
   char* buffTemp;
   buffTemp = pop (&ptIf);
   fprintf (fdArchOut, "jmp %s\n", buffTemp);
   free (buffTemp);
   buffTemp = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n", buffTemp);
   free (buffTemp);
   return (char*)"hola";
}

char* gEndIf(char* op1,char* op2) {
   char* buffTemp;
   buffTemp = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n", buffTemp);
   free (buffTemp);
   return (char*)"endIf";
}

char* gPor(char* op1,char* op2) {
   return gFactor (op1, op2, "fimull");
}

char* gDiv(char* op1,char* op2) {
   return gFactor (op1, op2, "fidivl");
}

char* gFor(char* op1,char* op2) {
   char* labelFor = pop (&ptIf);
   fprintf (fdArchOut, "jmp %s\n", labelFor);
   free (labelFor);
   labelFor = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n", labelFor);
   free (labelFor);
   return (char*)"gFor";
}

char* gSwitch(char* op1,char* op2) {
   char* label;
   label = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n", label);
   free (label);
   label = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n",label);
   return (char*)"gSwitch";
}

char* gOpcionesSwitch(char* op1,char* op2) {
   return (char*)"gOpcionesSwitch";
}

char* gOpcionSwitch(char* op1,char* op2) {
   char* label;
   label = pop (&ptIf);
   fprintf (fdArchOut, "jmp %s\n", label);
   free (label);
   return (char*)"gOpcionSwitch";
}

int isBusy (char* nameReg) {
   int i = 0;

   for (; i < CANT_REGS; i++) {
      if ( strcmp (regsDescriptors[i].name, nameReg) == 0 ) {
         return (regsDescriptors[i].state == STATE_BUSY);
      }
   } 
   return 0;
}

char* gMayor(char* op1,char* op2) {
   // salta si es menor igual
   return gCompara (op1, op2, "jge");
}


char* gMenor(char* op1,char* op2) {
   // salta si es mayor igual
   return gCompara (op1, op2, "jle");
}

char* gMayorIgual(char* op1,char* op2) {
   // salta si es menor
   return gCompara (op1, op2, "jg");
}

char* gMenorIgual(char* op1,char* op2) {
   // salta si es mayor
   return gCompara (op1, op2, "jl");
}

char* gIgualIgual(char* op1,char* op2) {
   // salta si es distinto
   return gCompara (op1, op2, "jne");
}

char* gDistinto(char* op1,char* op2) {
   // salta si es igual
   return gCompara (op1, op2, "je");
}

char* gCompara (char* op1, char* op2, char* comparator) {
   char* label;
   label = pop (&ptIf);
   fprintf ( fdArchOut, "// Condicion %s\n",comparator );
   fprintf ( fdArchOut, "pushl %%eax\n" );
   fprintf ( fdArchOut, "pushl %%ebx\n" );

   if ( op1[0] != '%' && op1[0] != 'v' )  // si no es un registro y una variable
      fprintf ( fdArchOut, "movl $%s,%%eax\n", op1 );
   else
      if ( op1[1] != 'e' && op1[2] != 'x' )  // No es %eax
        fprintf ( fdArchOut, "movl %s,%%eax\n", op1 );

   if ( op2[0] != '%' && op2[0] != 'v' )  // si no es un registro y una variable
      fprintf ( fdArchOut, "movl $%s,%%ebx\n", op2 );
   else
      if ( op2[1] != 'e' && op2[2] != 'x' )  // No es %ebx
          fprintf ( fdArchOut, "movl %s,%%ebx\n", op2 );
    
   fprintf ( fdArchOut, "cmpl %%eax,%%ebx\n" );

   fprintf ( fdArchOut, "popl %%ebx\n");
   fprintf ( fdArchOut, "popl %%eax\n");

   fprintf ( fdArchOut, "%s %s",comparator, label);
   
   free (label);
   return comparator;
}

char* gInicializaSwitch(char* op1,char* op2) {
   char buffTemp[20];
   int i;
   int cantSwitch = atoi (op2);
   sprintf ( buffTemp, "endSwitch%d", labelCountSwitch);
   push (&ptIf, buffTemp); 
   if ( idSwitch != NULL )
      free ( idSwitch );
   idSwitch = strdup ( op1 );

   sprintf (buffTemp, "opcSwitch_%d_%d", labelCountSwitch, cantSwitch);
   push (&ptIf, buffTemp);

   for ( i = cantSwitch; i > 0; i-- ) {
      sprintf (buffTemp, "endSwitch%d", labelCountSwitch );
      push (&ptIf, buffTemp);
      sprintf (buffTemp, "opcSwitch_%d_%d\n", labelCountSwitch, i);
      push (&ptIf, buffTemp);
      sprintf (buffTemp, "opcSwitch_%d_%d", labelCountSwitch, i-1);
      push (&ptIf, buffTemp);
   }
   labelCountSwitch++;
   return (char*) "InicializaSwitch";
}

char* gComparadorSwitch(char* op1,char* op2) {
   char* buffTemp;
   buffTemp = pop (&ptIf);
   fprintf (fdArchOut, "%s:\n", buffTemp);
   return gIgualIgual (idSwitch, op2);
}

char* gPrint(char* op1,char* op2) {
   fprintf (fdArchOut, "// Print\n");

   fprintf (fdArchOut, "pushl %%eax\n");

   fprintf (fdArchOut, "pushl $buff\n");

   if ( op2[0] == 'v' || op2[0] == '%' )
      fprintf (fdArchOut, "pushl %s\n", op2);
   else
      fprintf (fdArchOut, "pushl $%s\n", op2);

   fprintf (fdArchOut, "call itoa\n");
   fprintf (fdArchOut, "write $1, $buff, %%eax\n");

   fprintf (fdArchOut, "pop buff\n");
   fprintf (fdArchOut, "pop buff\n");

   fprintf (fdArchOut, "popl %%eax\n");
   fprintf (fdArchOut, "write $1, $newline, $2\n");

   return (char*)"Print";
}

char* gNegacion (char* op1, char* op2) {
   static char* buff = NULL;
   if ( op1[0] >= '0' && op1[0] <= '9' ) { // niego un numero --> optimizacion
      buff = (char*) malloc (strlen (op1) + 1);
      sprintf (buff, "-%s", op1);
      free (op1);
      op1 = strdup (buff);
      free (buff); 
   } else if ( op1[0] == '-' ) { // negar un negativo!!!!
      op1[0] = '0'; // se le saca el signo: -23 = 023 ;-)
   } else {
      fprintf (fdArchOut, "negl %s\n", op1);
   }
   return op1;
}

char* gTermino (char* operator, char* op1, char* op2) {
    char* reg1;
    char buffTemp[20];
    if ( op1[0] != '%' ) { // si no es un registro
       if ( ((op1[0] >= '0' && op1[0] <= '9') || op1[0] == '-') 
           && ((op2[0] >= '0' && op2[0] <= '9') || op2[0] == '-') ) {
            // Si ambos son numeros entonces optimizacion de codigo
            if ( strcmp ( operator, "addl" ) == 0 )
               sprintf(buffTemp, "%d", atoi (op1) + atoi (op2));
            else
                sprintf(buffTemp, "%d", atoi (op1) - atoi (op2));
            reg1 = buffTemp;
            return reg1;
       }

       reg1 = getNextFreeReg ();
       if ( reg1 == NULL )
          reg1 = getNextFreeVar ();

       if ( (op1[0] >= '0' && op1[0] <= '9') || op1[0] == '-' ) // es un numero
          fprintf(fdArchOut,"movl $%s,%s\n",op1, reg1);
       else
          fprintf(fdArchOut,"movl %s,%s\n",op1, reg1);
    } else {
       reg1 = strdup (op1);
    }

    if ( (op2[0] >= '0' && op2[0] <= '9') || op2[0] == '-' ) // es un numero
       fprintf(fdArchOut,"%s $%s,%s\n",operator, op2, reg1);
    else
       fprintf(fdArchOut,"%s %s,%s\n",operator, op2, reg1);

   if ( op2[0] == '%' )
      freeReg (op2);
   else 
      freeVar (op2);

   return reg1;
}

char* gFactor(char* op1,char* op2, char* operando) {
    int flagEAX, flagEBX, flagEDX;
    char buffTemp[20];
    char* result;
    flagEAX = 0;
    flagEBX = 0;
    flagEDX = 0;

    if ( strcmp ( operando, "imull") == 0 ) 
      fprintf (fdArchOut, "// Multiplicacion\n");
    else
      fprintf (fdArchOut, "// Division\n");

    // Constante y Constante
    if ( esNumero (op1) && esNumero (op2) ) {
       
      if ( strcmp (operando, "imull") == 0) 
	sprintf (buffTemp, "%d", atoi (op1) * atoi (op2));
      else
	sprintf (buffTemp, "%d", atoi (op1) / atoi (op2));
      return strdup (buffTemp);

    }

    if ( isBusy ("%eax") ) {
      fprintf (fdArchOut, "pushl %%eax\n");
      flagEAX = 1;
    }
    if ( isBusy ("%ebx") ) {
      fprintf (fdArchOut, "pushl %%ebx\n");
      flagEBX = 1;
    }
    if ( strcmp (operando, "idivl") == 0 )  {
      if ( isBusy ("%edx") ) {
         fprintf (fdArchOut, "pushl %%edx\n");
         flagEDX = 1;
      }
      fprintf (fdArchOut, "movl $0, %%edx\n");
    }

    // Variable y Variable
    if ( esVariable(op1) && esVariable(op2) ) {
      fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      fprintf (fdArchOut, "%s %s\n", operando, op2);

      result = getNextFreeReg();
      if ( result == NULL ) 
         result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
         fprintf (fdArchOut, "movl %%eax, %s\n", result);

      // Variable y Numero
    } else if ( esVariable (op1) && esNumero (op2) ) {
      fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      fprintf (fdArchOut, "movl $%s, %%ebx\n", op2);
      fprintf (fdArchOut, "%s %%ebx\n", operando);

      result = getNextFreeReg();
      if ( result == NULL ) 
         result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
         fprintf (fdArchOut, "movl %%eax, %s\n", result);
     
      // Variable y Registro
    } else if ( esVariable (op1) && esRegistro (op2) ) {
      fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      if ( op2[2] != 'd')
         fprintf (fdArchOut, "movl %s, %%ebx\n", op1);
      fprintf (fdArchOut, "%s %%ebx\n", operando);

      result = getNextFreeReg();
      if ( result == NULL ) 
         result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
         fprintf (fdArchOut, "movl %%eax, %s\n", result);

      // Numero y Variable
    } else if ( esNumero (op1) && esVariable (op2) ) {
      fprintf (fdArchOut, "movl $%s, %%eax\n", op1);
      fprintf (fdArchOut, "%s %s\n", operando, op2);

      result = getNextFreeReg();
      if ( result == NULL ) 
         result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
         fprintf (fdArchOut, "movl %%eax, %s\n", result);

      // Numero y Registro
    } else if ( esNumero (op1) && esRegistro (op2) ) {
        fprintf (fdArchOut, "movl $%s, %%eax\n", op1);
        if ( op2[2] != 'b')
           fprintf (fdArchOut, "movl %s, %%ebx\n", op1);
        fprintf (fdArchOut, "%s %%ebx\n", operando);
        
        result = getNextFreeReg();
        if ( result == NULL ) 
            result = getNextFreeVar();
        if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
            fprintf (fdArchOut, "movl %%eax, %s\n", result);

      // Registro y Variable
    } else if ( esRegistro (op1) && esVariable (op2) ) {
      if ( op1[2] != 'a')
         fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      fprintf (fdArchOut, "%s %s\n", operando, op2);
    
      result = getNextFreeReg();
      if ( result == NULL ) 
          result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
          fprintf (fdArchOut, "movl %%eax, %s\n", result);

     // Registro y Numero
    } else if ( esRegistro (op1) && esNumero (op2) ) {
      if ( op1[2] != 'a')
         fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      fprintf (fdArchOut, "movl $%s, %%ebx\n", op2);
      fprintf (fdArchOut, "%s %%ebx\n", operando);

      result = getNextFreeReg();
      if ( result == NULL ) 
          result = getNextFreeVar();
      if ( (result[0] == '%' && result[2] != 'a') || esVariable (result) )  
          fprintf (fdArchOut, "movl %%eax, %s\n", result);

      // Registro y Registro
    } else if ( esRegistro (op1) && esRegistro (op2) ) {
      if ( op1[2] != 'a')
         fprintf (fdArchOut, "movl %s, %%eax\n", op1);
      if ( op1[2] != 'b')
         fprintf (fdArchOut, "movl %s, %%ebx\n", op2);
      fprintf (fdArchOut, "%s %%ebx\n", operando);
    }

    if ( flagEDX == 1 ) {
      fprintf (fdArchOut, "popl %%edx\n");
    }
    if ( flagEBX == 1 ) {
      fprintf (fdArchOut, "popl %%ebx\n");
    }
    if ( flagEAX == 1 ) {
      fprintf (fdArchOut, "popl %%eax\n");
    }

   if ( op1[0] == '%' )
      freeReg (op1);
   else 
      freeVar (op1);

    return result;
}

int esNumero (char* op) {
   return ((op[0] >= '0' && op[0] <= '9') || op[0] == '-');
}

int esVariable (char* op) {
   return (op[0] == 'v');
}

int esRegistro (char* op) {
   return (op[0] == '%');
}

