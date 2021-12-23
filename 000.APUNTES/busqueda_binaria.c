#include <stdio.h>


int busqueda_binaria_(int arreglo[], size_t inicio, size_t fin, int buscado) {

    if (inicio >= fin) {
        if (arreglo[inicio] == buscado) {
            return (int)inicio;
        } else {
            return -1; //no se encontró el elemento
        }
    }

    size_t medio = (inicio + fin) / 2;

    if (arreglo[medio] == buscado) {
        return (int)medio;
    } else if (arreglo[medio] > buscado) {
        return busqueda_binaria_(arreglo, inicio, medio - 1, buscado);
    } else {
        return busqueda_binaria_(arreglo, medio + 1, fin, buscado);
    }
} 


int busqueda_binaria(int arreglo[], size_t n, int buscado) {
    return busqueda_binaria_(arreglo, 0, n, buscado);
}


// int main() {
//     int arreglo[] = {-9, -7, -1, 0, 12, 24, 25, 35, 40, 41, 46, 59, 63, 67, 69, 76, 87, 100};
//     printf("El indice del elemento es: %d\n", busqueda_binaria(arreglo, 18, 0));
//     return 0;
// }
