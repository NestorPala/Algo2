#define _POSIX_C_SOURCE 200809L // strdup
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.h"

#define CAPACIDAD_INICIAL 1024
#define FACTOR_DE_CARGA 0.20
#define FACTOR_REDIMENSION 2
#define NOT_FOUND -1


/* Definición del struct hash, hash_campo e iterador. */

typedef enum {OCUPADO, VACIO, BORRADO} estado_t;

typedef struct hash_campo {
	char* clave;
	void* dato;
	estado_t estado;
} hash_campo_t;

struct hash {
	size_t cantidad;
	size_t capacidad;
	hash_destruir_dato_t destruir_dato;
	hash_campo_t* tabla;
};

struct hash_iter {
	const hash_t* hash;
	size_t indice;
};

/* ******************************************************************
 *                        FUNCION DE HASH
 * *****************************************************************/

// Fuente: Variante de djb2, sacada de http://www.cse.yorku.ca/~oz/hash.html
// Hilo de Reddit que ayudo a entender la funcion y modificarla: https://www.reddit.com/r/cs50/comments/ggvgvm/pset5_djb2_hash_function/ 
// Razon para elegir el unsigned long hash = 5381: http://coddingbuddy.com/article/50817235/reason-for-5381-number-in-djb-hash-function
static unsigned long funcion_hashing(const char* clave) {
	unsigned long hash = 5381;
	int c;

	while ((c = *clave++)) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

 // PRE: Crea una tabla de hash y recibe la capacidad de la tabla.
 // POST: devuelve la tabla de hash creada.

hash_campo_t* hash_crear_tabla(size_t capacidad) {
	hash_campo_t* tabla = malloc(sizeof(hash_campo_t) * capacidad);
	if (!tabla) {
		return NULL;
	}
	return tabla;
}

 // PRE: El hash fue creado.
 // POST: Inicia cada campo de la tabla de hash.
void hash_iniciar_tabla(hash_t* hash) {
	for (size_t i = 0; i < hash->capacidad; i++) {
		hash->tabla[i].clave = NULL;
		hash->tabla[i].dato = NULL;
		hash->tabla[i].estado = VACIO;
	}
}


 // PRE: El hash fue creado y se paso la nueva capacidad
 // POST: Se redimensiona la tabla de hash y retorna true | false segun si tuvo exito.
bool hash_redimensionar(hash_t* hash, size_t nueva_capacidad) {
	hash_campo_t* tabla_aux = hash->tabla;
	size_t capacidad_aux = hash->capacidad;

	hash_campo_t* nueva_tabla = hash_crear_tabla(nueva_capacidad);
	if (!nueva_tabla) {
		return false;
	}

	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;
	hash->cantidad = 0;
	hash_iniciar_tabla(hash);
	for (size_t i = 0; i < capacidad_aux; i++) {
		if (tabla_aux[i].estado == OCUPADO) {
			hash_guardar(hash, tabla_aux[i].clave, tabla_aux[i].dato);
			// Libero la clave almacenada en la tabla antigua.
			free(tabla_aux[i].clave);
		}
	}
	free(tabla_aux);
	return true;
}



// PRE: Recibe el hash, la clave a buscar y el bool de campo ocupado.
// POST: Obtiene el indice de la clave dentro del hash, o el indice de un lugar vacio.
// 		Retorna el indice correspondiente o -1 si no fue encontrada la clave.
size_t hash_obtener_indice(const hash_t* hash, const char* clave, bool buscar_ocupado) {
	size_t indice = funcion_hashing(clave);

	for (size_t i = 0; i < hash->capacidad; i++) {
		indice = (indice + i * i) % hash->capacidad;
		if (buscar_ocupado && hash->tabla[indice].estado == OCUPADO && strcmp(clave, hash->tabla[indice].clave) == 0) {
			return indice;
		}
		if (!buscar_ocupado && hash->tabla[indice].estado != OCUPADO) {
			return indice;
		}
	}

	return NOT_FOUND;
}


// PRE: Recibe el hash, la clave, el dato y el indice.
// POST: COmpleta un campo dentro del hash.
void hash_completar_campo(hash_t* hash, const char* clave, void* dato, size_t indice) {
	char* clave_aux = strdup(clave);
	hash->tabla[indice].clave = clave_aux;
	hash->tabla[indice].dato = dato;
	hash->tabla[indice].estado = OCUPADO;
	hash->cantidad++;
}


// PRE: Recibe la clave allocada, el dato y el destruir_dato.
// POST: Libera la memoria de la clave y destruye el dato del campo indicado.
void hash_destruir_campo(char* clave, void* dato, hash_destruir_dato_t destruir_dato) {
	free(clave);
	if (destruir_dato) {
		destruir_dato(dato);
	}
}

// PRE: Recibe el hash, el dato y el indice.
// POST: Actualiza el dato de un campo del hash.
void hash_actualizar_campo(hash_t* hash, void* dato, size_t indice) {
	if (hash->destruir_dato) {
		hash->destruir_dato(hash->tabla[indice].dato);
	}
	hash->tabla[indice].dato = dato;
}

// PRE: REcibe el iterador creado previamente.
// POST: Avanza el iterador hasta el siguiente campo, retorna true si se logro posicionar
//	en un campo ocupado o el final del hash.
bool hash_iter_actualizar_siguiente_posicion(hash_iter_t* iter) {
	for (; iter->indice <= iter->hash->capacidad; iter->indice++) {
		if (iter->indice == iter->hash->capacidad || iter->hash->tabla[iter->indice].estado == OCUPADO) {
			return true;
		}
	}

	return false;
}


/*******************************************************************
 *                      PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) {
		return NULL;
	}

	hash->tabla = hash_crear_tabla(CAPACIDAD_INICIAL);
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

	hash->cantidad = 0;
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->destruir_dato = destruir_dato;
	hash_iniciar_tabla(hash);

	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if ((float) hash->cantidad / (float) hash->capacidad >= (float) FACTOR_DE_CARGA) {
		if(!hash_redimensionar(hash, hash->capacidad * FACTOR_REDIMENSION)) {
			return false;
		}
	}

	size_t indice = funcion_hashing(clave) % hash->capacidad;

	if (hash->tabla[indice].estado != OCUPADO) {
		hash_completar_campo(hash, clave, dato, indice);
	} else if (hash->tabla[indice].estado == OCUPADO && strcmp(hash->tabla[indice].clave, clave) == 0) {
		hash_actualizar_campo(hash, dato, indice);
	} else {
		indice = hash_obtener_indice(hash, clave, false);
		hash_completar_campo(hash, clave, dato, indice);
	}
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	if (!hash_pertenece(hash, clave)) {
		return NULL;
	}

	size_t indice = hash_obtener_indice(hash, clave, true);
	void* dato_aux = hash->tabla[indice].dato;
	hash->tabla[indice].dato = NULL;
	hash->tabla[indice].estado = VACIO;
	free(hash->tabla[indice].clave);
	hash->cantidad--;

	return dato_aux;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	if (!hash_pertenece(hash, clave)) {
		return NULL;
	}

	size_t indice = hash_obtener_indice(hash, clave, true);

	return hash->tabla[indice].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	if (hash_obtener_indice(hash, clave, true) == NOT_FOUND) {
		return false;
	}
	return true;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla[i].estado == OCUPADO) {
			hash_destruir_campo(hash->tabla[i].clave, hash->tabla[i].dato, hash->destruir_dato);
		}
	}
	free(hash->tabla);
	free(hash);
}



/* *****************************************************************
 *                   PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) {
		return NULL;
	}

	iter->hash = hash;
	iter->indice = 0;
	if (!hash_iter_actualizar_siguiente_posicion(iter)) {
		iter->indice = iter->hash->capacidad;
	}
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) {
		return false;
	}
	iter->indice++;
	return hash_iter_actualizar_siguiente_posicion(iter);
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {

	if (hash_iter_al_final(iter)) {
		return NULL;
	}
	return iter->hash->tabla[iter->indice].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->hash->capacidad == iter->indice;
}

void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}