#include "heap.h"
#include "testing.h"
#include <stdio.h> //printf()
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <string.h> //strcmp()
#include <stdbool.h>


/***********************************************************************************/

// Cantidad masiva de pruebas
const size_t N = 2000;

/***********************************************************************************/


// Funcion auxiliar de comparacion
int comp(const void* a, const void* b) {
    if (*(int*)a == *(int*)b) {
        return 0;
    } else if (*(int*)a > *(int*)b) {
        return 1;
    } else {
        return -1;
    }
}


void pruebas_unitarias() {

    heap_t* heap = heap_crear(comp);

    int numeros[] = {5, 9, 10, 12, 11, 2};
    size_t cant = 6;

    for (size_t i=0; i<cant; i++) {
        heap_encolar(heap, &numeros[i]);
        int numero_actual = *(int*)heap_ver_max(heap);
    }

    if (heap_esta_vacio(heap)) {
        printf("EL HEAP ESTA VACIO\n");
    }

    printf("EL MINIMO DEL HEAP ES: '%d'\n", *(int*)heap_ver_max(heap));
    printf("CANTIDAD DEL HEAP: %zu\n", heap_cantidad(heap));

    for (size_t i=0; i<cant; i++) {
        int numero_actual = *(int*)heap_desencolar(heap);
        printf("%d\t", numero_actual);
    }

    heap_destruir(heap, NULL);

    printf("\n------------------------------------------------------------------------------\n");

    void** numeros2 = malloc(6 * sizeof(void*));
    for (size_t i=0; i<6; i++) numeros2[i] = &numeros[i];

    heap_t* heap2 = heap_crear_arr(numeros2, 6, comp);

    for (size_t i=0; i<cant; i++) {
        int numero_actual = *(int*)heap_desencolar(heap2);
        printf("%d\t", numero_actual);
    }

    free(numeros2);
    heap_destruir(heap2, NULL);
}


/***********************************************************************************/


void pruebas_heap_estudiante() {
    // pruebas_heap_vacio();
    pruebas_unitarias();
    // pruebas_volumen();
}


/***********************************************************************************/


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {

    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
