#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbol-table.h"
#include "../support/utils.h"
typedef struct SymbolObjectNode * SymbolObject;

struct SymbolObjectNode {
    char * key;
    VarType type;
    unsigned int scope;
    SymbolObject next;
};

SymbolObject table[HASH_TABLE_SIZE] = {0};

unsigned int scope = 0;

boolean contains(char * key);
int hash_code(char * string) {
    unsigned long hash = 5381;
	int c;

	while (c = *string++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash % HASH_TABLE_SIZE;
}

void InsertInSymbolTable(char * varname, VarType type) {
    if (contains(varname)) {
        DeleteFromSymbolTable(varname);
    }

    int hash = hash_code(varname);
    SymbolObject first = table[hash];
    SymbolObject new_item = Malloc(sizeof(struct SymbolObjectNode));
    new_item->key = varname;
    new_item->type = type;
    new_item->scope = scope;
    new_item->next = first;
    table[hash] = new_item;
}

void DeleteFromSymbolTable(char * key) {
    SymbolObject current = table[hash_code(key)];
    SymbolObject prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
               table[hash_code(key)] = current->next;
            } else {
                prev->next = current->next;
            }
            Free(current);
        }
        current = current->next;
    }
}

Symbol GetFromSymbolTable(char * key) {
    SymbolObject current = table[hash_code(key)];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            Symbol new_symbol = Malloc(sizeof(struct SymbolNode));
            new_symbol->varname = current->key;
            new_symbol->type = current->type;
            return new_symbol;
        }
        current = current->next;
    }
    return NULL;
}

boolean contains(char * key) {
    int index = hash_code(key);
    SymbolObject current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

boolean SymbolTableContains(char * key) {
    int index = hash_code(key);
    SymbolObject current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (current->scope <= scope) {
               return true;
            }
            return false;
        }
        current = current->next;
    }
    return false;
}


void StepIntoScope() {
    scope++;
}

void LeaveScope() {
    scope--;
}

void tryInsertIntoSymbolTable(char *name, Type type){
		if(!SymbolTableContains(name)){
		switch (type->tType)
		{
		case T_INT:
			InsertInSymbolTable(name, VARTYPE_INT);
			break;
		case T_TEXT:
			InsertInSymbolTable(name, VARTYPE_TEXT);
			break;
		case T_BOOL:
			InsertInSymbolTable(name, VARTYPE_BOOL);
			break;
		case T_CHAR:
			InsertInSymbolTable(name, VARTYPE_CHAR);
			break;
		case T_RGB:
			InsertInSymbolTable(name, VARTYPE_RGB);
			break;
		case T_BLOCK:
			InsertInSymbolTable(name, VARTYPE_BLOCK);
			break;
		case T_GOBJECT:
			InsertInSymbolTable(name, VARTYPE_GOBJECT);
			break;
		case T_SCENE:
			InsertInSymbolTable(name, VARTYPE_SCENE);
			break;
		case T_BUTTON:
			InsertInSymbolTable(name, VARTYPE_BUTTON);
			break;
		case T_STRING:
			InsertInSymbolTable(name, VARTYPE_STRING);
			break;
		default:
			break;
		}
	}
}