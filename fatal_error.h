#ifndef FATAL_ERROR_H
#define FATAL_ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define FATAL_ERROR(str) \
    fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, str); \
    exit(1);


#endif // FATAL_ERROR_H
