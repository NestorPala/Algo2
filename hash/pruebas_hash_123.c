#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "lista.h"


void printb(bool a) {
    a ? printf("TRUE\n") : printf("FALSE\n");
}


void obtenemos(hash_t* hash, char* claves[], size_t n) {
    for (size_t i=0; i<n; i++) {
        void* elemento = hash_obtener(hash, claves[i]);
        elemento ? printf("\n%s: %d", claves[i], *(int*)elemento) : printf("\n%s: NULL", claves[i]);
    }
}


void pertenencia(hash_t* hash, char* clave) {
    printf("LA CLAVE '%s' PERTENECE AL HASH   >>>  ", clave);
    printb(hash_pertenece(hash, clave));
}


void borramos(hash_t* hash, char* claves[], size_t n) {
    for (size_t i=0; i<n; i++) {
        hash_borrar(hash, claves[i]);
        printf("\n%s BORRADO", claves[i]);
        printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));
        pertenencia(hash, claves[i]);
    }   
}


void guardamos(hash_t* hash, char* claves[], int** numeros, size_t n) {
    for (size_t i=0; i<n; i++) {
        printf("\nGUARDANDO: %s\n", claves[i]);
        printb(hash_guardar(hash, claves[i], numeros[i]));
        printf("CANTIDAD DE ELEMENTOS: %zu\n", hash_cantidad(hash));
        pertenencia(hash, claves[i]);
    }
}


void probamos_iterador(hash_t* hash) {
    printf("\n\n\nPRUEBAS DEL ITERADOR DE HASH\n");

    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter) {printf("NO SE PUDO CREAR EL ITERADOR"); return;}

    while(!hash_iter_al_final(iter)) {
        const char* clave_actual = hash_iter_ver_actual(iter);
        if (clave_actual) printf("\nCLAVE: %s", clave_actual);
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
}


int** crear_arreglo_numeros(int numeros[], size_t n) {
    int** arr = malloc(n * sizeof(int*));
    if (!arr) return NULL;
    for (size_t i=0; i<n; i++) arr[i] = &numeros[i];
    return arr;
}


int main() {
    hash_t* hash = hash_crear(NULL);
    size_t cant = 9;
    int num[] = {12, 32, 43, -99, 126, 45, 65, 177, 277};
    char* claves[] = {"pepe", "carlos", "juana", "estefania", "freddy", "mariana", "jose", "tomas", "cecilio"};
    int** x = crear_arreglo_numeros(num, cant);

    guardamos(hash, claves, x, cant);
    printf("\n\n");
    obtenemos(hash, claves, cant);
    probamos_iterador(hash);
    printf("\n\n");
    borramos(hash, claves, cant);
    printf("\n\n");

    hash_destruir(hash);
    free(x);

    return 0;
}