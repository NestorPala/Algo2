#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "vd.h"


void vd_print(vd_t* vector) {
    printf("\n[");
    for (size_t i=0; i<vd_largo(vector); i++) {
        void* elemento_actual = vd_obtener(vector, i);

        elemento_actual ? printf("%d", *(int*)elemento_actual) : printf("NULL");

        if (i < vd_largo(vector) - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    printf("El largo del vector es: %zu\n", vd_largo(vector));
}


int main() {

    int valor1 = 12, valor2 = -48, valor3 = 6;

    vd_t* vector = vd_crear();

    vd_guardar(vector, 0, &valor1);
    vd_guardar(vector, 1, &valor2);
    vd_guardar(vector, 2, &valor3);

    vd_print(vector);

    vd_guardar(vector, 2, NULL);
    vd_print(vector);

    vd_guardar(vector, 1, NULL);
    vd_print(vector);

    vd_guardar(vector, 0, NULL);
    vd_print(vector);

    vd_guardar(vector, 0, NULL);
    vd_print(vector);

    vd_guardar(vector, 10, NULL); //guarda en 3
    vd_print(vector);

    vd_borrar(vector, 3);
    vd_print(vector);

    vd_borrar(vector, 2);
    vd_print(vector);

    vd_borrar(vector, 1);
    vd_print(vector);

    vd_borrar(vector, 0);
    vd_print(vector);

    vd_borrar(vector, 0);
    vd_print(vector);

    vd_borrar(vector, 1);
    vd_print(vector);

    vd_borrar(vector, 2);
    vd_print(vector);

    vd_borrar(vector, 3);
    vd_print(vector);

    vd_borrar(vector, 4);
    vd_print(vector);

    vd_borrar(vector, 5);
    vd_print(vector);

    size_t n = 50;
    int numeros[n];

    for (int i=0; i<n; i++) {
        numeros[i] = i*i;
    }

    int** pepe = malloc(n * sizeof(int*));

    for (size_t i=0; i<n; i++) {
        pepe[i] = &numeros[i];
    }

    for (size_t i=0; i<n; i++) {
        (i % 3 == 0) ? vd_guardar(vector, i, pepe[i]) : vd_guardar(vector, i, NULL);
    }

    vd_print(vector);

    for (size_t i=0; i<n; i++) {
        vd_borrar(vector, i);
    }

    vd_print(vector);

    free(pepe);
    vd_destruir(vector);

    return 0;
}