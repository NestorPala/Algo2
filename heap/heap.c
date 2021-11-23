#include "heap.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h> //strcpy(), strcmp()


const size_t CAPACIDAD_INICIAL = 20;
const size_t FACTOR_CARGA = 2;


typedef void (*destr_t)(void *e);


struct heap {
	void** arr;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};


// AUXILIAR
void** arreglo_copia_crear(void** datos, size_t vieja_capacidad, size_t nueva_capacidad) {

    void** nuevo_arreglo = malloc(nueva_capacidad * sizeof(void*));
    if (!nuevo_arreglo) return NULL;

    for (size_t i=0; i<vieja_capacidad; i++) {
        nuevo_arreglo[i] = datos[i];
    }

    for (size_t i=vieja_capacidad; i<nueva_capacidad; i++) {
        nuevo_arreglo[i] = NULL;
    }

    return nuevo_arreglo;
}


// AUXILIAR
void arreglo_destruir(void** datos, size_t n, destr_t destruir_dato) {
    if (!datos || n == 0) return;

    for (size_t i=0; i<n; i++) {
        if (destruir_dato) {
            destruir_dato(datos[i]);
        }
    }

    free(datos);
}


// AUXILIAR
heap_t* heap_crear_2(void** datos, size_t n, cmp_func_t cmp) {

    if (!cmp) return NULL;

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    if (!datos || n == 0) {
        void** arr = malloc(n * sizeof(void*));
        if (!arr) {
            free(heap);
            return NULL;
        }

        for (size_t i=0; i<n; i++) {
            arr[i] = NULL;
        }
 
        heap->arr = arr;
        heap->cantidad = 0;
        heap->capacidad = n;
    } else {
        size_t nueva_capacidad = FACTOR_CARGA * n;

        heap->arr = arreglo_copia_crear(datos, n, nueva_capacidad);
        if (!heap->arr) {
            free(heap);
            return NULL;
        }

        //heapify(heap->arr); // ordenamos en forma de heap la copia del arreglo que nos pasaron

        heap->cantidad = n;
        heap->capacidad = nueva_capacidad;
    }

    heap->cmp = cmp;

    return heap;
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


heap_t *heap_crear(cmp_func_t cmp) {
    return heap_crear_2(NULL, CAPACIDAD_INICIAL, cmp);
}


heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    return heap_crear_2(arreglo, n, cmp);
}


size_t heap_cantidad(const heap_t *heap) {
    if (!heap || !heap->arr) return 0;
    return heap->cantidad;
}


bool heap_esta_vacio(const heap_t *heap) {
    return heap_cantidad(heap) == 0;
}


void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    return heap->arr[0];
}


bool heap_encolar(heap_t *heap, void *elem) {
    //...
    return false;
}


void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    void* maximo = heap->arr[0];

    // desencolar...

    return maximo;
}


void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    if (!heap) return;

    if (!heap->arr) {
        free(heap);
        return;
    }

    arreglo_destruir(heap->arr, heap->capacidad, destruir_elemento);
    free(heap);

    return;
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

    if (!cmp || !elementos || cant == 0) return;

    // ordenamiento...
}
