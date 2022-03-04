#include "pila_con_maximo.h"
#include <stdlib.h>
#include <stddef.h>  //AGREGADO PARA PERMITIR COMPARACIÓN CON "NULL"


const size_t PILA_TAMANIO_INICIAL = 20;


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila_con_maximo {
    int* datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
    int maximo;        // Guarda el elemento maximo del arreglo.
    bool hay_maximo;   // Para cuando la pila está vacía.
};


// AUXILIAR 
bool pila_redimensionar(pila_con_maximo_t* pila, size_t nueva_capacidad) {

    // Uso un puntero auxiliar por si falla el realloc()
    int* pila_redim_aux = realloc(pila -> datos, nueva_capacidad * sizeof(int));


    // Chequeo que se halla hecho el realloc correctamente
    if (!pila_redim_aux) {

        // Si el realloc() no se hizo correctamente, los datos originales nunca se ven afectados
        return false;
    }

    // Asigno los datos nuevos
    pila -> datos       = pila_redim_aux;
    pila -> capacidad   = nueva_capacidad;

    return true;
}


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_con_maximo_t *pila_crear(void) {

    pila_con_maximo_t* pila = malloc(sizeof(pila_con_maximo_t));

    pila -> datos = malloc(PILA_TAMANIO_INICIAL * sizeof(int));
    pila -> cantidad = 0;
    pila -> capacidad = PILA_TAMANIO_INICIAL;
    pila -> maximo = 0;
    pila -> hay_maximo = false;

    return pila;
}


void pila_destruir(pila_con_maximo_t *pila) {

    if (!pila) {
        return;
    }

    free(pila -> datos);
    free(pila);
}


bool pila_esta_vacia(const pila_con_maximo_t *pila) {
    return (!pila || pila -> cantidad == 0);
}


bool pila_apilar(pila_con_maximo_t *pila, int valor) {
    
    if (!pila) {
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

    // Obtenemos el nuevo máximo de la pila
    if (!pila->hay_maximo || (pila->hay_maximo && valor > pila->maximo)) {
        pila->maximo = valor;
        pila->hay_maximo = true;
    }

    return true;
}


bool pila_ver_tope(const pila_con_maximo_t *pila, int *resultado) {

    if (pila_esta_vacia(pila)) {
        return false;
    }

    *resultado = pila -> datos[pila -> cantidad - 1];

    return true;
}


bool pila_ver_maximo(pila_con_maximo_t *pila, int *resultado) {

    if (pila_esta_vacia(pila)) {
        return false;
    }

    *resultado = pila->maximo;

    return true;
}


bool pila_desapilar(pila_con_maximo_t *pila, int *resultado) {

    if (pila_esta_vacia(pila)) {
        return false;
    }

    // Redimensionamos la pila cuando sea necesario
    if (pila -> cantidad * 4  <=  pila -> capacidad) {
        if (!pila_redimensionar(pila, pila -> capacidad / 2)) {
            return false;
        }
    }

    pila -> cantidad  =  pila -> cantidad - 1;

    *resultado = pila -> datos[pila -> cantidad];

    return true;
}
