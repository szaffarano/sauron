#include <tokens.h>
#include <symbolTable.h>
#include <procesos.h>
#include <yylex.h>

extern int fdArchivoFuente; 
extern int flagTerminal;
extern int lineaLeida;
extern char* buff[MAX_BUFF];
extern char* tokenNames[];
extern int buffPos;
extern int yylvalue;

int main (int argc, char* argv[]) {
    int token;
    
    if ( argc != 2 ) {
       fprintf (stderr,ERROR_ARGUMENTOS,argv[0]);
       exit (1);
    }
  
    fdArchivoFuente = open(argv[1],O_RDONLY);
    if ( fdArchivoFuente < 0 ) {
       perror (argv[0]);
       exit (1);
    }
    
    initProcesos();

    fprintf(stdout,"Leyendo archivo fuente: %s\n",argv[1]);

    fprintf (stdout,"Tokens leidos: \n");
    while ( ((token = yylex()) != FIN_ARCHIVO)) {
         if (token == ERROR)
            fprintf(stdout,"Error de sintaxis en la linea: %d\n",lineaLeida);
         else {
              fprintf (stdout,"\tToken: %s",tokenNames[token-257]);
          
              if ( yylvalue == NOT_SET )
                  fprintf (stdout," es palabra reservada\n");
              else
                  fprintf (stdout," posicion en la tabla: %d\n", yylvalue);
         }
    }

    fprintf(stdout,"\nTotal de lineas leidas: %d \n",lineaLeida-1);

    dumpTable ();
    return 0;
}
