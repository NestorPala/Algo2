#include "pila.h"
#include <stdlib.h>
#include <stddef.h>  //AGREGADO PARA PERMITIR COMPARACIÓN CON "NULL"


const size_t PILA_TAMANIO_INICIAL = 20;


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

    pila_t* pila = malloc(sizeof(pila_t));

    pila -> datos = malloc(PILA_TAMANIO_INICIAL * sizeof(void*));
    pila -> cantidad = 0;
    pila -> capacidad = PILA_TAMANIO_INICIAL;

    return pila;
}


void pila_destruir(pila_t *pila) {

    if (pila == NULL) {
        return;
    }

    free(pila -> datos);
    free(pila);
}


bool pila_esta_vacia(const pila_t *pila) {

    if (pila == NULL || pila -> cantidad == 0) {
        return true;
    } else {
        return false;
    }
}


bool pila_redimensionar(pila_t* pila, size_t nueva_capacidad) {

    // Uso un puntero auxiliar por si falla el realloc()
    void** pila_redim_aux = realloc(pila -> datos, nueva_capacidad * sizeof(void*));


    // Chequeo que se halla hecho el realloc correctamente
    if (pila_redim_aux == NULL) {

        // Si el realloc() no se hizo correctamente, los datos originales nunca se ven afectados
        return false;
    }

    // Asigno los datos nuevos
    pila -> datos = pila_redim_aux;
    pila -> capacidad = nueva_capacidad;

    return true;
}


bool pila_apilar(pila_t *pila, void *valor) {
    
    if (pila == NULL) {
        return false;
    }


    // Redimensionamos la pila cuando sea necesario
    if (pila -> cantidad  ==  pila -> capacidad) {

        if (!pila_redimensionar(pila, pila -> capacidad * 2)) {
            return false;
        }
    }


    // Insertamos el elemento en la pila
    pila -> datos[pila -> cantidad] = valor;

    // Indicamos que la pila se agrandó
    pila -> cantidad  =  pila -> cantidad  +  1;


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

    // Redimensionamos la pila cuando sea necesario
    if (pila -> cantidad * 4  <=  pila -> capacidad) {

        pila_redimensionar(pila, pila -> capacidad / 2);
    }

    pila -> cantidad  =  pila -> cantidad - 1;

    return pila -> datos[pila -> cantidad];
}
