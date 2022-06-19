#ifndef _PILA_H
#define _PILA_H

#include <stdbool.h>


/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* Se trata de una pila que contiene datos de tipo void*
 * (punteros genéricos).  La pila en sí está definida en el .c.  */

struct pila_con_maximo;  // Definición completa en pila.c.
typedef struct pila_con_maximo pila_con_maximo_t;


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// Crea una pila.
pila_con_maximo_t *pila_crear(void);

// Destruye la pila.
void pila_destruir(pila_con_maximo_t *pila);

// Devuelve verdadero si la pila no tiene elementos apilados, false en caso contrario.
bool pila_esta_vacia(const pila_con_maximo_t *pila);

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
bool pila_apilar(pila_con_maximo_t *pila, int valor);

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope.
// Devuelve false si la pila no existe o se encuentra vacia y true en caso contrario.
bool pila_ver_tope(const pila_con_maximo_t *pila, int *resultado);

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor.
// Devuelve false si la pila no existe o se encuentra vacia y true en caso contrario.
bool pila_desapilar(pila_con_maximo_t *pila, int *resultado);

// Permite ver el numero entero mas grande guardado dentro de la pila.
// Devuelve false si la pila no existe o se encuentra vacia y true en caso contrario.
bool pila_ver_maximo(pila_con_maximo_t *pila, int *resultado);


#endif  // _PILA_H
