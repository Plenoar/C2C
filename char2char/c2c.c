#include "c2c.h"
#include "uthash.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct C2C_Entry {
    char *name;         // Key
    char *path;         // Value 1: The File Path
    void *user_ptr;     // Value 2: The Runtime Buffer (Auxiliary)
    UT_hash_handle hh;  
} C2C_Entry;

struct C2C {
    C2C_Entry *head;
};

C2C* c2c_create(void) {
    C2C *c2c = malloc(sizeof(C2C));
    if (c2c) {
        c2c->head = NULL;
    }
    return c2c;
}

void c2c_destroy(C2C *c2c) {
    if (!c2c) return;

    C2C_Entry *current_entry, *tmp;

    HASH_ITER(hh, c2c->head, current_entry, tmp) {
        HASH_DEL(c2c->head, current_entry);
        free(current_entry->name);
        free(current_entry->path);
        free(current_entry);
    }
    
    free(c2c);
}

int c2c_insert(C2C *c2c, const char *name, const char *path) {
    if (!c2c || !name || !path) return 0;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);
    if (entry) {
        return -1; 
    }

    entry = malloc(sizeof(C2C_Entry));
    if (!entry) return 0;

    entry->name = strdup(name);
    entry->path = strdup(path);
    entry->user_ptr = NULL; // Explicitly init to NULL

    if (!entry->name || !entry->path) {
        if (entry->name) free(entry->name);
        if (entry->path) free(entry->path);
        free(entry);
        return 0;
    }

    HASH_ADD_STR(c2c->head, name, entry); 
    return 1;
}

int c2c_bind_ptr(C2C *c2c, const char *name, void *ptr) {
    if (!c2c || !name) return 0;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);
    
    if (entry) {
        entry->user_ptr = ptr;
        return 1;
    }
    return 0;
}

void* c2c_get_ptr(const C2C *c2c, const char *name) {
    if (!c2c || !name) return NULL;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);

    if (entry) {
        return entry->user_ptr;
    }
    return NULL;
}

int c2c_update(C2C *c2c, const char *name, const char *new_path) {
    if (!c2c || !name || !new_path) return 0;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);
    
    if (!entry) return -1;

    char *copy = strdup(new_path);
    if (!copy) return 0;

    free(entry->path);
    entry->path = copy;
    // Note: This does NOT touch user_ptr. 
    // If updating the path invalidates the buffer, you must handle that manually.

    return 1;
}

const char* c2c_get(const C2C *c2c, const char *name) {
    if (!c2c || !name) return NULL;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);

    return entry ? entry->path : NULL;
}

int c2c_remove(C2C *c2c, const char *name) {
    if (!c2c || !name) return 0;

    C2C_Entry *entry = NULL;
    HASH_FIND_STR(c2c->head, name, entry);

    if (entry) {
        HASH_DEL(c2c->head, entry);
        free(entry->name);
        free(entry->path);
        free(entry);
        return 1;
    }
    return 0; 
}

size_t c2c_count(const C2C *c2c) {
    if (!c2c) return 0;
    return HASH_COUNT(c2c->head);
}

void c2c_foreach(C2C *c2c, c2c_callback cb, void *user_data) {
    if (!c2c || !cb) return;

    C2C_Entry *el, *tmp;
    HASH_ITER(hh, c2c->head, el, tmp) {
        cb(el->name, el->path, user_data);
    }
}