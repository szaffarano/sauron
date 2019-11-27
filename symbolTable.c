#include <symbolTable.h>

int countSymbols = 0;

int addTableId (char* name) {
   int i = 0;

   if ( countSymbols == MAX_SYMBOLS )
      return TABLE_FULL;

   for (; i < countSymbols; i++) {
      if ( strcmp (name, symbols[i]) == 0 )
         break;
   }

   if ( i == countSymbols ) { // agregamos a la tabla
      symbols [countSymbols] = strdup (name);
      return (countSymbols++);
   }
   return i;
}

int addTableConst (int value) {
   char buffTmp[256];
   sprintf (buffTmp, "%d", value);
   return addTableId (buffTmp);
}

void dumpTable () {
   int i = 0;
   printf ("Tabla de simbolos (%d entradas)\n\n", countSymbols);
   for (; i < countSymbols; i++) {
      printf ("\tNombre: %s\n", symbols[i]);
   }
}
