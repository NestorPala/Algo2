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


// Auxiliar
bool pruebas_heap_chequear_insercion(heap_t* heap, int numeros[], size_t cant, bool* todos_insertados){
    bool orden = true;
    void* elemento_anterior = NULL;

    for (size_t i=0; i<cant; i++) {

        if (i > 0) {
            elemento_anterior = heap_ver_max(heap);
        }

        if (!heap_encolar(heap, &numeros[i])) {
            *todos_insertados = false;
        }
        
        if (i > 0) {
            if (comp(&numeros[i], &numeros[i-1]) > 0) {
                if (comp(heap_ver_max(heap), elemento_anterior) < 0) {
                    orden = false;
                }
            }
        }
    }

    return orden;
}


// Auxiliar
bool pruebas_heap_chequear_eliminacion(heap_t* heap, int numeros[], size_t cant) {

    void* numero_anterior = NULL;
    void* numero_nuevo = NULL;
    bool orden_desencolados = true;

    for (size_t i=0; i<cant; i++) {
        numero_anterior = heap_ver_max(heap);
        numero_nuevo = heap_desencolar(heap);

        if (numero_anterior && numero_nuevo) {
            if (comp(numero_nuevo, numero_anterior) < 0) {
                orden_desencolados = false;
            }
        }
    }

    return orden_desencolados;
}


void pruebas_heap_crear() {
    printf("\n--------------------------------------- PRUEBAS HEAP CREAR ---------------------------------------\n");

    heap_t* heap = heap_crear(comp);

    int numeros[] = {5, 9, 10, 12, 11, 2};
    size_t cant = 6;
    bool ok = true, orden = true;

    orden = pruebas_heap_chequear_insercion(heap, numeros, cant, &ok);

    print_test("Se pudieron encolar todos los elementos correctamente: ", ok);
    print_test("Todos los elementos se insertaron en orden: ", orden);
    print_test("El heap no esta vacio: ", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos del heap es la correcta: ", heap_cantidad(heap) == 6);
    print_test("El maximo del heap es el correcto: ", *(int*)heap_ver_max(heap) == 12);

    bool orden_desencolados = pruebas_heap_chequear_eliminacion(heap, numeros, cant);
    print_test("Los elementos se desencolaron en el orden correcto: ", orden_desencolados);

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

    print_test("El heap no esta vacio: ", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos del heap es la correcta: ", heap_cantidad(heap) == 6);
    print_test("El maximo del heap es el correcto: ", *(int*)heap_ver_max(heap) == 12);

    bool orden_desencolados = pruebas_heap_chequear_eliminacion(heap, numeros, cant);
    print_test("Los elementos se desencolaron en el orden correcto: ", orden_desencolados);

    free(punteros);
    heap_destruir(heap, NULL);
}


void pruebas_heapsort() {
    printf("\n--------------------------------------- PRUEBAS HEAPSORT ---------------------------------------\n");

    int numeros[] = {32,45,5,67,56,45,2,-17};
    size_t cant = 8;
    void** punteros = malloc(cant * sizeof(void*));

    for (size_t i = 0; i < cant; i++){
        punteros[i] = &numeros[i];
    }

    heap_sort(punteros, cant, comp);

    for (size_t i=0; i<cant; i++) {
        printf("%d\t", *(int*)punteros[i]);
    }
    printf("\n");

    bool orden_hs = true;

    for (size_t i=0; i<cant; i++) {
        if (i > 0) {
            if (comp(punteros[i], punteros[i-1]) < 0) {
                orden_hs = false;
            }
        }
    }

    print_test("Los elementos se ordenaron correctamente: ", orden_hs);

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



void pruebas_volumen() {
    printf("\n--------------------------------------- PRUEBAS DE VOLUMEN ---------------------------------------\n");

    heap_t* heap = heap_crear(comp);

    int* numeros = malloc(N * sizeof(int*));

    for (size_t i=0; i<N; i++) {
        numeros[i] = rand() % 1000 + 1;
    }

    size_t cant = N;
    bool ok = true, orden = true;

    orden = pruebas_heap_chequear_insercion(heap, numeros, cant, &ok);

    print_test("Se pudieron encolar todos los elementos correctamente: ", ok);
    print_test("Todos los elementos se insertaron en orden: ", orden);
    print_test("El heap no esta vacio: ", !heap_esta_vacio(heap));
    print_test("La cantidad de elementos del heap es la correcta: ", heap_cantidad(heap) == N);

    bool orden_desencolados = pruebas_heap_chequear_eliminacion(heap, numeros, cant);
    print_test("Los elementos se desencolaron en el orden correcto: ", orden_desencolados);

    free(numeros);
    heap_destruir(heap, NULL);
}



void pruebas_heap_estudiante() {
    pruebas_heap_vacio();
    pruebas_unitarias();
    pruebas_volumen();
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
