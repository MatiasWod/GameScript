#include "utils.h"
#include <stdlib.h>
#include "shared.h"

typedef struct Node * List;

List allocated_memory = NULL;

struct Node {
    void * ptr;
    List next;
};


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

//TODO MOVER DE ACA
void FreeAll() {
	List allocated_memory = NULL;
    while (allocated_memory != NULL) {
        List to_free = allocated_memory;
        allocated_memory = to_free->next;
        free(to_free->ptr);
        free(to_free);
    }
}


void * Malloc(size_t size) {
	List allocated_memory = NULL;
    List new_node = malloc(sizeof(struct Node));
    void * requested_memory = malloc(size);
    new_node->ptr = requested_memory;
    new_node->next = allocated_memory;
    allocated_memory = new_node;
    return requested_memory;
}