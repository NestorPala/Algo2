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


void pruebas_heap_crear() {
    printf("\n--------------------------------------- PRUEBAS HEAP CREAR ---------------------------------------\n");

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
}


void pruebas_heap_crear_arr() {
    printf("\n--------------------------------------- PRUEBAS HEAP CREAR ARR ---------------------------------------\n");

    int numeros[] = {5, 9, 10, 12, 11, 2};
    size_t cant = 6;

    void** punteros = malloc(cant * sizeof(void*));
    for (size_t i = 0; i < cant; i++) {
        punteros[i] = &numeros[i];
    }
  
    heap_t* heap = heap_crear_arr(punteros, cant, comp);

    for (size_t i=0; i<cant; i++) {
        int valor_actual = *(int*)heap_desencolar(heap);
        valor_actual ? printf("%d\t", valor_actual) : printf("NULL\t");
    }

    free(punteros);
    heap_destruir(heap, NULL);
}


void pruebas_heapsort() {
    printf("\n--------------------------------------- PRUEBAS HEAPSORT ---------------------------------------\n");

    int numeros[] = {32,45,5,67,56,45,2,-17};
    size_t cant = 8;
    void** punteros = malloc(cant * sizeof(void*));
    for (size_t i = 0; i < cant; i++)
    {
        punteros[i] = &numeros[i];
    }

    heap_sort(punteros, cant, comp);

    for (size_t i=0; i<cant; i++) {
        printf("%d\t", *(int*)punteros[i]);
    }

    free(punteros);
}


void pruebas_unitarias() {
    pruebas_heap_crear();
    pruebas_heap_crear_arr();
    pruebas_heapsort();
}


void pruebas_heap_vacio() {
    printf("\n--------------------------------------- PRUEBAS HEAP VACIO ---------------------------------------\n");
    heap_t* heap = heap_crear(comp);

    print_test("El heap esta vacio: ", heap_esta_vacio(heap));
    print_test("La cantidad del heap es 0: ", heap_cantidad(heap) == 0);
    print_test("No se puede desencolar un heap vacio: ", !heap_desencolar(heap));
    print_test("No se puede ver el maximo un heap vacio: ", !heap_ver_max(heap));

    heap_destruir(heap, NULL);
}


/***********************************************************************************/


void pruebas_heap_estudiante() {
    pruebas_heap_vacio();
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
