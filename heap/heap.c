#include "heap.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h> //strcpy(), strcmp()
#include <stdio.h> //DEBUG


const size_t CAPACIDAD_INICIAL = 7;  // = 20; //arreglar luego redimension
const size_t FACTOR_CARGA = 2;


typedef void (*destr_t)(void *e);


// Implementamos el Heap de Mínimos
struct heap {
	void** arr;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};


// DEBUG
void heap_debug(void** datos) {
    size_t n = 7;

    printf("ESTADO ACTUAL DEL ARREGLO:\n");

    printf("[");

    for (size_t i=0; i<n; i++) {
        datos[i] ? printf("%d,  ", *(int*)datos[i]) : printf("NULL  ");
    }
    printf("]");

    printf("\n------------------------------------------------------------------------------\n");
}


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
size_t arreglo_calcular_minimo(void** datos, cmp_func_t cmp, size_t padre, size_t izq, size_t der) {
    void* min = datos[padre];
    size_t pos_min = padre;

    if (datos[izq] && cmp(datos[izq], min) < 0) {
        min = datos[izq];
        pos_min = izq;
    }

    if (datos[der] && cmp(datos[der], min) < 0) {
        min = datos[der];
        pos_min = der;
    }

    return pos_min;
}


// AUXILIAR 
void arreglo_downheap(void** datos, size_t cantidad, size_t padre, cmp_func_t cmp) {

    // Caso base: llego al final del arreglo
    if (padre == cantidad) return;

    // Calculamos la posicion de los hijos
    size_t izq = 2 * padre + 1;
	size_t der = 2 * padre + 2;

    // Calculamos el hijo mas chico
    size_t min = arreglo_calcular_minimo(datos, cmp, padre, izq, der);

    // Chequeamos la condicion de Heap
    if (min != padre) {
		arreglo_swap(datos, padre, min);
		arreglo_downheap(datos, cantidad, padre, cmp);
    }
}


// AUXILIAR
void arreglo_upheap(void** datos, size_t hijo, cmp_func_t cmp) {
    
    // Caso base: llego al inicio del arreglo
    if (hijo == 0) return; 

    // La posicion del arreglo donde se encuentra el "nodo padre"
    size_t padre = (hijo - 1) / 2;

    // Chequeamos la condicion de Heap
    if (cmp(datos[padre], datos[hijo]) > 0) {
        arreglo_swap(datos, padre, hijo);
        arreglo_upheap(datos, padre, cmp);
    }
} 


// AUXILIAR
void arreglo_heapify(void** datos, size_t n, cmp_func_t cmp) {

    for (size_t i=n; i<=0; i--) {
        arreglo_downheap(datos, n-i, i, cmp);
    }
}


// AUXILIAR
void heap_redimensionar(heap_t* heap, float carga, float redimension) {

    if ((float)(heap->cantidad) != carga * (float)(heap->capacidad)) return;

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
        arreglo_heapify(heap->arr, n, heap->cmp); 

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
    // float carga = (1/(2 * (float)FACTOR_CARGA)), redimension = (1/((float)FACTOR_CARGA));
    // heap_redimensionar(heap, carga, redimension);

    heap_debug(heap->arr); //debug

    // Borramos el primero
    size_t primero = 0, ultimo = heap->cantidad - 1;
    arreglo_swap(heap->arr, primero, ultimo);
    heap->arr[ultimo] = NULL;

    // "Ordenamos"
    arreglo_downheap(heap->arr, heap->cantidad, 0, heap->cmp);

    heap->cantidad--;
    return maximo;
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

    if (!cmp || !elementos || cant == 0) return;

    // ordenamiento...
}
