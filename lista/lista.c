#include "lista.h"
#include <stdlib.h>
#include <stddef.h>


typedef struct nodo nodo_t;

struct nodo {
    nodo_t* siguiente_nodo;
    void* dato;
};

struct lista {
    nodo_t* primer_elemento;
    nodo_t* ultimo_elemento;
    size_t largo;
};

// Iterador externo
struct lista_iter {
    //...
};


/***************************************************************************************************/
//Primitivas de la lista


lista_t *lista_crear(void) {
}


bool lista_esta_vacia(const lista_t *lista) {
}


bool lista_insertar_primero(lista_t *lista, void *dato) {
}


bool lista_insertar_ultimo(lista_t *lista, void *dato) {
}


void *lista_borrar_primero(lista_t *lista) {
}


void *lista_ver_primero(const lista_t *lista) {
}


void *lista_ver_ultimo(const lista_t* lista) {
}


size_t lista_largo(const lista_t *lista) {
}


void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
}


/***************************************************************************************************/
//Primitivas del iterador interno


void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
}


/***************************************************************************************************/
//Primitivas del iterador externo


lista_iter_t *lista_iter_crear(lista_t *lista) {
}


bool lista_iter_avanzar(lista_iter_t *iter) {
}


void *lista_iter_ver_actual(const lista_iter_t *iter) {
}


bool lista_iter_al_final(const lista_iter_t *iter) {
}


void lista_iter_destruir(lista_iter_t *iter) {
}


bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
}


void *lista_iter_borrar(lista_iter_t *iter) {
}
