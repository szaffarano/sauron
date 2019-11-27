CCFLAGS=-m32 -I./include
#CCFLAGS=-m32 -I./include -DDEBUG

GCC=gcc
RM=rm -rf

all: parser test

generador.o: generador.c symbolTable.o
	$(GCC) -c generador.c $(CCFLAGS)

parser: procesos.o yylex.o symbolTable.o main.o y.tab.o generador.o \
	pila.o itoa.o
	$(GCC) -o parser y.tab.o procesos.o yylex.o symbolTable.o \
	generador.o pila.o $(CCFLAGS)

test: procesos.o yylex.o symbolTable.o main.o
	$(GCC) -o lexer main.o procesos.o yylex.o symbolTable.o $(CCFLAGS)

include/y.tab.h: y.tab.c

itoa.o: itoa.c
	$(GCC) -c itoa.c $(CCFLAGS)
	cp -f itoa.o ./lib/

y.tab.c: parser.y
	yacc -d parser.y

y.tab.o: y.tab.c
	$(GCC) -c y.tab.c $(CCFLAGS)

main.o: main.c include/y.tab.h
	$(GCC) -c main.c $(CCFLAGS)

procesos.o: procesos.c include/procesos.h include/y.tab.h
	$(GCC) -c procesos.c $(CCFLAGS)

yylex.o: yylex.c include/yylex.h include/y.tab.h
	$(GCC) -c yylex.c $(CCFLAGS)

symbolTable.o: symbolTable.c include/symbolTable.h
	$(GCC) -c symbolTable.c $(CCFLAGS)

pila.o: pila.c include/pila.h
	$(GCC) -c pila.c $(CCFLAGS)

clean:
	$(RM) *.o y.tab.c y.tab.h *.S lexer parser a.out
