#ifndef VARS_H
#define VARS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct vars {
    size_t count;
    char **names;
    void **values;
};

struct vars* vars_new(void) {
    struct vars *vars = malloc(sizeof(struct vars));
    vars->count = 0;
    vars->names = NULL;
    vars->values = NULL;
    return vars;
}

void vars_delete(struct vars* vars) {
    for(size_t i = 0; i < vars->count; i++) {
        free(vars->names[i]);
    }
    free(vars->names);
    free(vars->values);
    free(vars);
}

static size_t var_indexByName(struct vars *vars, const char* name) {
    for(size_t i = 0; i < vars->count; i++) {
        if(strcmp(vars->names[i], name) == 0) {
            return i;
        }
    }
    return SIZE_MAX;
}

void var_set(struct vars *vars, const char* name, void* value) {
    size_t index = var_indexByName(vars, name);
    if(index == SIZE_MAX) {
        vars->count++;
        vars->names = realloc(vars->names, vars->count * sizeof(char*));
        index = vars->count - 1;
        vars->names[index] = malloc(strlen(name) + 1);
        strcpy(vars->names[index], name);
        vars->values = realloc(vars->values, vars->count * sizeof(void*));
    }
    vars->values[index] = value;
}

void* var_get(struct vars *vars, const char* name) {
    size_t index = var_indexByName(vars, name);
    if(index == SIZE_MAX) {
        return NULL;
    }
    return vars->values[index];
}




#endif // VARS_H
