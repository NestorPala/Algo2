#include <stdio.h>


// https://www.youtube.com/watch?v=YzHDIvxOQcI


// AUXILIAR 
void imprimir_vector(int vector[], size_t n) {
    printf("\n[");
    for (size_t i = 0; i < n; i++)
        printf ("%d, ", vector[i]);
    printf("]\n");
}


void swap(int arreglo[], size_t a, size_t b) {
    int aux = 0;
    aux = arreglo[a];
    arreglo[a] = arreglo[b];
    arreglo[b] = aux;
}


size_t ubicar_pivote(int arreglo[], size_t inicio, size_t fin) {
    while (inicio < fin) {
        while (arreglo[fin] >= arreglo[inicio] && inicio < fin) {
            fin--;
        }
        swap(arreglo, fin, inicio);

        while (arreglo[inicio] <= arreglo[fin] && inicio < fin) {
            inicio++;
        }
        swap(arreglo, inicio, fin);
    }
    return inicio;
}


void quicksort(int arreglo[], size_t inicio, size_t fin) {
    if (inicio < fin) {
        size_t pivote = ubicar_pivote(arreglo, inicio, fin);
        quicksort(arreglo, inicio, pivote - 1);
        quicksort(arreglo, pivote + 1, fin);
    }
}


// int main() {

//     int numeros[] = {32, 54, 65, 45, 3, 4, 87, 6, 57, 89, 10, 0, -12, -7, -23, -6};
//     size_t n = 16;
    
//     printf("QUICKSORT\n");
//     imprimir_vector(numeros, n-1);
//     quicksort(numeros, 0, n-1);
//     imprimir_vector(numeros, n-1);

//     return 0;
// }
