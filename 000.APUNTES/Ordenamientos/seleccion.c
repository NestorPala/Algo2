#include <stdio.h>


// AUXILIAR 
void imprimir_vector__(int vector[], size_t n) {
    printf("\n[");
    for (size_t i = 0; i < n; i++)
        printf ("%d, ", vector[i]);
    printf("]\n");
}


void swap_(int arreglo[], size_t a, size_t b) {
    int aux = 0;
    aux = arreglo[a];
    arreglo[a] = arreglo[b];
    arreglo[b] = aux;
}


int maximo(int vector[], size_t n) {

	if (n == 0) {
	    return -1;
	}

    int maximo = vector[0], posicion_maximo = 0;

	for (int i = 1; i < (int)n; i++) {
		if (vector[i] > maximo) {
			maximo = vector[i];
			posicion_maximo = i;
		}
	}

	return posicion_maximo;
}


/* Tuvimos que implementar seleccion al revés (desde el final hacia el principio del vector)
    para poder usar la función maximo(), ya que los elementos más grandes se colocarán al final */
void seleccion(int vector[], size_t n) {
	size_t indice_maximo = 0;

	for (size_t i=n-1; i>=0; i--) {
		indice_maximo = maximo(vector, i+1);

        if (indice_maximo == -1) {
            return;
        }

		swap_(vector, i, indice_maximo);
	}
}


// int main() {

//     int numeros[] = {32, 54, 65, 45, 3, 4, 87, 6, 57, 89, 10, 0, -12, -7, -23, -6};
//     size_t n = 16;
    
//     printf("SELECCION\n");
//     imprimir_vector__(numeros, n-1);
//     seleccion(numeros, n);
//     imprimir_vector__(numeros, n-1);

//     return 0;
// }
