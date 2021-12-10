#include "heap.h"
#include <stdlib.h>
#include <stddef.h>


const size_t CAPACIDAD_INICIAL_HEAP = 10;
const size_t FACTOR_CARGA_HEAP      =  2;    // Se utiliza para las redimensiones del Heap


typedef void (*destr_t)(void *e);       // Funcion de destruccion de dato


struct heap {
	void** arr;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};


// AUXILIAR
// Permite redimensión "hacia abajo" y "hacia arriba".
// Devuelve una copia en memoria dinámica del arreglo, con la capacidad nueva_capacidad. 
// Si la nueva capacidad es igual a la vieja capacidad, el arreglo no se copia.
// Pre: el arreglo existe.
// Post: el arreglo original no se modifica.
void** arreglo_copiar_arreglo(void** datos, size_t vieja_capacidad, size_t nueva_capacidad) {

    void** nuevo_arreglo = malloc(nueva_capacidad * sizeof(void*));

    if (!nuevo_arreglo) {
        return NULL;
    }

    if (vieja_capacidad == nueva_capacidad) {
        return datos;
    } else if (vieja_capacidad < nueva_capacidad) {
        for (size_t i=0; i<vieja_capacidad; i++) {
            nuevo_arreglo[i] = datos[i];
        }

        for (size_t i=vieja_capacidad; i<nueva_capacidad; i++) {
            nuevo_arreglo[i] = NULL;
        }
    } else {
        for (size_t i=0; i<nueva_capacidad; i++) {
            nuevo_arreglo[i] = datos[i];
        }
    }

    return nuevo_arreglo;
}


// AUXILIAR
// Destruye el arreglo y todo su contenido, si la función destruir_dato fue especificada.
// Pre: el arreglo existe.
// Post: el arreglo ya no existe.
void arreglo_destruir(void** datos, size_t n, destr_t destruir_dato) {

    if (!datos || n == 0) {
        return;
    }

    if (destruir_dato) {
        for (size_t i=0; i<n; i++) {
            destruir_dato(datos[i]);
        }
    }

    free(datos);
}


// AUXILIAR
// Intercambia dos posiciones de un arreglo.
// Pre: las dos posiciones son válidas.
// Post: la cantidad de elementos del arreglo no se modifica, pero sí su ordenamiento.
void arreglo_swap(void** datos, size_t a, size_t b) {
    void* aux = NULL;
    aux = datos[a];
    datos[a] = datos[b];
    datos[b] = aux;
}


// AUXILIAR
// Realiza la operación de "down-heap" a partir de una posición 'padre' del arreglo, 
// mientras éste (a partir de 'padre') no cumpla con la condición de heap.
// Pre: el arreglo existe.
// Post: el arreglo cumple (a partir de 'padre') con la condición de heap.
void arreglo_downheap(void** datos, size_t cantidad, size_t padre, cmp_func_t cmp) {                    // DOWNHEAP
    size_t izq = 2 * padre + 1;
	size_t der = 2 * padre + 2;
    size_t max = 0;

    if (izq < cantidad 
            && datos[izq] 
            && datos[padre] 
            && cmp(datos[izq], datos[padre]) > 0) {
        max = izq;
    } else {
        max = padre;
    }

    if (der < cantidad 
            && datos[der] 
            && datos[max] 
            && cmp(datos[der], datos[max]) > 0) {
        max = der;
    }

    // Chequeamos la condicion de Heap
    if (max != padre) {
		arreglo_swap(datos, padre, max);
		arreglo_downheap(datos, cantidad, max, cmp);
    }
}


// AUXILIAR                                                                                         
// Realiza la operación de "up-heap" a partir de una posición 'pos_hijo' del arreglo, 
// mientras éste (a partir de 'pos_hijo') no cumpla con la condición de heap.
// Pre: el arreglo existe.
// Post: el arreglo cumple (a partir de 'pos_hijo') con la condición de heap.
void arreglo_upheap(heap_t* heap, size_t pos_hijo) {                                                    // UPHEAP
    
    // Caso base: llego al inicio del arreglo
    if (pos_hijo == 0) {
        return; 
    }

    // La posicion del arreglo donde se encuentra el "nodo padre"
    size_t padre = (pos_hijo - 1) / 2;

    // Chequeamos la condicion de Heap
    if (heap->cmp(heap->arr[padre], heap->arr[pos_hijo]) < 0) {
        arreglo_swap(heap->arr, padre, pos_hijo);
        arreglo_upheap(heap, padre);
    }
} 


// AUXILIAR
// Convierte un arreglo en su totalidad en un heap, aplicando desde la mitad del mismo, en reversa, 
// hasta la primera posición, la operación "down-heap" en cada posicion del arreglo.
// Pre: el arreglo existe.
// Post: el arreglo en su totalidad cumple con la condición de heap.
void arreglo_heapify(void** datos, size_t n, cmp_func_t cmp) {                                          // HEAPIFY
    for (int i = ((int)n) / 2;  i >= 0;  i--) {
        arreglo_downheap(datos, n, (size_t)i, cmp);
    }
}


// AUXILIAR
// Agranda o achica el heap según sea necesario o no. No se crea un heap nuevo. No se borra el heap original.
// Pre: el heap existe.
// Post: el arreglo original del heap ya no existe. Su memoria ha sido borrada y en su lugar se encuentra un nuevo arreglo.
void heap_redimensionar(heap_t* heap, float carga, float redimension) {

    if ( (float)(heap->cantidad)  !=  carga * (float)(heap->capacidad) ) {
        return;
    }

    void** aux = heap->arr;

    size_t nueva_capacidad = (size_t)(redimension * (float)(heap->capacidad));
    heap->arr = arreglo_copiar_arreglo(heap->arr, heap->capacidad, nueva_capacidad);

    if (heap->arr) {
        free(aux);
        heap->capacidad = nueva_capacidad;
    } else {
        heap->arr = aux;
    }
}


// AUXILIAR 
// Modularización de heap_crear() para su uso en la funcion en heap_crear_arr().
// Devuelve un heap completo o NULL en caso de error.
// Post: el arreglo original (si existía) no fue modificado.
heap_t* heap_crear_general(void *arr[], size_t n, cmp_func_t cmp) {

    if (!cmp) {
        return NULL;
    }

    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) {
        return NULL;
    }

    size_t nueva_capacidad = arr ? (FACTOR_CARGA_HEAP * n) 
                                 : CAPACIDAD_INICIAL_HEAP;

    void** datos = arr ? arreglo_copiar_arreglo(arr, n, nueva_capacidad)
                       : malloc(nueva_capacidad * sizeof(void*));

    if (!datos) {
        free(heap);
        return NULL;
    }

    if (arr) {
        arreglo_heapify(datos, n, cmp);
    } else {
        for (size_t i=0; i<nueva_capacidad; i++) {
            datos[i] = NULL;
        }
    }

    heap->arr = datos;
    heap->cantidad = n;
    heap->capacidad = nueva_capacidad;
    heap->cmp = cmp;

    return heap;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


heap_t *heap_crear(cmp_func_t cmp) {
    return heap_crear_general(NULL, 0, cmp);
}


heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    return heap_crear_general(arreglo, n, cmp);
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

    if (!heap) {
        return;
    }

    if (!heap->arr) {
        free(heap);
        return;
    }

    arreglo_destruir(heap->arr, heap->capacidad, destruir_elemento);
    free(heap);
}


bool heap_encolar(heap_t *heap, void *elem) {

    // Redimensionamos el arreglo si es necesario
    float carga        =  1;
    float redimension  =  (float)FACTOR_CARGA_HEAP;
    heap_redimensionar(heap, carga, redimension);
    
    // Guardamos el elemento y lo "ordenamos" como un heap
    heap->arr[heap->cantidad] = elem;
    arreglo_upheap(heap, heap->cantidad);

    heap->cantidad++;
    return true;
}


void *heap_desencolar(heap_t *heap) {

    if (heap_esta_vacio(heap)) {
        return NULL;
    }

    //Redimensionamos el arreglo si es necesario
    float carga         =   ( 1  / ( 2 * (float)FACTOR_CARGA_HEAP ) );
    float redimension   =   ( 1  / (     (float)FACTOR_CARGA_HEAP ) );
    heap_redimensionar(heap, carga, redimension);

    // Elemento a devolver
    void* maximo = heap->arr[0];

    // Borramos el primero
    size_t primero = 0;
    size_t ultimo  = heap->cantidad - 1;
    arreglo_swap(heap->arr, primero, ultimo);
    heap->arr[ultimo] = NULL;

    // Aplicamos ordenamiento tipo heap
    arreglo_downheap(heap->arr, heap->cantidad, 0, heap->cmp);

    heap->cantidad--;
    return maximo;
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

    if (!cmp || !elementos || cant == 0) {
        return;
    }

    arreglo_heapify(elementos, cant, cmp);
    size_t aux = cant;

    for (size_t i=0; i<cant-1; i++) {
        arreglo_swap(elementos, 0, cant-1-i);
        aux--;
        arreglo_downheap(elementos, aux, 0, cmp);
    }
}
