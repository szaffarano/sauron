#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <string.h>
#include <stdio.h>

#define MAX_SYMBOLS 100

#define TABLE_FULL -1
#define DEFAULT_VALUE -1

char* symbols[MAX_SYMBOLS];

int addTableId (char*);
int addTableConst (int value);
void dumpTable ();

#endif //_SYMBOL_TABLE_H_
