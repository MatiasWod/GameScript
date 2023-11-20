#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../support/shared.h"

#define HASH_TABLE_SIZE 4096

typedef enum {
    VARTYPE_INT,
    VARTYPE_BOOL,
    VARTYPE_TEXT,
    VARTYPE_CHAR,
    VARTYPE_RGB,
    VARTYPE_BLOCK,
    VARTYPE_GOBJECT,
    VARTYPE_SCENE,
    VARTYPE_BUTTON,
    VARTYPE_STRING
} VarType;

struct SymbolNode {
    char * varname;
    VarType type;
};

typedef struct SymbolNode * Symbol;

void InsertInSymbolTable(char * varname, VarType type, void * value);
void DeleteFromSymbolTable(char * key);
Symbol GetFromSymbolTable(char * key);
boolean SymbolTableContains(char * key);

#endif