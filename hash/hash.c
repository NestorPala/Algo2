#include "hash.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"


const size_t CAPACIDAD_INICIAL = 23; //numero primo
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
    const hash_t* hash;
    lista_iter_t* campo_actual;
    size_t lista_actual;
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

    free(hash_nuevo->tabla);
    lista_t** nueva_tabla = malloc(nueva_capacidad * sizeof(lista_t*));
    if (!nueva_tabla) return NULL;
    for (size_t i=0; i<nueva_capacidad; i++) nueva_tabla[i] = NULL;

    hash_nuevo->tabla = nueva_tabla;
    hash_nuevo->capacidad = nueva_capacidad;
    hash_nuevo->cantidad = hash->cantidad;

    for (size_t i=0; i<hash->capacidad; i++) {

        if (!hash->tabla[i]) continue;

        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        if(!iter){
            free(hash_nuevo->tabla);
            hash_destruir(hash_nuevo);
            return NULL;
        }

        while(!lista_iter_al_final(iter)) {
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            if (!hash_guardar(hash_nuevo, campo_actual->clave, campo_actual->dato)){
                free(hash_nuevo->tabla);
                hash_destruir(hash_nuevo);
                lista_iter_destruir(iter);
                return NULL;
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
        size_t nueva_capacidad = FACTOR_REDIMENSION * hash->capacidad;
        hash_t* hash_nuevo = hash_redimensionar(hash, nueva_capacidad);
        if (!hash_nuevo) return false;
        //hash_destruir(hash);
        hash = hash_nuevo;
    }

    bool clave_ya_estaba = false;

    hg* guardador = guardador_crear();
    campo_t* campo = hash_buscar(hash, clave, false, NULL, guardador);
    if (campo) clave_ya_estaba = true;

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

    if (!clave_ya_estaba) hash->cantidad++;
    free(guardador);
    return hash_pertenece(hash, clave);
}


void *hash_borrar(hash_t *hash, const char *clave) {
    if (!hash || !clave) return NULL;
    void* dato_borrado = NULL; 

    size_t indice = hash_(clave, hash->capacidad);
    if (!hash->tabla[indice]) return NULL;

    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    if (!iter) return NULL;

    bool encontrado = false;

    while (!lista_iter_al_final(iter)) {
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if (strcmp(campo_actual->clave, clave) == 0) {
            encontrado = true;

            if (hash->destruir_dato) {
                hash->destruir_dato(campo_actual->dato);
            } else {
                dato_borrado = campo_actual->dato;
            }

            free(campo_actual->clave);
            free(campo_actual);
            lista_iter_borrar(iter);
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);

    if (lista_esta_vacia(hash->tabla[indice])) {
        lista_destruir(hash->tabla[indice], NULL);
        hash->tabla[indice] = NULL;
    }

    if (encontrado) hash->cantidad--;
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


void hash_destruir(hash_t *hash) {
    if (!hash) return;

    for (size_t i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) {
            continue;
        } 
        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        if (!iter) return;

        while(!lista_iter_al_final(iter)) {
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            if(hash->destruir_dato) hash->destruir_dato(campo_actual->dato);
            free(campo_actual->clave);
            free(campo_actual);
            lista_iter_borrar(iter);
        }
        lista_iter_destruir(iter);
    }

    free(hash->tabla);
    free(hash);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;

    lista_iter_t* campo_actual = NULL;
    int lista_actual = -1;

    for (int i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) continue;
        lista_actual = i;
        campo_actual = lista_iter_crear(hash->tabla[i]);
        if (!campo_actual) {
            free(iter);
            return NULL;
        }
        break;
    }

    if (lista_actual == -1) {
        free(iter);
        return NULL;
    }

    iter->hash = hash;
    iter->campo_actual = campo_actual;
    iter->lista_actual = lista_actual;

    return iter;
}


bool hash_iter_avanzar(hash_iter_t *iter) {

    lista_iter_avanzar(iter->campo_actual);

    if (lista_iter_al_final(iter->campo_actual)) {
        for (size_t i=iter->lista_actual+1; i<iter->hash->capacidad; i++) {
            if (!iter->hash->tabla[i]) continue;
            lista_iter_t* nuevo_iter = lista_iter_crear(iter->hash->tabla[i]);
            if (!nuevo_iter) return false;
            lista_iter_destruir(iter->campo_actual);
            iter->campo_actual = nuevo_iter;
            iter->lista_actual = i;
            return true;
        }
    }

    return false;  
}


const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    campo_t* campo_actual = lista_iter_ver_actual(iter->campo_actual);
    const char* clave = strdup(campo_actual->clave);
    return clave;
}


bool hash_iter_al_final(const hash_iter_t *iter) {

    for (size_t i=iter->lista_actual; i<iter->hash->capacidad; i++) {
        if (!iter->hash->tabla[i]) continue;
        else return false;
    }
    return true;
}


void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->campo_actual);
    free(iter);
}



