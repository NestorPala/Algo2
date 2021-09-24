#include "pila.h"
#include <stdlib.h>
#include <stddef.h>  //AGREGADO PARA PERMITIR COMPARACIÓN CON "NULL"


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {

    pila_t* pepe = malloc(sizeof(pila_t));

    size_t tamanio_inicial = 20;

    pepe -> datos = malloc(tamanio_inicial * sizeof(void*));
    pepe -> cantidad = 0;
    pepe -> capacidad = tamanio_inicial;

    return pepe;
}


void pila_destruir(pila_t *pila) {

    if (pila == NULL) {
        return;
    }

    free(pila -> datos);
    free(pila);
}


bool pila_esta_vacia(const pila_t *pila) {

    if (pila == NULL || pila -> capacidad == 0 || pila -> cantidad == 0) {
        return true;
    }
    else {
        return false;
    }
}


bool pila_apilar(pila_t *pila, void *valor) {
    
    if (pila == NULL) {
        return false;
    }

    //En caso de que el usuario quiera apilar en una PILA con tamaño cero.
    if (pila -> capacidad == 0) {
        pila -> capacidad = 20;
    }

    //Insertamos el elemento en la pila
    pila -> datos[pila -> cantidad] = valor;

    //Agrandamos la pila
    pila -> cantidad  =  pila -> cantidad  +  1;
    
    if (pila -> cantidad  ==  pila -> capacidad) {
        pila -> datos  =  realloc(pila -> datos, pila -> capacidad * 2 * sizeof(void*));
        pila -> capacidad  =  pila -> capacidad  *  2;
    }

    return true;
}


void *pila_ver_tope(const pila_t *pila) {

    if (pila_esta_vacia(pila)) {
        return NULL;
    }

    return pila -> datos[pila -> cantidad - 1];
}


void *pila_desapilar(pila_t *pila) {

    if (pila_esta_vacia(pila)) {
        return NULL;
    }

    pila -> cantidad  =  pila -> cantidad  -  1;

    if (pila -> cantidad * 4  <=  pila -> capacidad) {
        
        pila -> capacidad  =  pila -> capacidad  /  2;
        pila -> datos  =  realloc(pila -> datos,  pila -> capacidad  *  sizeof(void*));
    }

    return pila -> datos[pila -> cantidad];
}
