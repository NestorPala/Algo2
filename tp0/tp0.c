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

	int indice_minimo;

	for (int i=0; i<n-1; i++) {

		indice_minimo = i;

		for (int j=indice_minimo+1; j<n; j++) {

			if (vector[j] < vector[indice_minimo]) {
				indice_minimo = j;
			}

		}

		swap(&vector[i], &vector[indice_minimo]);
	}
}


/*
//PRUEBAS INTERNAS
int main() {

	int vec1[] = {3, 5, 4, 2, 1}, vec1_ord[] = {1, 2, 3, 4, 5};
	int vec2[] = {4, 8, 15, 16, 23, 42}, vec2_ord[] = {4, 8, 15, 16, 23, 42};
	int vec3[] = {-38, -46, -65, -78}, vec3_ord[] = {-78, -65, -46, -38};

	seleccion(vec3, 4);

	for (int i=0; i<4; i++) {
		printf("%d\t", vec3[i]);
	}

	return 0;
}
*/
