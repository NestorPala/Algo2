#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "algogram.h"


//...


void algogram(FILE* usuarios) {
    size_t n = 16;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, usuarios) != EOF) {
        printf("%s\n", buffer);
    }

    //...

    free(buffer);
}
