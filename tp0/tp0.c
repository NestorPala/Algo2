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
	
	if (n != 0) {
		maximo = vector[0];
		posicion_maximo = 0;
		
		for (i=1; i<n; i++) {
			if (vector[i] > maximo) {
				maximo = vector[i];
				posicion_maximo = i;
			}
		}
		
		return posicion_maximo;
	}
	
	return -1;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
	
	int n3, i, vector_mayor = 0;
	
	n3 = (n1 <= n2) ? n1 : n2;

	for (i=0; i<n3; i++) {
		if (vector1[i] != vector2[i]) {
			if (vector1[i] < vector2[i]) {
				vector_mayor = -1;
			}
			else {
				vector_mayor = 1;
			}
			break;
		}
	}
	
	if (vector_mayor == 0) {
		if (n1 != n2) {
			if (n1 > n2) return 1;
			else return -1;
		}
		else return 0;
	}
	else return vector_mayor;
}


void seleccion(int vector[], int n) {
	int separador, i, j;	
	
	for (i=0; i<n; i++) {
		separador = i;

		for (j=i+1; j<n; j++) {
			if (vector[j] <= vector[separador]) {
				swap(&vector[j], &vector[separador]);
			}
		}
	}
}
