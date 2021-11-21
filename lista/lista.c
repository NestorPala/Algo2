#include "lista.h"
#include <stdlib.h>
#include <stddef.h>


typedef struct nodo nodo_t;

struct nodo {
    nodo_t* siguiente_nodo;
    void *dato;
};

struct lista {
    nodo_t* primer_elemento;
    nodo_t* ultimo_elemento;
    size_t largo;
};

// Iterador externo
struct lista_iter {
  nodo_t *nodo_anterior;
  nodo_t *nodo_actual;
  lista_t *lista;
};


/***************************************************************************************************/
//Primitivas de la lista


lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (!lista){
        return NULL;
    }
    lista->primer_elemento = NULL;
    lista->ultimo_elemento = NULL;
    lista->largo = 0;
    return lista;
}


// Código reutilizado de la entrega "Cola" de Palavecino
nodo_t* crear_nodo(void *valor, nodo_t *siguiente_nodo) {

    nodo_t *nodo = malloc(sizeof(nodo_t));

    if (!nodo) {
        return NULL;
    }
    // Inicializo el nodo nuevo
    nodo -> dato = valor;
    nodo -> siguiente_nodo = siguiente_nodo;

    return nodo;
}


bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}


bool lista_insertar_primero(lista_t *lista, void *dato) {
    
  nodo_t* nuevo_nodo = crear_nodo(dato, lista->primer_elemento);

  if (!nuevo_nodo) {
      return false;
  }
  if (lista_esta_vacia(lista))
  {
    lista->ultimo_elemento = nuevo_nodo;
    lista->ultimo_elemento->siguiente_nodo = NULL;
  }
  lista->primer_elemento = nuevo_nodo;
  lista->largo++;
  return true;
}


bool lista_insertar_ultimo(lista_t *lista, void *dato) {

    nodo_t* nuevo_nodo = crear_nodo(dato, NULL);
    
    if (!nuevo_nodo){
        return false;
    }
    if (lista_esta_vacia(lista)){
        lista->primer_elemento = nuevo_nodo;
    }   
    else{
        lista->ultimo_elemento->siguiente_nodo = nuevo_nodo;
    }
    lista->ultimo_elemento = nuevo_nodo;
    lista->largo++;

    return true;
}


void *lista_borrar_primero(lista_t *lista) {

    if (lista_esta_vacia(lista)) {
        return NULL;
    }

    nodo_t *nodo_eliminado = lista->primer_elemento;
    void *dato_nodo_eliminado = lista_ver_primero(lista);

    lista->primer_elemento = lista->primer_elemento->siguiente_nodo;
    free(nodo_eliminado);
    lista->largo--;

    return dato_nodo_eliminado;
}


void *lista_ver_primero(const lista_t *lista) {
    return lista_esta_vacia(lista)? NULL : lista->primer_elemento->dato;
}


void *lista_ver_ultimo(const lista_t* lista) {
    return lista_esta_vacia(lista)? NULL : lista->ultimo_elemento->dato;
}


size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}


void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while(!lista_esta_vacia(lista)) {
        void* dato = lista_ver_primero(lista);
        if (destruir_dato) {
            destruir_dato(dato);
        }
        lista_borrar_primero(lista);
    }
    free(lista);
}


/***************************************************************************************************/
//Primitivas del iterador interno


void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t *nodo_actual = lista->primer_elemento;
    bool visitar_return = true;
    while (nodo_actual && visitar_return){
        if (visitar){
            visitar_return = visitar(nodo_actual->dato, extra);
        }
        nodo_actual = nodo_actual->siguiente_nodo;
    }
}


/***************************************************************************************************/
//Primitivas del iterador externo


lista_iter_t* lista_iter_crear(lista_t* lista) {

    lista_iter_t* iterador_externo = malloc(sizeof(lista_iter_t));

    if (!iterador_externo) {
        return NULL;
    }

    iterador_externo->lista = lista;
    iterador_externo->nodo_actual = lista->primer_elemento;
    iterador_externo->nodo_anterior = NULL;

    return iterador_externo;
}


bool lista_iter_al_final(const lista_iter_t* iter) {

    return iter->nodo_actual == NULL;
}


bool lista_iter_avanzar(lista_iter_t* iter) {

    if (lista_esta_vacia(iter->lista) || (lista_iter_al_final(iter))) {
        return false;
    }

    iter->nodo_anterior = iter->nodo_actual;
    iter->nodo_actual = iter->nodo_actual->siguiente_nodo;

    return true;
}


void* lista_iter_ver_actual(const lista_iter_t* iter) {

    if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return NULL;
    }

    return iter->nodo_actual->dato;
}


void lista_iter_destruir(lista_iter_t* iterador) {

    free(iterador);
}


// Función auxiliar. Pre: el iterador existe.
bool lista_iter_al_principio(lista_iter_t* iter) {

    return iter->nodo_anterior == NULL;
}


// Función auxiliar. Pre: el iterador existe.
void lista_iter_insertar_actual(lista_iter_t* iter, void* dato) {

    nodo_t* nuevo_nodo = crear_nodo(dato, iter->nodo_actual);

    iter->nodo_anterior->siguiente_nodo = nuevo_nodo;
    iter->nodo_actual = nuevo_nodo;

    iter->lista->largo++;
}


bool lista_iter_insertar(lista_iter_t* iter, void* dato) {

    if (lista_esta_vacia(iter->lista)) {
        if (!lista_insertar_primero(iter->lista, dato)) {
            return false;
        }
        iter->nodo_actual = iter->lista->primer_elemento;
        return true;
    }

    // Primer elemento
    if (lista_iter_al_principio(iter)) {
        if (!lista_insertar_primero(iter->lista, dato)) {
            return false;
        }
        iter->nodo_actual = iter->lista->primer_elemento;
        return true;
    }

    // Último elemento
    if (lista_iter_al_final(iter)) {
        if(!lista_insertar_ultimo(iter->lista, dato)) {
            return false;
        }
        
        iter->nodo_actual = iter->lista->ultimo_elemento;
        return true;
    }

    // Elemento en el medio de la lista
    lista_iter_insertar_actual(iter, dato);

    return true;
}


// Función auxiliar. Pre: el iterador existe.
void* lista_iter_borrar_medio(lista_iter_t* iter) {

    void* elemento_borrado = iter->nodo_actual->dato;

    iter->nodo_anterior->siguiente_nodo = iter->nodo_actual->siguiente_nodo;
    free(iter->nodo_actual);
    iter->nodo_actual = iter->nodo_anterior->siguiente_nodo;

    iter->lista->largo--;

    return elemento_borrado;
}


void* lista_iter_borrar_ultimo(lista_iter_t* iter) {

    void* dato_elim = iter->nodo_actual->dato;

    iter->nodo_anterior->siguiente_nodo = NULL;
    free(iter->nodo_actual);
    iter->nodo_actual = NULL;
    iter->lista->ultimo_elemento = iter->nodo_anterior;
    iter->lista->largo--;

    return dato_elim;
}


void* lista_iter_borrar(lista_iter_t* iter) {

    if (lista_esta_vacia(iter->lista)) {
        return NULL;
    }

    // Primer elemento
    if (lista_iter_al_principio(iter)) {
        void* aux_borrado = lista_borrar_primero(iter->lista);
        iter->nodo_actual = iter->lista->primer_elemento;
        return aux_borrado;
    }

    // Iterador al final
    if (lista_iter_al_final(iter)) {
        return NULL;
    }

    // Último elemento
    if (iter->nodo_actual->siguiente_nodo == NULL) {
        return lista_iter_borrar_ultimo(iter);
    }

    // En el medio de la lista
    return lista_iter_borrar_medio(iter);
}
