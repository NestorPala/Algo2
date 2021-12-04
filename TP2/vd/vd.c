#include <stddef.h>
#include "vd.h"
		

struct vector {
    void** datos;
	size_t cantidad;
	size_t capacidad;
};


bool vd_redimensionar(vd_t* vector, size_t capacidad) {

	//...
	return true;
}


vd_t* vd_crear(size_t capacidad) {

    vd_t *vector = malloc(sizeof(vd_t));

    if (!vector) {
        return NULL;
    }

    void** datos = malloc(capacidad * sizeof(void*));

    if (!datos) {
        free(vector);
        return NULL;
    }

	for (size_t i=0; i<capacidad; i++) {
		datos[i] = NULL;
	}

	vector->datos = datos;
    vector->cantidad = 0;
	vector->capacidad = capacidad;

    return vector;
}


bool vd_guardar(vd_t* vector, size_t pos, void* dato) {
	if (!vector || pos >= vector->capacidad) return false;
	if (vector->datos[pos]) vector->cantidad++;
	vector->datos[pos] = dato;
	return true;
}


void* vd_obtener(vd_t* vector, size_t pos) {
	return (!vector || pos >= vector->cantidad) ? NULL : vector->datos[pos];
}


size_t vd_largo(vd_t* vector) {
	return vector ? vector->cantidad : 0;
}


bool vd_esta_vacio(vd_t* vector) {
	return vector ? vector->cantidad == 0 : true;
}


void vd_destruir(vd_t* vector) {
	free(vector->datos);
	free(vector);
}
