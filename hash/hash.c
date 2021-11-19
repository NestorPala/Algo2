#include "hash.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"


const size_t CAPACIDAD_INICIAL = 59; //numero primo
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
    int lista_actual;  //vale -1 si el hash esta vacio
};


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


// AUXILIAR
hash_t* hash_crear_2(size_t capacidad, hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) {
        return NULL;
    }

    lista_t** tabla = malloc(capacidad * sizeof(lista_t*));
    if (!tabla) {
        free(hash);
        return NULL;
    }

    for (size_t i = 0; i < capacidad; i++) {
        tabla[i] = NULL;
    }
    
    hash->tabla = tabla;
    hash->cantidad = 0;
    hash->capacidad = capacidad;
    hash->destruir_dato = destruir_dato;

    return hash;
}


// Definimos que la posicion esta vacia cuando es NULL
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    return hash_crear_2(CAPACIDAD_INICIAL, destruir_dato);
}


// AUXILIAR
campo_t* campo_crear(const char* clave, void* dato) {
    campo_t* campo = malloc(sizeof(campo_t));
    if(!campo) {
        return NULL;
    }
    campo->clave = strdup(clave);
    campo->dato = dato;
    return campo;
}


// AUXILIAR
void campo_destruir(campo_t* campo, hash_destruir_dato_t destruir_dato) {
    if (!campo) return;
    free(campo->clave);
    if (destruir_dato) {destruir_dato(campo->dato);}
    free(campo);
}


// AUXILIAR 
lista_iter_t* hash_buscar(const hash_t* hash, const char* clave, int* estado_busqueda) {

    size_t x = hash_(clave, hash->capacidad);

    if (!hash->tabla[x]) {
        if (estado_busqueda) {
            *estado_busqueda = 1;
        }
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(hash->tabla[x]);
    if (!iter) {
        if (estado_busqueda) {
            *estado_busqueda = -1;
        }
        return NULL;
    }

    while(!lista_iter_al_final(iter)){
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if( strcmp(campo_actual->clave, clave) == 0) {
            if (estado_busqueda) {
                *estado_busqueda = 2;
            }
            return iter;
        }
        lista_iter_avanzar(iter);
        if (lista_iter_al_final(iter) && estado_busqueda) {
            *estado_busqueda = 3;
        }
    }
    lista_iter_destruir(iter);

    return NULL;
}


// AUXILIAR
// #include <stdio.h> //debug  
hash_t* hash_redimensionar(hash_t* hash, size_t nueva_capacidad) {
    // printf("\n------------------- REDIMENSIONAMOS -------------------\n\n");  ////debug

    // hash_t* nuevo_hash = hash_crear_2(nueva_capacidad, hash->destruir_dato);
    // if (!nuevo_hash) {
    //     return NULL;
    // }

    // hash_iter_t* iter = hash_iter_crear(hash);
    // if (!iter) {
    //     hash_destruir(nuevo_hash);
    //     return NULL;
    // }

    // while(!hash_iter_al_final(iter)) {
    //     const char* clave_actual = hash_iter_ver_actual(iter);
    //     void* dato_actual = hash_obtener(hash, clave_actual);

    //     hash_guardar(nuevo_hash, clave_actual, dato_actual);
    //     hash_iter_avanzar(iter);
    // }
    // hash_iter_destruir(iter);

    // return nuevo_hash;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

    // printf("CAPACIDAD DEL HASH: %zu\n", hash->capacidad); ////////////

    if (!hash || !clave) {
        return false;
    }

    if (hash->cantidad == FACTOR_CARGA * hash->capacidad) {
        size_t nueva_capacidad = FACTOR_REDIMENSION * hash->capacidad;

        hash_t* nuevo_hash = hash_redimensionar(hash, nueva_capacidad);

        if (!nuevo_hash){
            return false;
        }

        hash_t* aux_hash = hash;
        *hash = *nuevo_hash;
        // hash_destruir(aux_hash); //
    }

    bool clave_ya_estaba = false;
    int estado_busqueda = 0;
    lista_iter_t* iter =  NULL;

    if (hash->cantidad != 0) {  // Si el hash esta vacio no busco nada
        iter = hash_buscar(hash, clave, &estado_busqueda);
        if (iter) {
            clave_ya_estaba = true;
        }
    } else {
        estado_busqueda = 1;
    }

    size_t x = hash_(clave, hash->capacidad);

    campo_t* nuevo_campo = campo_crear(clave, dato);
    if (!nuevo_campo) {
        if (iter) lista_iter_destruir(iter);
        return false;
    }

    if (estado_busqueda == 1) {  //la posicion X de la tabla no contiene una lista, por ende, la clave no está
        lista_t* lista = lista_crear();
        if (!lista) {
            return false;
        }
        if (!lista_insertar_primero(lista, nuevo_campo)) {
            campo_destruir(nuevo_campo, hash->destruir_dato);
            lista_destruir(lista, NULL);
            return false;
        }
        hash->tabla[x] = lista;
    } else if (estado_busqueda == 2) {  //la posicion X de la tabla contiene una lista, y se encontró la clave en esa lista
        campo_t* viejo_campo = lista_iter_ver_actual(iter);
        campo_destruir(viejo_campo, hash->destruir_dato);
        lista_iter_borrar(iter);
        lista_iter_insertar(iter, nuevo_campo);
        lista_iter_destruir(iter);
    } else if (estado_busqueda == 3) { //la posicion X de la tabla contiene una lista, y NO se encontró la clave en esa lista
        if (!lista_insertar_ultimo(hash->tabla[x], nuevo_campo)) {
            campo_destruir(nuevo_campo, hash->destruir_dato);
            return false;
        }
    } else {
        if (iter) lista_iter_destruir(iter);
        return false;
    }

    if (!clave_ya_estaba) { hash->cantidad++; }
    return true;
}


void *hash_borrar(hash_t *hash, const char *clave) {

    // AGREGAR REDIMENSION LUEGO DE BORRAR CIERTA CANTIDAD DE ELEMENTOS

    if (!hash || !clave) {
        return NULL;
    }

    if (hash->cantidad == 0) {
        return NULL;
    }

    void* dato_borrado = NULL; 

    size_t x = hash_(clave, hash->capacidad);
    if (!hash->tabla[x]) {
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(hash->tabla[x]);
    if (!iter) {
        return NULL;
    }

    bool encontrado = false;

    while (!lista_iter_al_final(iter)) {
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if (!campo_actual) {
            lista_iter_destruir(iter);
            return false;
        }
        if (strcmp(campo_actual->clave, clave) == 0) {
            encontrado = true;
            dato_borrado = campo_actual->dato;
            campo_destruir(campo_actual, hash->destruir_dato);
            lista_iter_borrar(iter);
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);

    if (lista_esta_vacia(hash->tabla[x])) {
        lista_destruir(hash->tabla[x], NULL);
        hash->tabla[x] = NULL;
    }

    if (encontrado) { hash->cantidad--; }
    return dato_borrado;
}


void *hash_obtener(const hash_t *hash, const char *clave) {
    if (!hash || !clave) {
        return NULL;
    }
    lista_iter_t* iter = hash_buscar(hash, clave, NULL);
    if (!iter) return NULL;
    void* dato = ((campo_t*)lista_iter_ver_actual(iter))->dato;
    lista_iter_destruir(iter);
    return dato;
}


bool hash_pertenece(const hash_t *hash, const char *clave) {
    if (!hash || !clave) return false;
    lista_iter_t* iter = hash_buscar(hash, clave, NULL);
    if (!iter) return false;
    char* clave2 = ((campo_t*)lista_iter_ver_actual(iter))->clave;
    lista_iter_destruir(iter);
    return clave2 ? true : false;
}


size_t hash_cantidad(const hash_t *hash) {
    if (!hash) {
        return 0;
    }
    return hash->cantidad;
}


void hash_destruir(hash_t *hash) {
    if (!hash) {
        return;
    }

    if (hash->cantidad == 0) {
        free(hash->tabla);
        free(hash);
        return;
    }

    for (size_t i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) {
            continue;
        }
        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        if (!iter) {
            return;
        }

        while(!lista_iter_al_final(iter)) {
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            campo_destruir(campo_actual, hash->destruir_dato);
            lista_iter_borrar(iter);
        }
        lista_iter_destruir(iter);
        lista_destruir(hash->tabla[i], NULL);
        hash->tabla[i] = NULL;
    }

    free(hash->tabla);
    free(hash);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


hash_iter_t *hash_iter_crear(const hash_t *hash) {

    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }

    lista_iter_t* campo_actual = NULL;
    int lista_actual = -1;

    for (int i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) {
            continue;
        }
        lista_actual = i;
        campo_actual = lista_iter_crear(hash->tabla[i]);
        if (!campo_actual) {
            free(iter);
            return NULL;
        }
        break;
    }

    iter->hash = hash;
    iter->campo_actual = campo_actual;
    iter->lista_actual = lista_actual;

    return iter;
}


bool hash_iter_avanzar(hash_iter_t *iter) {

    if (!iter || !iter->hash || iter->hash->cantidad == 0) {
        return false;
    } 

    lista_iter_avanzar(iter->campo_actual);

    if (lista_iter_al_final(iter->campo_actual)) {
        for (int i=iter->lista_actual+1; i<iter->hash->capacidad; i++) {
            if (!iter->hash->tabla[i]) {
                continue;
            }
            lista_iter_t* nuevo_iter = lista_iter_crear(iter->hash->tabla[i]);
            if (!nuevo_iter) {
                return false;
            }
            lista_iter_destruir(iter->campo_actual);
            iter->campo_actual = nuevo_iter;
            iter->lista_actual = i;
            return true;
        }
        return false;  
    }
    return true;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (!iter || !iter->hash || iter->hash->cantidad == 0) {
        return NULL;
    }
    campo_t* campo_actual = lista_iter_ver_actual(iter->campo_actual);
    const char* clave = NULL;
    if (!hash_iter_al_final(iter)) {
        clave = campo_actual->clave;
    }
    return clave;
}


bool hash_iter_al_final(const hash_iter_t *iter) {

    if (!iter || !iter->hash || iter->hash->cantidad == 0) {
        return true;
    }

    if (lista_iter_al_final(iter->campo_actual)) {
        for (int i=iter->lista_actual; i<iter->hash->capacidad; i++) {
            if (!iter->hash->tabla[i]) {
                continue;
            }
            else {
                return true;
            } 
        }
    }
    return false;
}


void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->campo_actual);
    free(iter);
}
