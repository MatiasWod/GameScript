#include <string.h>
#include <stdio.h>
#include "symbol-table.h"

typedef struct SymbolObjectNode * SymbolObject;

struct SymbolObjectNode {
    char * key;
    VarType type;
    unsigned int scope;
    SymbolObject next;
};

SymbolObject table[HASH_TABLE_SIZE] = {0};

typedef struct Node * List;

struct Node {
    void * ptr;
    List next;
};

unsigned int scope = 0;

List allocated_memory = NULL;

void Free(void * ptr) {
    if (ptr == NULL) {
        return;
    }
    
    List previous = NULL;
    List head = allocated_memory;
    boolean found = false;
    while (!found && head != NULL) {
        if (head->ptr == ptr) {
            found = true;
        } else {
            previous = head;
            head = head->next;
        }
    }

    if (!found) {
        return;
    }

    if (previous == NULL) {
        allocated_memory = head->next;
    } else {
        previous->next = head->next;
    }
    free(head->ptr);
    free(head);
}

void FreeAll() {
    while (allocated_memory != NULL) {
        List to_free = allocated_memory;
        allocated_memory = to_free->next;
        free(to_free->ptr);
        free(to_free);
    }
}


void * Malloc(size_t size) {
    List new_node = malloc(sizeof(struct Node));
    void * requested_memory = malloc(size);
    new_node->ptr = requested_memory;
    new_node->next = allocated_memory;
    allocated_memory = new_node;
    return requested_memory;
}

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