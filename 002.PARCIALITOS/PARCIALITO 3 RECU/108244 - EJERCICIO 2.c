// Orden del algoritmo: O(N + K * LOG N), siendo N cantidad de héroes en total y 
// K la cantidad de héroes que se quieren reclutar.

// Utilizamos un heap de máximos en donde la función de comparación daría como máximo al mejor superhéroe, y como
// mínimo al peor superhéroe.

// Explicación: el primer término del orden corresponde a la operacion HEAP_CREAR_ARR, 
// que internamente debería (asumimos que sí) implementar una funcion de "heapify", cuyo costo lineal sea O(N). 
// El segundo término corresponde a desencolar K veces un héroe del heap, operación que cuesta O(log N) debido
// a que el heap contiene a los N superhéroes. Todas las demás operaciones cuestan O(1) así que
// no afectan a la complejidad global.


#include <stdio.h>
#include <heap.h>
#include <heroe.h>


typedef int (*cmp_t)(const void *a, const void *b); //O(1)


heroe_t** obtener_mejores_k_heroes (heroe_t** lista_heroes, size_t cant_heroes, size_t k, cmp_t cmp) { //O(1)

    if (!cmp || !lista_heroes || cant_heroes == 0 || k > cant_heroes || k == 0) { //O(1)
        return NULL; //O(1)
    }

    heap_t* heroes = heap_crear_arr(lista_heroes, cant_heroes, cmp); //O(N)

    if (!heroes) { //O(1)
        return NULL; //O(1)
    }

    heroe_t** mejores_k_heroes = malloc(k * sizeof(heroe_t*)); //O(1)

    if (!mejores_k_heroes) { //O(1)
        heap_destruir(heroes, NULL); //O(N)
        return NULL; //O(1)
    }

    for (size_t i=0; i<k; i++) { //O(K)
        mejores_k_herores[i] = heap_desencolar(heroes); //O(LOG N)
    }

    return mejores_k_heroes; //O(1)
}
