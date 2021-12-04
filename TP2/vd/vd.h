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
vd_t* vd_crear(size_t capacidad);


// Guarda el elemento "dato" en la posicion "pos". Devuelve true si el elemento se guardo correctamente y false si no.
// Si el usuario guarda NULL en una posicion que antes contenia un elemento no nulo, la cantidad de elementos se reduce en 1.
bool vd_guardar(vd_t* vector, size_t pos, void* dato);


// Devuelve el elemento guardado en la posicion "pos". 
// Se devuelve true en "resultado" si la posicion es valida.
// Se devuelve false en "resultado" si la posicion es invalida o el vector no existe.
void* vd_obtener(vd_t* vector, size_t pos, bool* resultado);


// Devuelve la cantidad de elementos del vector. Se define como elemento a aquel valor no nulo dentro del vector.
size_t vd_cantidad(vd_t* vector);


//
bool vd_esta_vacio(vd_t* vector);


size_t vd_largo(vd_t* vector);


bool vd_redimensionar(vd_t* vector, size_t capacidad);


void vd_destruir(vd_t* vector);


#endif  // _VD_H
