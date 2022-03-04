#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>


void imprimir_arreglo(int arr[], size_t inicio, size_t fin) {

    printf("\n\n[");

    for(size_t i=inicio; i<fin + 1; i++) {
        if (i < fin) {
            printf("%d, ", arr[i]);
        } else {
            printf("%d", arr[i]);
        }
    }

    printf("]\n\n");
}


bool es_potencia_de_2(size_t n) {

    if (n == 0) {
        return false;
    }

    while (n != 1) {
        if (n % 2 != 0) {
            return false;
        }
        n = n / 2;
    }

    return true;
}


void swap(int arr[], size_t pos_a, size_t pos_b) {
    int aux = 0;
    aux = arr[pos_a];
    arr[pos_a] = arr[pos_b];
    arr[pos_b] = aux;
}


void intercalar_arreglo_2n_(int arr[], size_t inicio, size_t fin) {

    size_t n = fin - inicio + 1;

    if (n == 4) {
        swap(arr, inicio + 1, fin - 1);
        return;
    }

    for (size_t i = inicio + n/4; i < inicio + n/2; i++) {
        swap(arr, i, i + n/4);
    }

    size_t medio = (inicio + fin) / 2;

    intercalar_arreglo_2n_(arr, inicio, medio);
    intercalar_arreglo_2n_(arr, medio + 1, fin);
}


bool intercalar_arreglo_2n(int arr[], size_t n) {

    if (!es_potencia_de_2(n)) {
        return false;
    }

    if (n == 1 || n == 2) {
        return true;
    }

    intercalar_arreglo_2n_(arr, 0, n - 1);

    return true;
}


int main() {

    int numeros[] = {11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 23, 24, 25, 26, 27, 28};
    size_t n = 16;

    if (intercalar_arreglo_2n(numeros, n)) {
        imprimir_arreglo(numeros, 0, n - 1);
    } else {
        printf("Error: La cantidad de elementos del arreglo no es potencia de 2\n");
    }

    return 0;
}
