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

/*
int maximo_2(int vector[], int inicio, int final) {

	int medio, a, b, max;

	if (inicio == final) {
		return vector[inicio];
	}

	medio = (inicio + final) / 2;

	a = maximo_2(vector, 0, medio);
	b = maximo_2(vector, medio + 1, final);
	
	max = (a > b) ? a : b;

	return max;
}
*/

int maximo(int vector[], int n) {
	
	/*
	if (n == 0) return -1;
	return maximo_2(vector, 0, n);*/

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


/*
//PRUEBA INTERNA (LUEGO SE USA tp0_pruebas.c)
int main(void) {
	
	int i;
	int e1 = 111, e2 = 222;
	
	printf("%d %d", e1, e2);
	swap(&e1, &e2);
	printf("\n%d %d", e1, e2);
	
	////////////////////////////////////////////////////////////////
	
	int numeros[] = {23, 4, -16, 0, 343, 4, -12, 0, 54};
	printf("\n\nLa posicion del vector donde se encuentra el maximo es: %d\n\n", maximo(numeros, len(numeros)));
	
	////////////////////////////////////////////////////////////////
	
	seleccion(numeros, len(numeros));
	
	for (i=0; i<len(numeros); i++) {
		printf("%d\t", numeros[i]);
	}
	
	////////////////////////////////////////////////////////////////
	
	
	int a[] = {1,2,3,4,5,6};
	int b[] = {1,2,3,4,5};
	
	int c = comparar(a, len(a), b, len(b));
	printf("\n\n\n%d", c);

	////////////////////////////////////////////////

	int vec3[] = { 5, 8, 9 };
	int vec4[] = { 5, 8, 9, 10, 3};

	int c2 = comparar(vec3, len(vec3), vec4, len(vec4));

	int i;
	printf("\n");
	for(i=0; i<len(vec4); i++) {
		printf("%d\t", vec3[i]);
	}
	printf("\n");
	for(i=0; i<len(vec4); i++) {
		printf("%d\t", vec4[i]);
	}

	printf("\n\n\n%d", c2);


	
	return 0;
}
*/


/*
int main(void) {

	int pepe[] = {4, 3, 99, 22222, 6, 0, -9};
	printf("\n\n%d", maximo(pepe, 7));
	
}
*/
