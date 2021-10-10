
/*
Implementar un algoritmo en C que reciba un arreglo de enteros de tamaño n, ordenado ascendentemente y sin elementos repetidos, y determine en O(log n) si es mágico. Un arreglo es mágico si existe algún valor i tal que 0 <= i y arr[i] = i. Justificar el orden del algoritmo.

Ejemplos:

A = [ -3, 0, 1, 3, 7, 9 ] es mágico porque A[3] = 3.

B = [ 1, 2, 4, 6, 7, 9 ] no es mágico porque B[i] != i para todo i.

*/



//#include "main.h" // No borrar

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>


bool subarreglo_es_magico(int arr[], size_t inicio, size_t fin) {

    // Caso base
    if (inicio == fin) {
        return arr[inicio] == inicio;
    }

    // División
    size_t medio = (inicio + fin) / 2;

    // Parte recursiva
    return subarreglo_es_magico(arr, inicio, medio) || subarreglo_es_magico(arr, medio+1, fin);
}


bool arreglo_es_magico(int arr[], size_t n) {

    return subarreglo_es_magico(arr, 0, n-1);
}


int main() {
    int pepe[5] = {87, 1, 4, 15, 0};

    arreglo_es_magico(pepe, 5) ? printf("TRUE\n") : printf("FALSE\n");

    return 0;
}
