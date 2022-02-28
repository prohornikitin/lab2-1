#ifndef VARS_H
#define VARS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct vars;
struct vars* vars_new(void);
void vars_delete(struct vars* vars);
void var_set(struct vars *vars, const char* name, void* value);
void* var_get(struct vars *vars, const char* name);


#endif // VARS_H
