#include "cola.h"
#include <stdlib.h>
#include <stddef.h>  //AGREGADO PARA PERMITIR COMPARACIÓN CON "NULL"



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



// Uso interno
bool cola_unico_elemento(cola_t* cola) {

    return
    cola -> primer_elemento  ==  cola -> ultimo_elemento
    &&
    cola -> primer_elemento != NULL;
}



bool cola_esta_vacia(const cola_t *cola) {

    if (cola == NULL) {
        return true;
    }

    return (cola -> primer_elemento == NULL) && (cola -> ultimo_elemento == NULL); //verificar luego si esto es correcto
}



bool cola_encolar(cola_t *cola, void *valor) {


    if (cola == NULL) {
        return false;
    }


    nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));


    if (nodo_nuevo == NULL) {
        return false;
    }


    // Inicializo el nodo nuevo (que voy a encolar)
    nodo_nuevo -> siguiente_nodo = NULL;
    nodo_nuevo -> dato = valor;


    if (cola_esta_vacia(cola)) {

        cola -> primer_elemento = nodo_nuevo;
        cola -> ultimo_elemento = nodo_nuevo;

        return true;
    }


    // Apunto el último nodo al nuevo nodo
    cola -> ultimo_elemento -> siguiente_nodo  =  nodo_nuevo;

    // Ahora el "nuevo" nodo es el último nodo
    cola -> ultimo_elemento = nodo_nuevo;


    return true;
}



void *cola_ver_primero(const cola_t *cola) {

    if (cola == NULL  ||  cola_esta_vacia(cola)) {
        return NULL;
    }

    return cola -> primer_elemento -> dato;
}



void *cola_desencolar(cola_t *cola) {


    if (cola == NULL  ||  cola_esta_vacia(cola)) {
        return NULL;
    }


    // Obtengo el valor del nodo que voy a quitar
    void* tope_anterior_cola  =  cola -> primer_elemento -> dato; //


    // Compruebo caso borde de 1 solo elemento
    if (cola_unico_elemento(cola)) {

        free(cola -> primer_elemento);
        cola -> primer_elemento = NULL;
        cola -> ultimo_elemento = NULL;

    } else {

        // Utilizo puntero a nodo auxiliar para no perder la referencia al siguiente elemento
        nodo_t* puntero_siguiente  =  cola -> primer_elemento -> siguiente_nodo;

        // Borro el elemento desencolado
        free(cola -> primer_elemento);

        // Recupero la referencia del elemento siguiente (ahora, primer elemento)
        cola -> primer_elemento  =  puntero_siguiente;
    }


    return tope_anterior_cola;
}



void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {


    if (cola == NULL) {
        return;
    }


    if (destruir_dato != NULL) //CAMBIAR LUEGO, NO ES NULL LO QUE USAMOS
    {
        //...
        return;
    }


    void* aux;

    do {
        aux = cola_desencolar(cola);
    } while (aux != NULL);
    

    free(cola); //CAMBIAR LUEGO
}
