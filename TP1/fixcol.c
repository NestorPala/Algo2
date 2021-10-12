#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h> //fprintf()
#include <stdbool.h>
#include <stddef.h> //NULL
#include <ctype.h> //isdigit()
#include <string.h> //strstr()


void fixcol(char** argv, FILE* archivo) {

    int tamanio_buffer  =  atoi(argv[1]) + 1;
    char linea[tamanio_buffer];

    while (fgets(linea, tamanio_buffer, archivo) != NULL) {

        if (linea[strlen(linea) - 1] == '\n') {
            linea[strlen(linea) - 1] = '\0';
        }

        if (linea[0] != '\0') {
            printf("%s\n", linea);
        }
    }
}


FILE* obtener_archivo(char** argv) {

    // Se comprobó previamente que stdin tenga contenido utilizando stdin_vacio()

    if (argv[2] == NULL) {
        return stdin;
    }

    FILE* archivo = fopen(argv[2], "r");

    if (archivo == NULL) {
        return NULL;
    }

    return archivo;
}


bool es_numero(char* param) {

    // Recorro el string caracter a caracter preguntando si ese caracter es un número

    while (*param != '\0') {
        if (!isdigit(*param)) {
            return false;
        }
        param++;
    }

    return true;
}


bool cheq_err_parametros(char** argv) {
    
    // Verifico que no hayan 0 parámetros, y luego, si hay al menos uno, que el primero sea un número

    if (argv[1] == NULL || !es_numero(argv[1])) {
        fprintf(stderr, "%s", "Error: Cantidad erronea de parametros");
        return false;
    }

    return true;
}


int main(int argc, char** argv) {

    // Chequeo de parámetros
    if (!cheq_err_parametros(argv)) {
        return -1;
    }

    FILE* archivo = obtener_archivo(argv);

    if (archivo == NULL) {
        fprintf(stderr, "%s", "Error: archivo fuente inaccesible");
        return -2;
    }

    // Muestra el archivo por líneas
    fixcol(argv, archivo);

    fclose(archivo);
    return 0;
}
