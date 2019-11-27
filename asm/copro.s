# programa que realiza multiplicaciones con la pila del coprocesador
# matematico del pentium 
.data
vallong:
	.long 9995 
valor:
	.long 4
.text
.globl _start
_start:
	fild (%eax)   # carga en la pila del copro
        #fimull (%eax)
        fiaddl (%eax)
        fist vallong    # guardamos en un reg

       mov $vallong, %ecx
       mov $4, %eax
       mov $1, %ebx
       mov $1, %edx
        int $0x80

        mov $1, %eax
        xorl %ebx, %ebx
        int $0x80
