#include "003_hash_con_iterador_interno.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "003_lista.h"


enum estado_busqueda_en_hash {
    // Sea X el indice arrojado por la funcion de hashing

    // Hubo errores de creacion de variables en la busqueda de la clave
    BUSQUEDA_FALLO, 

    //la posicion X de la tabla no contiene una lista, por ende, la clave no está
    NO_HAY_LISTA_EN_INDICE_X, 

    //la posicion X de la tabla contiene una lista, y se encontró la clave en esa lista
    CLAVE_ENCONTRADA,

    //la posicion X de la tabla contiene una lista, y NO se encontró la clave en esa lista
    CLAVE_NO_ENCONTRADA 
};


const size_t CAPACIDAD_INICIAL = 19; //numero primo
const size_t FACTOR_CARGA = 2;
const int TABLA_HASH_VACIA = -1; //Valor por defecto del iterador de hash


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
    int lista_actual;
};


//https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f
unsigned long hash_(const char* clave, size_t largo) {

    unsigned long hash = 5381;
    int c;

    while ((c = *clave++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % largo;
}


// AUXILIAR
// Permite crear un hash especificando el tamaño inicial de la tabla de hash que esta contiene. 
// Se utiliza para el proceso de redimensión especifícamente. 
// Si se especificase capacidad 0 no se pedirá memoria dinámica, y la tabla de hash se tendrá que crear y asignar manualmente luego.
hash_t* hash_crear_dimensionar_tabla(size_t capacidad, hash_destruir_dato_t destruir_dato) {

    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    lista_t** tabla_hash = NULL;

    if (capacidad != 0) {
        tabla_hash = malloc(capacidad * sizeof(lista_t*));
        if (!tabla_hash) {
            free(hash);
            return NULL;
        }
        for (size_t i = 0; i < capacidad; i++) {
            tabla_hash[i] = NULL;
        }
    }
    
    hash->tabla = tabla_hash;
    hash->cantidad = 0;
    hash->capacidad = capacidad;
    hash->destruir_dato = destruir_dato;

    return hash;
}


// Definimos que una posición de la tabla de hash está vacía cuando su contenido es NULL (no se guardan listas vacias).
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    return hash_crear_dimensionar_tabla(CAPACIDAD_INICIAL, destruir_dato);
}


// AUXILIAR
// Contiene el par clave-valor de un elemento que el usuario guarda en el Hash.
campo_t* campo_crear(const char* clave, void* dato) {
    campo_t* campo = malloc(sizeof(campo_t));
    campo->clave = strdup(clave);
    campo->dato = dato;
    return campo;
}


// AUXILIAR
// Destruye la clave y el dato (si el usuario lo especificó) y luego libera la memoria del campo.
void campo_destruir(campo_t* campo, hash_destruir_dato_t destruir_dato) {
    if (!campo) return;

    if (destruir_dato) {
        destruir_dato(campo->dato);
    }

    free(campo->clave);
    free(campo);
}


// AUXILIAR 
// Busca dentro del hash la clave especificada por el usuario, y si la encuentra 
// devuelve un acceso directo a la variable que la contiene, para luego poder realizar operaciones con ella.
// Ademas, encuentre o no la clave, devuelve el estado de la búsqueda.
lista_iter_t* hash_buscar(const hash_t* hash, const char* clave, int* estado_busqueda) {

    size_t x = hash_(clave, hash->capacidad);

    if (!hash->tabla[x]) {
        if (estado_busqueda) {
            *estado_busqueda = NO_HAY_LISTA_EN_INDICE_X;
        }
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(hash->tabla[x]);
    if (!iter) {
        if (estado_busqueda) {
            *estado_busqueda = BUSQUEDA_FALLO;
        }
        return NULL;
    }

    while(!lista_iter_al_final(iter)){
        campo_t* campo_actual = lista_iter_ver_actual(iter);
        if( strcmp(campo_actual->clave, clave) == 0) {
            if (estado_busqueda) {
                *estado_busqueda = CLAVE_ENCONTRADA;
            }
            return iter;
        }
        lista_iter_avanzar(iter);
    }

    if (lista_iter_al_final(iter) && estado_busqueda) {
        *estado_busqueda = CLAVE_NO_ENCONTRADA;
    }

    lista_iter_destruir(iter);

    return NULL;
}


// AUXILIAR
// Destruye ordenadamente todos y cada uno de los elementos de la tabla de Hash y luego libera la memoria de la misma.
bool tabla_destruir(lista_t** tabla_hash, size_t capacidad, hash_destruir_dato_t destruir_dato) {

    if (!tabla_hash) return false;

    for (size_t i=0; i<capacidad; i++) {
        if (!tabla_hash[i]) continue;

        lista_iter_t* iter = lista_iter_crear(tabla_hash[i]);
        if (!iter) {
            return false;
        }

        while(!lista_iter_al_final(iter)) {
            campo_t* campo_actual = lista_iter_ver_actual(iter);
            campo_destruir(campo_actual, destruir_dato);
            lista_iter_borrar(iter);
        }

        lista_iter_destruir(iter);
        lista_destruir(tabla_hash[i], NULL);
        tabla_hash[i] = NULL;
    }

    free(tabla_hash);
    tabla_hash = NULL;

    return true;
}


// AUXILIAR 
// Crea un nuevo hash que contiene una tabla con la dimensión nueva, 
// y lo va llenando con los elementos del hash viejo a medida que recorre éste.
// No se destruye el hash viejo dentro de la función.
hash_t* hash_redimensionar_trasladar_campos(hash_t* hash, size_t nueva_capacidad) {

    hash_t* nuevo_hash = hash_crear_dimensionar_tabla(nueva_capacidad, hash->destruir_dato);
    if (!nuevo_hash) {
        return NULL;
    }

    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter) {
        hash_destruir(nuevo_hash);
        return NULL;
    }

    while(!hash_iter_al_final(iter)) {
        const char* clave_actual = hash_iter_ver_actual(iter);
        void* dato_actual = hash_obtener(hash, clave_actual);

        hash_guardar(nuevo_hash, clave_actual, dato_actual);
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);

    return nuevo_hash;
}


// AUXILIAR 
// Permite agrandar o achicar el tamaño de la tabla de Hash para optimizar las busquedas posteriormente.
void hash_redimensionar(hash_t* hash, float carga, float redimension) {
    
    if ((float)(hash->cantidad) != carga * (float)(hash->capacidad)) return;

    size_t nueva_capacidad = (size_t)(redimension * (float)(hash->capacidad));
    hash_t* nuevo_hash = hash_redimensionar_trasladar_campos(hash, nueva_capacidad);

    if (!nuevo_hash) return;

    if (!tabla_destruir(hash->tabla, hash->capacidad, NULL)){   //borrar tabla vieja
        hash_destruir(nuevo_hash);
        return;
    }
    
    hash->capacidad = nuevo_hash->capacidad;
    hash->tabla = nuevo_hash->tabla;
    free(nuevo_hash);
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

    if (!hash || !clave) {
        return false;
    }

    float carga = (float)FACTOR_CARGA, redimension = (float)(FACTOR_CARGA * 2);
    hash_redimensionar(hash, carga, redimension);

    bool clave_ya_estaba = false;
    int estado_busqueda = 0;
    lista_iter_t* iter =  NULL;

    if (hash->cantidad != 0) {  // Si el hash esta vacio no busco nada
        iter = hash_buscar(hash, clave, &estado_busqueda);
        if (iter) {
            clave_ya_estaba = true;
        }
    } else {
        estado_busqueda = NO_HAY_LISTA_EN_INDICE_X;
    }

    size_t x = hash_(clave, hash->capacidad);

    campo_t* nuevo_campo = campo_crear(clave, dato);
    if (!nuevo_campo) {
        if (iter) lista_iter_destruir(iter);
        return false;
    }

    if (estado_busqueda == NO_HAY_LISTA_EN_INDICE_X) {  

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

    } else if (estado_busqueda == CLAVE_ENCONTRADA) {  

        campo_t* viejo_campo = lista_iter_ver_actual(iter);
        campo_destruir(viejo_campo, hash->destruir_dato);
        lista_iter_borrar(iter);
        lista_iter_insertar(iter, nuevo_campo);
        lista_iter_destruir(iter);

    } else if (estado_busqueda == CLAVE_NO_ENCONTRADA) { 

        if (!lista_insertar_ultimo(hash->tabla[x], nuevo_campo)) {
            campo_destruir(nuevo_campo, hash->destruir_dato);
            return false;
        }

    } else {
        if (iter) lista_iter_destruir(iter);
        return false;
    }

    if (!clave_ya_estaba) { 
        hash->cantidad++; 
    }

    return true;
}


void *hash_borrar(hash_t *hash, const char *clave) {

    if (!hash || !clave || hash->cantidad == 0) {
        return NULL;
    }

    float carga = (1/(2 * (float)FACTOR_CARGA)), redimension = (1/((float)FACTOR_CARGA));
    hash_redimensionar(hash, carga, redimension);

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
            campo_destruir(campo_actual, NULL);
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

    if (encontrado) { 
        hash->cantidad--; 
    }

    return dato_borrado;
}


void *hash_obtener(const hash_t *hash, const char *clave) {
    if (!hash || !clave) return NULL;

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

    char* clave_hallada = ((campo_t*)lista_iter_ver_actual(iter))->clave;
    
    lista_iter_destruir(iter);
    return clave_hallada ? true : false;
}


size_t hash_cantidad(const hash_t *hash) {
    if (!hash) return 0;
    return hash->cantidad;
}


void hash_destruir(hash_t *hash) {
    if (!hash) return;

    if (hash->cantidad == 0) {
        free(hash->tabla);
        free(hash);
        return;
    }

    tabla_destruir(hash->tabla, hash->capacidad, hash->destruir_dato);
    free(hash);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void hash_iterar(hash_t* hash, visit_f visitar, void* extra) {

    if (!hash || 
        !hash->tabla || 
        hash->capacidad == 0 || 
        hash->cantidad == 0 || 
        !visitar) {
        return;
    }

    for (size_t i=0; i<hash->capacidad; i++) {
        if (!hash->tabla[i]) {
            continue;
        }

        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);

        while(!lista_iter_al_final(iter)) {
            char* clave = ((campo_t*) lista_iter_ver_actual(iter)) -> clave;
            void* dato = ((campo_t*) lista_iter_ver_actual(iter)) -> dato;

            if (!visitar(clave, dato, extra)) {
                return;
            }

            lista_iter_avanzar(iter);
        }

        lista_iter_destruir(iter);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


hash_iter_t *hash_iter_crear(const hash_t *hash) {

    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }

    lista_iter_t* campo_actual = NULL;
    int lista_actual = TABLA_HASH_VACIA;

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
