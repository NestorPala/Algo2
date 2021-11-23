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
void** arreglo_copiar_arreglo(void** datos, size_t vieja_capacidad, size_t nueva_capacidad) {

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
void arreglo_swap(void** datos, size_t a, size_t b) {
    void* aux = NULL;
    aux = datos[a];
    datos[a] = datos[b];
    datos[b] = aux;
}


// AUXILIAR 
void arreglo_downheap(void** datos, size_t padre, cmp_func_t cmp) {
    //...
}


// AUXILIAR
void arreglo_upheap(void** datos, size_t hijo, cmp_func_t cmp) {
    //...
} 


// AUXILIAR
void arreglo_heapify(void** datos, cmp_func_t cmp) {
    //...
}


// AUXILIAR
void heap_redimensionar(heap_t* heap, float carga, float redimension) {

    if ((float)(heap->cantidad) <= carga * (float)(heap->capacidad)) return;  // el original es "!="

    size_t nueva_capacidad = (size_t)(redimension * (float)(heap->capacidad));

    void** aux = heap->arr;
    heap->arr = arreglo_copiar_arreglo(heap->arr, heap->capacidad, nueva_capacidad);

    if (heap->arr) {
        free(aux);
        heap->capacidad = nueva_capacidad;
    } else {
        heap->arr = aux;
    }
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

        heap->arr = arreglo_copiar_arreglo(datos, n, nueva_capacidad);
        if (!heap->arr) {
            free(heap);
            return NULL;
        }

        // ordenamos en forma de heap la copia del arreglo que nos pasaron
        arreglo_heapify(heap->arr, heap->cmp); 

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


bool heap_encolar(heap_t *heap, void *elem) {
    if (heap_esta_vacio(heap)) return false;

    // Redimensionamos el arreglo si es necesario
    float carga = (float)FACTOR_CARGA, redimension = (float)(FACTOR_CARGA * 2);
    heap_redimensionar(heap, carga, redimension);
    
    // Guardamos el elemento y lo "ordenamos"
    heap->arr[heap->cantidad] = elem;
    arreglo_upheap(heap->arr, heap->cantidad, heap->cmp);

    heap->cantidad++;

    return true;
}


void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    void* maximo = heap->arr[0];

    // Redimensionamos el arreglo si es necesario
    float carga = (1/(2 * (float)FACTOR_CARGA)), redimension = (1/((float)FACTOR_CARGA));
    heap_redimensionar(heap, carga, redimension);

    // Borramos el primero
    size_t primero = 0, ultimo = heap->cantidad - 1;
    arreglo_swap(heap->arr, primero, ultimo);
    heap->arr[ultimo] = NULL;

    // "Ordenamos"
    arreglo_downheap(heap->arr, 0, heap->cmp);

    heap->cantidad--;
    return maximo;
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

    if (!cmp || !elementos || cant == 0) return;

    // ordenamiento...
}
