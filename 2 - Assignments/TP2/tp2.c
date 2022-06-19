#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "algogram.h"


int main(int argc, char** argv) {

    // Chequeo de parámetros
    if (argc != 2) {
        fprintf(stderr, "%s", "Error: Cantidad erronea de parametros");
        return 1;
    }

    FILE* usuarios = NULL;
    
    usuarios = fopen(argv[1], "rb");

    if (!usuarios) {
        fprintf(stderr, "%s", "Error: Archivo fuente inaccesible");
        return 1;
    }

    // Iniciamos la red social
    algogram(usuarios);
    
    fclose(usuarios);
    return 0;
}
