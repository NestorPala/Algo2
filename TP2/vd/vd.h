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


vd_t* vd_crear(size_t capacidad);


bool vd_guardar(vd_t* vector, size_t pos, void* dato);


void* vd_obtener(vd_t* vector, size_t pos);


size_t vd_largo(vd_t* vector);


bool vd_esta_vacio(vd_t* vector);


bool vd_redimensionar(vd_t* vector, size_t capacidad);


void vd_destruir(vd_t* vector);


#endif  // _VD_H
