#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "vd.h"


void vd_print(vd_t* vector) {
    printf("\n[");
    for (size_t i=0; i<vd_largo(vector); i++) {
        void* elemento_actual = vd_obtener(vector, i, NULL);

        elemento_actual ? printf("%d", *(int*)elemento_actual) : printf("__");

        if (i < vd_largo(vector) - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    printf("El largo del vector es: %zu\n", vd_largo(vector));
    printf("La cantidad de elementos del vector es: %zu\n", vd_cantidad(vector));
}


int main() {

    int valor1 = 12, valor2 = -48, valor3 = 6;

    vd_t* vector = vd_crear(20);

    vd_guardar(vector, 0, &valor1);
    vd_guardar(vector, 1, &valor2);
    vd_guardar(vector, 2, &valor3);
    vd_print(vector);

    vd_redimensionar(vector, 10);
    vd_print(vector);

    vd_redimensionar(vector, 5);
    vd_print(vector);

    vd_redimensionar(vector, 3);
    vd_print(vector);

    vd_redimensionar(vector, 2);
    vd_print(vector);

    vd_redimensionar(vector, 1);
    vd_print(vector);

    vd_redimensionar(vector, 15);
    vd_print(vector);

    for (int i=0; i<vd_largo(vector); i++) {
        vd_guardar(vector, i, &valor1);
    }

    vd_print(vector);

    vd_guardar(vector, 8, NULL);
    vd_print(vector);

    vd_guardar(vector, 6, NULL);
    vd_print(vector);

    vd_guardar(vector, 0, NULL);
    vd_print(vector);

    vd_guardar(vector, 0, NULL);
    vd_print(vector);

    vd_guardar(vector, 0, NULL);
    vd_print(vector);

    for (int i=0; i<10000; i++) {
        vd_guardar(vector, i % vd_largo(vector), NULL);
    }

    vd_print(vector);

    for (int i=0; i<10000; i++) {
        vd_guardar(vector, i % vd_largo(vector), &valor2);
    }

    vd_print(vector);

    vd_destruir(vector);

    return 0;
}