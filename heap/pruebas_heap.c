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

    printf("\n--------------------------------------- HEAP CREAR ---------------------------------------\n");

    heap_t* heap = heap_crear(comp);

    int numeros[] = {5, 9, 10, 12, 11, 2};
    size_t cant = 6;

    for (size_t i=0; i<cant; i++) {
        heap_encolar(heap, &numeros[i]);
    }

    if (heap_esta_vacio(heap)) {
        printf("EL HEAP ESTA VACIO\n");
    }

    printf("EL MINIMO DEL HEAP ES: '%d'\n", *(int*)heap_ver_max(heap));
    printf("CANTIDAD DEL HEAP: %zu\n", heap_cantidad(heap));

    for (size_t i=0; i<cant; i++) {
        int numero_actual = *(int*)heap_desencolar(heap);
        numero_actual ? printf("%d\t", numero_actual) : printf("NULL\t");
    }

    heap_destruir(heap, NULL);

    printf("\n--------------------------------------- HEAP CREAR ARR ---------------------------------------\n");

    void** x1 = malloc(6 * sizeof(void*));
    for (size_t i = 0; i < 6; i++)
    {
        x1[i] = &numeros[i];
    }
  
    heap_t* heap2 = heap_crear_arr(x1, 6, comp);

    for (size_t i=0; i<cant; i++) {
        int pepe = *(int*)heap_desencolar(heap2);
        pepe ? printf("%d\t", pepe) : printf("NULL\t");
    }

    free(x1);
    heap_destruir(heap2, NULL);

    printf("\n--------------------------------------- HEAPSORT ---------------------------------------\n");

    int asd[] = {32,45,5,67,56,45,2,-17};
    void** x87 = malloc(8 * sizeof(void*));
    for (size_t i = 0; i < 8; i++)
    {
        x87[i] = &asd[i];
    }

    heap_sort(x87, 8, comp);

    for (size_t i=0; i<8; i++) {
        printf("%d\t", *(int*)x87[i]);
    }

    free(x87);
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
