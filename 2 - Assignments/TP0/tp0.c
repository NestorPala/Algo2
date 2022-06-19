#include "tp0.h"
#include <stdio.h>


/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/


void swap(int *x, int *y) {
	
	int aux = 0;

	aux = *x;
	*x = *y;
	*y = aux;
}


int maximo(int vector[], int n) {

	int maximo, posicion_maximo, i;

	if (n == 0) {
	    return -1;
	}

	maximo = vector[0];
	posicion_maximo = 0;

	for (i = 1; i < n; i++) {
		if (vector[i] > maximo) {
			maximo = vector[i];
			posicion_maximo = i;
		}
	}

	return posicion_maximo;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {

	int n3, i;

	n3 = (n1 <= n2) ? n1 : n2;

	for (i=0; i<n3; i++) {
		if (vector1[i] != vector2[i]) {
			if (vector1[i] < vector2[i]) {
				return -1;
			}
			else {
				return 1;
			}
		}
	}

	if (n1 == n2) {
		return 0;
	} else if (n1 > n2) {
		return 1;
	} else {
		return -1;
	}
}


void seleccion(int vector[], int n) {
	/* Tuvimos que implementar seleccion al revés (desde el final hacia el principio del vector)
	   para poder usar la función maximo(), ya que los elementos más grandes se colocarán al final*/

	int indice_maximo;

	for (int i=n-1; i>=0; i--) {
		
		indice_maximo = maximo(vector, i+1);

		swap(&vector[i], &vector[indice_maximo]);
	}
}
