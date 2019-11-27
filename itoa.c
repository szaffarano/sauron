int itoa (int number, char* buf) {
   static int aux;
   static int count;
   aux = number;
   count = 0;

   if ( number < 0 ) {
      number *= -1;
      *buf++ = '-';
   }

   do {
      buf++;
      count++;
   } while ( (aux/=10) != 0 );

   *buf-- = '\0';
   while ( number != 0 ) {
      aux = number%10;
      number = number/10; 
      *buf-- = aux + 48;
   }
   return ++count;
}
