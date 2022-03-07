#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "top-k_heap.h"


void imprimir_arreglo_cadenas(char* cadenas[], size_t n) {
    printf("\n[");

    for (size_t i=0; i<n; i++) {
        if (i < n-1) {
            printf("%s, ", cadenas[i]);
        } else {
            printf("%s", cadenas[i]);
        }
    }

    printf("]\n");
}


// Función de comparación de cadenas según su largo
int strcmp_(const void* a, const void* b) {
    return (int) strlen((const char*) a)  -  (int) strlen((const char*) b);
}


// Implementación del algoritmo TOP-K en O(N + K * log N)
char** top_k_cadenas(char* cadenas[], size_t n, size_t k) {
    if (k > n) {
        printf("K no puede ser mayor que el largo del arreglo de cadenas\n");
        return NULL;
    }

    char** top_cadenas = malloc(k * sizeof(char*));
    void** cadenas_aux = malloc(n * sizeof(void*));

    for (size_t i = 0; i < n; i++){
        cadenas_aux[i] = cadenas[i];
    }
    
    heap_t* heap = heap_crear_arr(cadenas_aux, n, strcmp_);

    for (size_t i = 0; i < k; i++) {
        top_cadenas[i] = (char*) heap_desencolar(heap);
    }

    heap_destruir(heap, NULL);
    free(cadenas_aux);

    return top_cadenas;
}


// C99
int main() {
    char* cadenas[] = {"pepe", "gonzalo", "maximiliano", "oso", "adrian"};
    size_t n = 5;
    size_t k = 2;

    imprimir_arreglo_cadenas(cadenas, n);

    char** top_cadenas = top_k_cadenas(cadenas, n, k);

    if (!top_cadenas) {
        return -1;
    }

    imprimir_arreglo_cadenas(top_cadenas, k);

    free(top_cadenas);

    return 0;
}
