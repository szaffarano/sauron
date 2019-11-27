%{
#include <procesos.h>
#include <generador.h>
#include <pila.h>
#include <yylex.h>
#include <symbolTable.h>

extern int fdArchivoFuente;
extern int yylvalue;
extern FILE* fdArchOut;
int countSwitch;
%}
   %union {
      int numero;
      struct nodo* ptNodo;
   }

   %start programa

   %type <ptNodo> programa bloque lista_sentencias sentencia asignacion 
   %type <ptNodo> expresion termino factor if switch opciones_switch opcion 
   %type <ptNodo> for condicion cte id s_factor
   %type <numero> operador_logico 

   %token <ptNodo> ID
   %token <ptNodo> MAS
   %token <ptNodo> MENOS
   %token <ptNodo> POR
   %token <ptNodo> DIVIDIDO
   %token <ptNodo> CONST_DEC
   %token <ptNodo> CONST_HEX
   %token <ptNodo> CONST_BIN
   %token <ptNodo> CONST_OCT
   %token <ptNodo> PARENTESIS_ABIERTA
   %token <ptNodo> PARENTESIS_CERRADA
   %token <ptNodo> PUNTO_COMA
   %token <ptNodo> LLAVES_ABIERTA
   %token <ptNodo> LLAVES_CERRADA
   %token <ptNodo> MENOR
   %token <ptNodo> MAYOR
   %token <ptNodo> MENOR_IGUAL
   %token <ptNodo> MAYOR_IGUAL
   %token <ptNodo> IGUAL
   %token <ptNodo> IGUAL_IGUAL
   %token <ptNodo> DISTINTO
   %token <ptNodo> NEGACION
   %token <ptNodo> PALABRA_MAIN
   %token <ptNodo> PALABRA_ENDMAIN
   %token <ptNodo> PALABRA_IF
   %token <ptNodo> PALABRA_ELSE
   %token <ptNodo> PALABRA_CASE
   %token <ptNodo> PALABRA_SWITCH
   %token <ptNodo> PALABRA_FOR
   %token <ptNodo> PALABRA_BREAK
   %token <ptNodo> DOS_PUNTOS
   %token <ptNodo> PALABRA_PRINT

%%
   programa 		: PALABRA_MAIN bloque PALABRA_ENDMAIN {
                             #ifdef DEBUG
                                printf ("Arbol sintactico:\n");
                                muestra_arbol ($2); 
                                printf ("\n");
                             #endif
                             genera_cabecera ();
                             genera_codigo ($2);
                             genera_salida (); 
                             genera_data ();
                        }
			;

   bloque		: LLAVES_ABIERTA lista_sentencias LLAVES_CERRADA {
			   #ifdef DEBUG
			      printf ("Lista de sentencias encontrada\n");
			   #endif
                           $$ = $2;
                        }
   			| asignacion PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Bloque de una linea encontrado\n");
			   #endif
                           $$ = $1;
                        }
                        | PALABRA_PRINT id PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (PRINT)\n");
			   #endif
                           $$ = crea_nodo (OP_PRINT, $2, NULL);
                        }
                        | PALABRA_PRINT cte PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (PRINT)\n");
			   #endif
                           $$ = crea_nodo (OP_PRINT, $2, NULL);
                        }
			; 

   lista_sentencias	: sentencia  {
			   #ifdef DEBUG
			      printf ("Lista de sentencias encontrada (unica sentencia)\n");
			   #endif
                           $$ = $1;
                        }
			| lista_sentencias sentencia {
			   #ifdef DEBUG
			      printf ("Lista de sentencias encontrada\n");
			   #endif
                           $$ = crea_nodo (OP_SENTENCIAS, $1, $2);
                        }
			;
			
   sentencia		: asignacion PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (asignacion)\n");
			   #endif
                           $$ = $1;
                        }
			| if {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (IF)\n");
			   #endif
                           $$ = $1;
                        }
			| switch {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (SWITCH)\n");
			   #endif
                           $$ = $1;
                        }
			| for {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (FOR)\n");
			   #endif
                           $$ = $1;
                        }
                        | PALABRA_PRINT id PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (PRINT)\n");
			   #endif
                           $$ = crea_nodo (OP_PRINT, $2, NULL);
                        }
                        | PALABRA_PRINT cte PUNTO_COMA {
			   #ifdef DEBUG
			      printf ("Sentencia encontrada (PRINT)\n");
			   #endif
                           $$ = crea_nodo (OP_PRINT, $2, NULL);
                        }
			;

   asignacion		: id IGUAL expresion {
			   #ifdef DEBUG
			      printf ("Asignacion encontrada (id=cte)\n");
			   #endif
                           $$ = crea_nodo (OP_IGUAL, $1, $3);
                        }
			;

   id			: ID {
			   #ifdef DEBUG
			      printf ("Id encontrada\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
      			; 

   s_factor		: MENOS factor {
                           struct nodo* nodo_negado;
                           struct nodo* dummy;
			   #ifdef DEBUG
			      printf ("Negacion encontrada (ter)\n");
			   #endif
                           dummy = crea_hoja ("dummy", 1);
                           nodo_negado = crea_nodo (OP_NEGACION, $2, dummy);
                           $$ = nodo_negado;
                        }
			| MAS factor {
			   #ifdef DEBUG
			      printf ("Expresion '+' encontrada (ter)\n");
			   #endif
                           $$ = $2;
                        }
			| factor {
			   #ifdef DEBUG
			      printf ("Expresion encontrada (ter)\n");
			   #endif
                           $$ = $1;
                        }
			;
   
   expresion		: expresion MAS termino {
			   #ifdef DEBUG
			      printf ("Expresion encontrada (expr+ter)\n");
			   #endif
                           $$ = crea_nodo (OP_SUMA, $1, $3);
                        }
   			| expresion MENOS termino {
			   #ifdef DEBUG
			      printf ("Expresion encontrada (expr-ter)\n");
			   #endif
                           $$ = crea_nodo (OP_RESTA, $1, $3);
                        }
			| termino {
			   #ifdef DEBUG
			      printf ("Expresion encontrada (ter)\n");
			   #endif
                           $$ = $1;
                        }
			;

   termino		: termino POR s_factor {
			   #ifdef DEBUG
			      printf ("Termino encontrado (ter*fact)\n");
			   #endif
                           $$ = crea_nodo (OP_POR, $1, $3);
                        }
			| termino DIVIDIDO s_factor {
			   #ifdef DEBUG
			      printf ("Termino encontrado (ter/fact)\n");
			   #endif
                           $$ = crea_nodo (OP_DIV, $1, $3);
                        }
			| s_factor {
			   #ifdef DEBUG
			      printf ("Termino encontrado (fact)\n");
			   #endif
                           $$ = $1;
                        }
			;

   factor		: id  {
			   #ifdef DEBUG
			      printf ("Termino encontrado (ID)\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
			| cte {
			   #ifdef DEBUG
			      printf ("Termino encontrado (cte)\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
			| PARENTESIS_ABIERTA expresion PARENTESIS_CERRADA  {
			   #ifdef DEBUG
			      printf ("Termino encontrado ('(expr)')\n");
			   #endif
                           $$ = $2;
                        }
			;

   if			: PALABRA_IF PARENTESIS_ABIERTA condicion PARENTESIS_CERRADA bloque {
                          struct nodo* cuerpo;
                          struct nodo* temporal;
                          struct nodo* cond;
                          struct nodo* inicializa_if;
                          temporal = NULL;
			  #ifdef DEBUG
			     printf ("IF sin ELSE encontrado\n");
			  #endif
                          if ( $5->operacion.operacion == OP_SENTENCIAS ) {
                             $5->operacion.operacion = OP_SENTENCIAS_IF;
                             $5->tipo = NODO;
                          } else {
                             temporal = crea_nodo (OP_SENTENCIAS_IF, $5, NULL);
                             $5 = temporal;
                          } 
                          inicializa_if = crea_hoja ("op_inic_if", 1);
                          inicializa_if->operacion.operacion = OP_INICIALIZA_IF;
                          inicializa_if->tipo = NODO;
                          cuerpo = crea_nodo (OP_CUERPO_IF, $5, NULL);
                          cond = crea_nodo (OP_SENTENCIAS, inicializa_if, $3);
                          $$ = crea_nodo (OP_IF, cond, cuerpo);
                       }
			| PALABRA_IF PARENTESIS_ABIERTA condicion PARENTESIS_CERRADA bloque PALABRA_ELSE bloque {
                          struct nodo* cuerpo;
                          struct nodo* temporal;
                          struct nodo* cond;
                          struct nodo* inicializa_if;
                          temporal = NULL;
			  #ifdef DEBUG
			     printf ("IF con ELSE encontrado\n");
			  #endif

                          if ( $5->operacion.operacion == OP_SENTENCIAS ) {
                             $5->operacion.operacion = OP_SENTENCIAS_IF;
                             $5->tipo = NODO;
                          } else {
                             temporal = crea_nodo (OP_SENTENCIAS_IF, $5, NULL);
                             $5 = temporal;
                          } 
                          inicializa_if = crea_hoja ("op_inic_if", 1);
                          inicializa_if->operacion.operacion = OP_INICIALIZA_IF;
                          inicializa_if->tipo = NODO;
                          cuerpo = crea_nodo (OP_CUERPO_IF, $5, $7);
                          cond = crea_nodo (OP_SENTENCIAS, inicializa_if, $3);
                          $$ = crea_nodo (OP_IF, cond, cuerpo);
                       }

   switch		: PALABRA_SWITCH PARENTESIS_ABIERTA id PARENTESIS_CERRADA LLAVES_ABIERTA opciones_switch LLAVES_CERRADA {
                           struct nodo* init_switch = NULL; 
                           struct nodo* cantSwitch = NULL;
                           char buff[20];
			   #ifdef DEBUG
			      printf ("SWITCH encontrado\n");
			   #endif
                           countSwitch = 0;
                           cuenta_case ($6);
                           sprintf (buff, "%d", countSwitch);
                           cantSwitch = crea_hoja (buff, 1);
                           init_switch = crea_nodo ( OP_INIT_SWITCH, $3, cantSwitch);
                           $$ = crea_nodo (OP_SWITCH, init_switch, $6);
                        }
			;

   opciones_switch	: opcion {
			   #ifdef DEBUG
			      printf ("opcion SWITCH encontrada\n");
			   #endif
                           $$ = $1;
                        }
			| opciones_switch opcion {
			   #ifdef DEBUG
			      printf ("opciones SWITCH encontradas\n");
			   #endif
                           $$ = crea_nodo (OP_OPCIONES_SWITCH, $1, $2);
                        }
			;

   opcion		: PALABRA_CASE cte DOS_PUNTOS bloque PALABRA_BREAK PUNTO_COMA {
                           struct nodo* sent_switch = NULL;
                           struct nodo* id_ficticio = NULL;
			   #ifdef DEBUG
			      printf ("Opcion del SWITCH encontrada\n");
			   #endif
                           id_ficticio = crea_hoja ("id_ficticio", 1);
                           sent_switch = crea_nodo ( OP_SENT_SWITCH, id_ficticio, $2);
                           $$ = crea_nodo (OP_OPCION_SWITCH, sent_switch, $4);
                        }
			;

   for			: PALABRA_FOR PARENTESIS_ABIERTA asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PARENTESIS_CERRADA bloque {
                           struct nodo* sentAsign = NULL;
                           struct nodo* sentFor = NULL;
                           struct nodo* nodoFor = NULL;
                           struct nodo* sentencias = NULL;
                           struct nodo* init = NULL;
			   #ifdef DEBUG
			      printf ("FOR encontrado\n");
			   #endif
                           init = crea_hoja ("init_for", 1);
                           init->operacion.operacion = OP_INICIALIZA_FOR;
                           init->tipo = NODO;
                           sentFor = crea_nodo (OP_SENTENCIAS, $9, $7);
                           sentencias = crea_nodo (OP_SENTENCIAS, $3, init);
                           nodoFor = crea_nodo (OP_FOR, $5, sentFor);
                           sentAsign = crea_nodo (OP_SENTENCIAS, sentencias, nodoFor); 
                           $$ = sentAsign;
                        }
			;

   condicion		: expresion operador_logico expresion {
			   #ifdef DEBUG
			      printf ("Condicion encontrada\n");
			   #endif
                           $$ = crea_nodo ($2, $1, $3);
                        }
			| NEGACION condicion {
			   #ifdef DEBUG
			      printf ("Condicion negada encontrada\n");
			   #endif
                           switch ( $2->operacion.operacion ) {
                              case OP_MENOR:
                                 $2->operacion.operacion = OP_MAYOR_IGUAL;
                                 break;
                              case OP_MAYOR:
                                 $2->operacion.operacion = OP_MENOR_IGUAL;
                                 break;
                              case OP_MENOR_IGUAL:
                                 $2->operacion.operacion = OP_MAYOR;
                                 break;
                              case OP_MAYOR_IGUAL:
                                 $2->operacion.operacion = OP_MENOR;
                                 break;
                              case OP_IGUAL_IGUAL:
                                 $2->operacion.operacion = OP_DISTINTO;
                                 break;
                              case OP_DISTINTO:
                                 $2->operacion.operacion = OP_IGUAL_IGUAL;
                                 break;
                           };
                           $$ = $2;
                        }
			;

   cte			: CONST_DEC {
			   #ifdef DEBUG
			      printf ("Cte dec encontrada\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
			| CONST_HEX {
			   #ifdef DEBUG
			      printf ("Cte hexa encontrada\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
  			| CONST_BIN {
			   #ifdef DEBUG
			      printf ("Cte binaria encontrada\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
 			| CONST_OCT {
			   #ifdef DEBUG
			      printf ("Cte octal encontrada\n");
			   #endif
                           $$ = crea_hoja (symbols[yylvalue], 1);
                        }
			;

   operador_logico	: MENOR {
                           $$ = OP_MENOR;
                        }
			| MAYOR {
                           $$ = OP_MAYOR;
                        }
   			| MENOR_IGUAL {
                           $$ = OP_MENOR_IGUAL;
                        }
   			| MAYOR_IGUAL {
                           $$ = OP_MAYOR_IGUAL;
                        }
   			| IGUAL_IGUAL {
                           $$ = OP_IGUAL_IGUAL;
                        }
   			| DISTINTO {
                           $$ = OP_DISTINTO;
                        }
			;

%%

int main (int argc, char* argv[]) {
    char* archOut;
    char* archOutAsm;
    char* archOutObj;
    int ret;
    char cmd[255];
 
    extern int yyparse ();

    if ( argc != 2 && argc != 4 ) {
       fprintf (stderr,ERROR_ARGUMENTOS,argv[0]);
       exit (1);
    }
    if ( argc == 4 ) {
       if ( strcmp (argv[2], "-o") == 0 ) {
          archOut = strdup (argv[3]);
          fdArchivoFuente = open(argv[1],O_RDONLY);
       } else if ( strcmp (argv[1], "-o") == 0 ) {
          archOut = strdup (argv[2]);
          fdArchivoFuente = open(argv[3],O_RDONLY);
       } else {
          fprintf (stderr,ERROR_ARGUMENTOS,argv[0]);
          exit (1);
       }
    } else {
       fdArchivoFuente = open(argv[1],O_RDONLY);
       archOut = strdup ("a.out");
    }

    archOutAsm = (char*) malloc (strlen (archOut) + 2);
    archOutObj = (char*) malloc (strlen (archOut) + 2);

    strcpy (archOutObj, archOut);
    strcat (archOutObj, ".o");

    strcpy (archOutAsm, archOut);
    strcat (archOutAsm, ".S");

    initGenerator ();

    fdArchOut = fopen (archOutAsm, "w");

    if ( fdArchOut == NULL ) {
       perror (archOutAsm);
       exit (1);
    }
     
    if ( fdArchivoFuente < 0 ) {
       perror (argv[0]);
       exit (1);
    }
    
    initProcesos();
    ret = yyparse ();
    fclose ( fdArchOut );

    if ( !ret ) {
       sprintf (cmd, "as --32 -o %s %s", archOutObj, archOutAsm);
       system (cmd);
       sprintf (cmd, "ld -m elf_i386 -o %s %s lib/itoa.o", archOut, archOutObj);
       system (cmd);
    }

    free ( archOut );
    free ( archOutAsm );
    free ( archOutObj );

    return ( ret );
}

void yyerror (char* msg) {
   fprintf (stderr, "%s\n", msg);
}
