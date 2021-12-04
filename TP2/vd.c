#include <stddef.h>
#include "vd.h"
		

struct vector {
    void** datos;
	size_t cantidad;
	size_t capacidad;
};


// AUXILIAR
// Permite recalcular la cantidad de elementos que no son NULL dentro del arreglo redimensionado
size_t vd_cantidad_recalcular(vd_t* vector) {

	size_t nueva_cantidad = 0;

	for (size_t i=0; i<vector->capacidad; i++) {
		if (vector->datos[i]) {
			nueva_cantidad++;
		}
	}

	return nueva_cantidad;
}


// AUXILIAR
// Permite redimension "hacia abajo" y "hacia arriba"
void** vd_arreglo_copiar(vd_t* vector, size_t nueva_capacidad) {

    void** nuevo_arreglo = malloc(nueva_capacidad * sizeof(void*));

    if (!nuevo_arreglo) {
        return NULL;
    }

    if (nueva_capacidad == vector->capacidad) {
        return vector->datos;
    } else if (vector->capacidad < nueva_capacidad) {
        for (size_t i=0; i<vector->capacidad; i++) {
            nuevo_arreglo[i] = vector->datos[i];
        }

        for (size_t i=vector->capacidad; i<nueva_capacidad; i++) {
            nuevo_arreglo[i] = NULL;
        }
    } else {
        for (size_t i=0; i<nueva_capacidad; i++) {
            nuevo_arreglo[i] = vector->datos[i];
        }
    }

    return nuevo_arreglo;
}


vd_t* vd_crear(size_t capacidad) {

	if (capacidad == 0) {
		return NULL;
	}

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

	if (!vector || pos >= vector->capacidad) {
		return false;
	}

	if (dato) {
		if (!vector->datos[pos]) {
			vector->cantidad++;
		}
	} else {
		if (vector->datos[pos]) {
			vector->cantidad--;
		}
	}

	vector->datos[pos] = dato;

	return true;
}


void* vd_obtener(vd_t* vector, size_t pos, bool* resultado) {

	if (!vector || pos >= vector->capacidad) {
		if (resultado) {
			*resultado = false;
		}
		return NULL;
	}

	if (resultado) {
		*resultado = true;
	}

	return vector->datos[pos];
}


size_t vd_cantidad(vd_t* vector) {
	return vector ? vector->cantidad : 0;
}


bool vd_esta_vacio(vd_t* vector) {
	return vd_cantidad(vector) == 0;
}


size_t vd_largo(vd_t* vector) {
	return vector ? vector->capacidad : 0;
}


bool vd_redimensionar(vd_t* vector, size_t capacidad) {

	if (capacidad == 0 || vd_esta_vacio(vector)) {
		return false;
	}

	if (capacidad == vector->capacidad) {
		return true;
	}

	void** aux_datos = vector->datos;

    vector->datos = vd_arreglo_copiar(vector, capacidad);

    if (vector->datos) {
        free(aux_datos);
        vector->capacidad = capacidad;

		if (capacidad < vector->cantidad) {
			vector->cantidad = vd_cantidad_recalcular(vector);
		}
    } else {
        vector->datos = aux_datos;
    }

	return true;
}


void vd_destruir(vd_t* vector) {
	if (!vector) return;
	free(vector->datos);
	free(vector);
}
