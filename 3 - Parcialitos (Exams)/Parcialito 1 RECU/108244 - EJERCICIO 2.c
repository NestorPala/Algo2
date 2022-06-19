#include <stdio.h>


// Complejidad: O(log n), siendo n la cantidad de elementos del arreglo.

// Justificacion: utilizamos un algoritmo de division y conquista. Por lo tanto:
//
// T(n) = A * T(n/B) + O(n^C)
//
// A = 1, porque si bien tenemos dos llamadas recursivas, estas estan dentro de un condicional, por lo tanto solo se llama a una de las dos.
// B = 2, porque en cada llamado utilizamos solo la mitad de elementos que los de la llamada recursiva actual.
// C = 0, porque el costo de todo lo que no son llamados recursivos es O(1) (solo comparamos valores).
//
// Con esta información, utilizando el teorema maestro, llegamos a la conclusion de que el orden del algoritmo es O(log n).


int encontrar_primer_uno_(int arreglo[], int inicio, int fin) {

    if (inicio == fin) {
        return arreglo[inicio] == 1 ? inicio : -1;
    }

    int medio = (inicio + fin) / 2;

    if (arreglo[medio] == 1) {
        if (arreglo[medio - 1] == 0) {
            return medio;
        } else {
            return encontrar_primer_uno_(arreglo, inicio, medio - 1);
        }
    } else {
        if (arreglo[medio + 1] == 1) {
            return medio + 1;
        } else {
            return encontrar_primer_uno_(arreglo, medio + 1, fin);
        }
    }
}


int encontrar_primer_uno(int arreglo[], int n) {
    return encontrar_primer_uno_(arreglo, 0, n);
}


int contar_unos(int numeros[], int n) {
    if (n == 0) {
        return 0;
    }

    int pos_inicio_unos = encontrar_primer_uno(numeros, n);

    if (pos_inicio_unos == -1) {
        return 0;
    }

    return n - pos_inicio_unos;
}
