#include <stdio.h>
#include "lista.h"


void swap(int arr[], size_t a, size_t b) {
    int aux = 0;
    aux = arr[a];
    arr[a] = arr[b];
    arr[b] = aux;
}


int comparar_numeros(void* a, void* b) {

    if (*(int*)a == *(int*)b) {
        return 0;
    } else if (*(int*)a < *(int*)b) {
        return -1;
    } else {
        return 1;
    }
}


lista_t* combinados_mayores_a_k(int arr[], size_t cant, size_t k) {

    void** enteros = malloc(cant * sizeof(void*)); //O(1)

    if (!enteros) { //O(1)
        return NULL;
    }

    if (cant == 1) { //O(1)
        return NULL;
    }

    for (size_t i=0; i<cant; i++) { //orden O(cant)
        enteros[i] = &arr[i];
    }

    //O(cant), porque podrian llegar a quedarme solo 2 elementos, que sean ambos menores a k, los combino y me queda un solo elemento
    while (*(int*)enteros[0] < k || *(int*)enteros[1] < k) { 

        // Ordeno el arreglo para sacar los dos elementos mas chicos facilmente  //O(cant), porque suponemos que la implementacion es con downheap
        heapify(arr, cant, comparar_enteros); 

        // Cortamos en caso de que ya no se pueda seguir combinando //O(1)
        if (*(int*)enteros[0] <= k || *(int*)enteros[1] > k) {
            free(enteros);
            return NULL;
        }

        // Generamos el nuevo entero //O(1)
        int combinado = *(int*)enteros[0]  +  2  *  *(int*)enteros[1];

        // Eliminamos los dos numeros "viejos" //O(1)
        swap(arr, 0, cant - 1);
        arr[cant - 1] = NULL;

        swap(arr, 1, cant - 2);
        arr[cant - 2] = NULL;

        // Insertamos el numero nuevo al final y ordenamos de nuevo // O(1)
        arr[cant - 2] = combinado
    }

    // generamos la lista a devolver y la llenamos // O(cant)
    lista_t* lista = lista_crear();

    if (!lista) {
        free(enteros);
        return NULL;
    }

    for (size_t i=0; i<cant; i++) {
        if (enteros[i]) {
            lista_insertar_ultimo(lista, enteros[i]);
        }
    }

    return lista; //O(1)
}



// COMPLEJIDAD DE LA FUNCION: O(N^2), con N = cant, el largo del arreglo que nos pasaron