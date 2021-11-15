#include "hash.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"


const size_t CAPACIDAD_INICIAL = 1; //numero primo
const size_t FACTOR_CARGA = 2;
const size_t FACTOR_REDIMENSION = 2;


struct hash {
    lista_t** tabla;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
};

typedef struct campo {
    char* clave;
    void* dato;
} campo_t;

struct hash_iter {
    campo_t* campo_anterior;
    campo_t* campo_actual;
    hash_t* hash;
};


// AUXILIAR 
typedef struct hash_guardador {
	size_t estado_busqueda; //1: vacio, 2: ocupado o 3: lleno y no encontrado
	campo_t** encontrado; //estado 2
} hg;


/* crea una copia (memoria dinamica) de la cadena pasada por parametro */
char* strdup(const char* org) {
    char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
    if (!cpy) return NULL;
    strcpy(cpy, org);
    return cpy;
}


//https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f
unsigned long hash_(const char* clave, size_t largo) {

    unsigned long hash = 5381;
    int c;

    while ((c = *clave++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % largo;
}


// Definimos que la posicion esta vacia cuando es NULL
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    lista_t** tabla = malloc(CAPACIDAD_INICIAL * sizeof(lista_t*));

    if (!tabla) {
        free(hash);
        return NULL;
    }

    for (size_t i = 0; i < CAPACIDAD_INICIAL; i++) {
        tabla[i] = NULL;
    }
    
    hash->tabla = tabla;
    hash->cantidad = 0;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->destruir_dato = destruir_dato;

    return hash;
}


// AUXILIAR
campo_t* campo_crear(const char* clave, void* dato) {
    campo_t* campo = malloc(sizeof(campo_t));
    if(!campo) return NULL;
    campo->clave = strdup(clave);
    campo->dato = dato;
    return campo;
}


// AUXILIAR 
campo_t* hash_buscar(const hash_t* hash, const char* clave, bool borrar, void** borrado, hg* guardador) {

    size_t indice = hash_(clave, hash->capacidad);

    if (!hash->tabla[indice]) {
        if (guardador) guardador->estado_busqueda = 1;
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    if (!iter) return NULL;

    while(!lista_iter_al_final(iter)){
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if( strcmp(campo_actual->clave, clave) == 0) {
            if (borrar) {
                *borrado = campo_actual->dato;
                if (hash->destruir_dato) { hash->destruir_dato(campo_actual->dato);}
                free(campo_actual->clave);
                free(campo_actual);
                lista_iter_borrar(iter);
                lista_iter_destruir(iter);
                return NULL;
            }
            if (guardador) {
                guardador->estado_busqueda = 2;
                guardador->encontrado = &campo_actual;
            }
            lista_iter_destruir(iter);
            return campo_actual;
        }
        lista_iter_avanzar(iter);
        if (lista_iter_al_final(iter) && guardador) guardador->estado_busqueda = 3;
    }
    lista_iter_destruir(iter);

    return NULL;
}


// AUXILIAR  
hash_t* hash_redimensionar(hash_t* hash, size_t nueva_capacidad) {
    hash_t* hash_nuevo = hash_crear(hash->destruir_dato);
    if (!hash_nuevo) return NULL;

    lista_t** nueva_tabla = realloc(hash_nuevo->tabla, nueva_capacidad);
    if (!nueva_tabla) return NULL;

    hash_nuevo->tabla = nueva_tabla;
    hash_nuevo->capacidad = nueva_capacidad;
    hash_nuevo->cantidad = hash->cantidad;

    for (size_t i=0; i<hash->capacidad; i++) {

        if (!hash->tabla[i]) {
            continue;
        } 

        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        if(!iter){
            hash_destruir(hash_nuevo);
            return NULL;
        }

        while(!lista_iter_al_final(iter)) {
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            if (!hash_guardar(hash_nuevo, campo_actual->clave, campo_actual->dato)){
                hash_destruir(hash_nuevo);
                lista_iter_destruir(iter);
                return hash;
            }
            lista_iter_avanzar(iter);
        }
        lista_iter_destruir(iter);
    }

    return hash_nuevo;
}


// AUXILIAR
hg* guardador_crear() {
    hg* guardador = malloc(sizeof(hg));
    if (!guardador) return NULL;
    guardador->estado_busqueda = 0;
    guardador->encontrado = NULL;
    return guardador;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (!hash || !clave) return false;

    if (hash->cantidad == FACTOR_CARGA * hash->capacidad) {
        size_t vieja_capacidad = hash->capacidad;
        size_t nueva_capacidad = FACTOR_REDIMENSION * hash->capacidad;
        hash_t* hash_nuevo = hash_redimensionar(hash, nueva_capacidad);
        if (!hash_nuevo) {
            return false;
        }
        //hash_destruir(hash);
        hash = hash_nuevo;
    }

    hg* guardador = guardador_crear();
    hash_buscar(hash, clave, false, NULL, guardador);

    if (guardador->estado_busqueda == 1) {
        size_t indice = hash_(clave, hash->capacidad);
        lista_t* lista = lista_crear();
        if (!lista) return false;
        campo_t* campo = campo_crear(clave, dato);
        if (!campo) {
            free(lista);
            return false;
        }
        if (!lista_insertar_primero(lista, campo)) {
            free(campo);
            free(lista);
            return false;
        }
        hash->tabla[indice] = lista;

    } else if (guardador->estado_busqueda == 2) {

        campo_t** c1 = guardador->encontrado;
        if(hash->destruir_dato) {
            hash->destruir_dato((*c1)->dato);
        }
        (*c1)->dato = dato;

    } else {
        size_t indice = hash_(clave, hash->capacidad);
        lista_t* lista_indice = hash->tabla[indice];
        campo_t* campo = campo_crear(clave, dato);
        if (!campo) return false;
        if (!lista_insertar_ultimo(lista_indice, campo)) return false;
    }

    hash->cantidad++;
    free(guardador);
    return true;
}


void *hash_borrar(hash_t *hash, const char *clave) {
    if (!hash || !clave) return NULL;
    void* dato_borrado = NULL;
    hash_buscar(hash, clave, true, &dato_borrado, NULL);
    hash->cantidad--;
    return dato_borrado;
}


void *hash_obtener(const hash_t *hash, const char *clave) {
    if (!hash || !clave) return NULL;
    campo_t* campo = hash_buscar(hash, clave, false, NULL, NULL);
    return campo ? campo->dato : NULL;
}


bool hash_pertenece(const hash_t *hash, const char *clave) {
    if (!hash || !clave) return false;
    return hash_buscar(hash, clave, false, NULL, NULL) ? true : false;
}


size_t hash_cantidad(const hash_t *hash) {
    if (!hash) return 0;
    return hash->cantidad;
}


// AUXILIAR 
void lista_i_destruir(lista_t* lista, hash_destruir_dato_t destruir_dato) {
    while(!lista_esta_vacia(lista)) {
        campo_t* campo_actual = lista_ver_primero(lista);
        if (destruir_dato) {
            destruir_dato(campo_actual->dato);
        }
        free((void*)campo_actual->clave);
        free(campo_actual);
        lista_borrar_primero(lista);
    }
    lista_destruir(lista, NULL);
}


void hash_destruir(hash_t *hash) {
    if (!hash) return;

    for (size_t i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) {
            continue;
        } 
        lista_i_destruir(hash->tabla[i], hash->destruir_dato);
    }

    free(hash->tabla);
    free(hash);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




