#include "vector.h"


// Funciones a implementar.

// ...

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_destruir(vector_t *vector) {
	if (vector != NULL) {
		free(vector->datos);
		free(vector);
	}
}

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
bool vector_obtener(vector_t *vector, size_t pos, int *valor) {
	if (vector != NULL) {
		if (pos < vector->tam) {
			*valor = vector->datos[pos];
			return true;
		}
	}
	return false;
}

// Almacena el valor en la posición pos
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del vector, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool vector_guardar(vector_t *vector, size_t pos, int valor) {
	if (vector != NULL) {
		if (pos < vector->tam) {
			vector->datos[pos] = valor;
			return true;
		}
	}
	return false;
}

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vector_largo(vector_t *vector) {
	if (vector != NULL) {
		return vector->tam;
	}
	return 0;
}


// Funciones ya implementadas.

vector_t *vector_crear(size_t tam) {
    vector_t *vector = malloc(sizeof(vector_t));

    if (vector == NULL) {
        return NULL;
    }
    vector->datos = malloc(tam * sizeof(int));

    if (tam > 0 && vector->datos == NULL) {
        free(vector);
        return NULL;
    }
    vector->tam = tam;
    return vector;
}

bool vector_redimensionar(vector_t *vector, size_t tam_nuevo) {
    int *datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));

    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}

////////////////////


/*
//PRUEBAS INTERNAS
int main() {
	vector_t* pepe;
	pepe = vector_crear(8);
	
	size_t tamanio = vector_largo(pepe);
	printf("El tamanio del vector es: %d", tamanio);	
	
	bool estado;
	estado = vector_redimensionar(pepe, 9);
	tamanio = vector_largo(pepe);
	printf("\nEl tamanio del vector es: %d", tamanio);
	
	int num = -319;
	bool estado2;
	estado2 = vector_guardar(pepe, 0, num);
	if(estado2) printf("\ntrue"); else printf("\nfalse");
	
	int cajita;
	bool estado3;
	estado3 = vector_obtener(pepe, 0, &cajita);
	if(estado3) printf("\ntrue"); else printf("\nfalse");
	printf("\nEl valor de pepe[0] es: %d", cajita);
	
	////////////
	
	int numeros[] = {23, -26, 0, 0, 2081, 1, 456, 97};
	size_t i;
	bool estado4[8];
	
	for (i=0; i<8; i++) {
		estado4[i] = vector_guardar(pepe, i, numeros[i]);
	}
	
	printf("\n");
    printf("Vector guardar\n");
	
	for (i=0; i<8; i++) {
		if(estado4[i]) printf("true\t"); else printf("false\t");
	}
	
	printf("\n\n");
	printf("Vector obtener\n");
	
	bool estado5;
	int cajita2;
	
	for (i=0; i<8; i++) {
		estado5 = vector_obtener(pepe, i, &cajita2);
		if (estado5) {
			printf("true: %d\n", cajita2);
		}
		else printf("false\n");
	}
	
	////////////
	
	vector_destruir(pepe);
	return 0;
}
*/