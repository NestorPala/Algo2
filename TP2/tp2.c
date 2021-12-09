#define _POSIX_C_SOURCE 200809L
#define USUARIOS "usuarios.txt"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "algogram.h"


bool es_usuarios_txt(char* archivo) {
    return (strcmp(archivo, USUARIOS) == 0) ? true : false;
}


// Verifico que haya un solo parámetro y que sea un txt
bool cheq_err_parametros(int argc, char** argv) {
    
    if (argc != 2) {
        fprintf(stderr, "%s", "Error: Cantidad erronea de parametros");
        return false;
    }

    if (!es_usuarios_txt(argv[1])) {
        fprintf(stderr, "%s", "Error: El nombre del archivo no es válido");
        return false;
    }

    return true;
}


int main(int argc, char** argv) {

    // Chequeo de parámetros
    if (!cheq_err_parametros(argc, argv)) {
        return -1;
    }

    FILE* usuarios = fopen(argv[1], "r");

    if (!usuarios) {
        fprintf(stderr, "%s", "Error: Archivo fuente inaccesible");
        return -2;
    }

    // Iniciamos la ""red social""
    algogram(usuarios);
    
    fclose(usuarios);
    return 0;
}
