#include <stdio.h>


//https://www.youtube.com/watch?v=kOgzXagXpTg


// AUXILIAR 
void imprimir_vector_(int vector[], size_t n) {
    printf("\n[");
    for (size_t i = 0; i < n; i++)
        printf ("%d, ", vector[i]);
    printf("]\n");
}


void merge(int arreglo[], size_t inicio, size_t medio, size_t fin) {
    size_t izq = inicio;
    size_t der = medio + 1;
    size_t ia = 0;
    int auxiliar[fin - inicio];

    while (izq <= medio && der <= fin) {
        if (arreglo[izq] < arreglo[der]) {
            auxiliar[ia] = arreglo[izq];
            izq++;
            ia++;
        } else {
            auxiliar[ia] = arreglo[der];
            der++;
            ia++;
        }
    }

    for (size_t k=der; k<=fin; k++) {
        auxiliar[ia] = arreglo[k];
        ia++;
    }

    for (size_t k=izq; k<=medio; k++) {
        auxiliar[ia] = arreglo[k];
        ia++;
    }

    for (size_t k=0; k<ia; k++) {
        arreglo[inicio + k] = auxiliar[k];
    }
}


void mergesort(int arreglo[], size_t inicio, size_t fin) {
    if (inicio < fin) {
        size_t medio = (inicio + fin) / 2;
        mergesort(arreglo, inicio, medio);
        mergesort(arreglo, medio + 1, fin);
        merge(arreglo, inicio, medio, fin);
    }
}


// int main() {

//     int numeros[] = {32, 54, 65, 45, 3, 4, 87, 6, 57, 89, 10, 0, -12, -7, -23, -6};
//     size_t n = 16;

//     printf("MERGESORT\n");
//     imprimir_vector_(numeros, n-1);
//     mergesort(numeros, 0, n-1);
//     imprimir_vector_(numeros, n-1);

//     return 0;
// }
