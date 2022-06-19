#ifndef _VD_H
#define _VD_H

#include <stdbool.h>
#include <stdlib.h>


/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct vector vd_t;


/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/


// Crea un vector dinamico de largo "capacidad". Si la capacidad es igual a 0 no se crea ningun vector y devuelve NULL.
// Post: el vector contiene "capacidad" lugares vacios listos para usar.
vd_t* vd_crear(size_t capacidad);


// Guarda el elemento "dato" en la posicion "pos". Devuelve true si el elemento se guardo correctamente y false si no.
// Si el usuario guarda NULL en una posicion que antes contenia un elemento no nulo, la cantidad de elementos se reduce en 1.
// Post: el vector contiene un elemento no nulo mas. La cantidad se ha incrementado en 1.
bool vd_guardar(vd_t* vector, size_t pos, void* dato);


// Devuelve el elemento guardado en la posicion "pos". 
// Se devuelve true en "resultado" si la posicion es valida.
// Se devuelve false en "resultado" si la posicion es invalida o el vector no existe.
// Post: el vector original no se ha modificado.
void* vd_obtener(vd_t* vector, size_t pos, bool* resultado);


// Devuelve la cantidad de elementos del vector. Se define como elemento a aquel valor no nulo dentro del vector.
// Post: el vector original no se ha modificado.
size_t vd_cantidad(vd_t* vector);


// Devuelve true si el vector esta vacio, y false si el vector contiene al menos un elemento.
// Post: el vector original no se ha modificado.
bool vd_esta_vacio(vd_t* vector);


// Devuelve el cantidad maxima de elementos que puede guardar actualmente el vector.
// Post: el vector original no se ha modificado.
size_t vd_largo(vd_t* vector);


// Permite cambiar el tamaño del vector, haciendolo mas grande o mas chico.
// Devuelve true si el vector se redimensiona correctamente, y false si ocurre algun error o la capacidad solicitada por el usuario es 0.
// Post: el vector contiene una cantidad maxima de elementos nueva. La cantidad de elementos puede haberse modificado si la nueva capacidad es demasiado chica.
bool vd_redimensionar(vd_t* vector, size_t capacidad);


// Destruye el vector. Destruye los elementos originales apuntados por los elementos guardados en este vector si se ha especificado una funcion para ello.
// Post: el vector original ya no existe.
void vd_destruir(vd_t* vector, void (*destruir_elemento)(void *e));


#endif  // _VD_H
