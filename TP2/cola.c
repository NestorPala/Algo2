#include "cola.h"
#include <stdlib.h>
#include <stddef.h>


typedef struct nodo nodo_t;

struct nodo {
    nodo_t* siguiente_nodo;
    void* dato;
};

struct cola {
    nodo_t* primer_elemento;
    nodo_t* ultimo_elemento;
};


/***********************************************************************************/


cola_t *cola_crear(void) {

    cola_t* cola = malloc(sizeof(cola_t));

    cola -> primer_elemento  =  NULL;
    cola -> ultimo_elemento  =  NULL;

    return cola;
}


bool cola_esta_vacia(const cola_t *cola) {

    return (cola -> primer_elemento == NULL) && (cola -> ultimo_elemento == NULL);
}


nodo_t* crear_cola_nodo(void* valor) {

    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (nodo == NULL) {
        return NULL;
    }

    // Inicializo el nodo nuevo (que voy a encolar)
    nodo -> siguiente_nodo = NULL;
    nodo -> dato = valor;

    return nodo;
}


bool cola_encolar(cola_t *cola, void *valor) {

    nodo_t* nodo_nuevo = crear_cola_nodo(valor);

    if (nodo_nuevo == NULL) {
        return false;
    }
    
    if (cola_esta_vacia(cola)) {
        cola -> primer_elemento = nodo_nuevo;
    }
    
    if (cola -> ultimo_elemento != NULL) {

        // Apunto el último nodo al nuevo nodo
        cola -> ultimo_elemento -> siguiente_nodo  =  nodo_nuevo;
    }

    // Ahora el "nuevo" nodo es el último nodo
    cola -> ultimo_elemento = nodo_nuevo;

    return true;
}


void *cola_ver_primero(const cola_t *cola) {

    if (cola_esta_vacia(cola)) {
        return NULL;
    }

    return cola -> primer_elemento -> dato;
}


void *cola_desencolar(cola_t *cola) {

    if (cola_esta_vacia(cola)) {
        return NULL;
    }

    // Obtengo el valor del nodo que voy a quitar
    void* tope_anterior_cola  =  cola -> primer_elemento -> dato;

    // Utilizo puntero a nodo auxiliar para no perder la referencia al siguiente elemento
    nodo_t* puntero_siguiente  =  cola -> primer_elemento -> siguiente_nodo;

    // Compruebo caso borde de 1 solo elemento
    if (cola -> primer_elemento == cola -> ultimo_elemento) {
        cola -> ultimo_elemento = NULL;
    }

    // Borro el elemento desencolado
    free(cola -> primer_elemento);

    // Recupero la referencia del elemento siguiente (ahora, primer elemento)
    cola -> primer_elemento  =  puntero_siguiente;

    return tope_anterior_cola;
}


void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {

    while (!cola_esta_vacia(cola)) {

        if (destruir_dato != NULL) {  
            destruir_dato(cola -> primer_elemento -> dato);
        }
        cola_desencolar(cola);
    }
    
    free(cola);
}
