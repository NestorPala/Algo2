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

    // Donde comienza la mitad izquierda a ordenar
    size_t izq = inicio;

    // Donde comienza la mitad derecha a ordenar
    size_t der = medio + 1;

    // Donde guardaremos temporalmente los elementos ordenados
    int auxiliar[fin - inicio];

    // Índice auxiliar para recorrer el arreglo auxiliar
    size_t ia = 0;

    // Mientras no se complete alguna de las 2 listas (izquierda o derecha)
    while (izq <= medio && der <= fin) {

        // Agarramos el elemento actual de la lista izquierda y el elem. actual de la lista derecha.
        // El que sea mayor lo appendeamos a la lista auxiliar, 
        // corremos el puntero de esa lista un lugar hacia adelante,
        // y luego corremos un lugar hacia adelante el índice auxiliar
        // para poder seguir colocando elementos.
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

    // Cuando alguna de las 2 listas se complete, la que continue 
    // sin completar el ciclo, terminará de llenar la lista auxiliar,
    // ya que todos los elementos restantes son mayores que los anteriores.

    // Llenamos la lista auxiliar 
    // con los elementos que sobraron en la lista derecha
    // (si es que sobraron)
    for (size_t k=der; k<=fin; k++) {
        auxiliar[ia] = arreglo[k];
        ia++;
    }

    // Llenamos la lista auxiliar 
    // con los elementos que sobraron en la lista izquierda
    // (si es que sobraron)
    for (size_t k=izq; k<=medio; k++) {
        auxiliar[ia] = arreglo[k];
        ia++;
    }

    // Copiamos los datos al arreglo original
    for (size_t k=0; k<ia; k++) {
        arreglo[inicio + k] = auxiliar[k];
    }
}


// MERGESORT (TOP-DOWN)
// Complejidad: O(N * log N), con N la cantidad de elementos del arreglo.
// Es estable: SÍ
// Espacio: O(N)
void mergesort(int arreglo[], size_t inicio, size_t fin) {

    // Si la lista tiene más de un elemento la ordenamos,
    // ya que si contiene un solo elemento ya se encuentra ordenada.
    if (inicio < fin) {
        size_t medio = (inicio + fin) / 2;

        // Ordenamos cada mitad de la lista por separado
        // (Pasamos la lista por referencia)

        // Mitad izquierda
        mergesort(arreglo, inicio, medio);

        // Mitad derecha
        mergesort(arreglo, medio + 1, fin);

        // Mergeamos las dos listas ordenadas para
        // obtener la lista ordenada final.
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
