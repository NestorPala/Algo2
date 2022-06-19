#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>


/***************************************************************************************************************************************/

/*//////////////////////////////////  DEFINICIÓN DE LOS TIPOS DE DATOS DEL ABB \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);  //ej: strcmp()
typedef void (*abb_destruir_dato_t) (void *);


/***************************************************************************************************************************************/

/*//////////////////////////////////  PRIMITIVAS DEL ÁRBOL BINARIO DE BÚSQUEDA (ABB)  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


// Crea un árbol binario de búsqueda (ABB).
// Pre: La función de comparacion de cadenas existe.
// Post: ABB creado con 0 elementos dentro.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


// Guarda un par clave valor dentro del ABB. La clave puede ser vacía pero no NULL. El valor puede ser NULL.
// Pre: El ABB existe.
// Post: Elemento guardado y la cantidad del ABB aumenta en 1.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


// Dada una clave, devuelve su valor si la clave existe dentro del ABB o NULL si no. 
// Pre: El ABB existe. La clave no es NULL.
// Post: El arbol no fue modificado.
void *abb_obtener(const abb_t *arbol, const char *clave);


// Dada una clave, devuelve true si esta se encuentra dentro del ABB o false si no.
// Pre: El ABB existe. La clave no es NULL.
// Post: Ninguno.
bool abb_pertenece(const abb_t *arbol, const char *clave);


// Devuelve la cantidad de elementos que contiene el ABB. 
// Pre: El ABB existe.
// Post: Ninguno.
size_t abb_cantidad(const abb_t *arbol);


// Dada una clave, borra ésta y su valor asociado del ABB.
// Pre: El ABB existe. La clave no es NULL. 
// Post: El ABB contiene un elemento menos.
void *abb_borrar(abb_t *arbol, const char *clave);


// Destruye todos los elementos del ABB y luego libera la memoria de éste.
// Pre: El ABB existe. 
// Post: El ABB ya no existe.
void abb_destruir(abb_t *arbol);


/***************************************************************************************************************************************/

/*//////////////////////////////////  PRIMITIVAS DEL ITERADOR INTERNO DEL ABB  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


// Recorre el ABB utilizando el recorrido in-order y aplica a cada elemento del mismo una función pasada por el usuario.
// La función "visitar" puede no ser especificada; en dicho caso esta función no tendrá efecto alguno.
// Pre: El árbol existe. 
// Post: Los elementos del ABB están modificados si el usuario lo especificó así, sino se devuelve una variable "extra" con información que
//       resulta de haber realizado el recorrido por el árbol.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/***************************************************************************************************************************************/

/*//////////////////////////////////  PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


// Crea un iterador externo para el ABB.
// Pre: El árbol existe.
// Post: El iterador apunta a la clave más pequeña del ABB.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);


// Avanza en uno la posición del iterador y devuelve true. Si éste ya se encuentra al final, devuelve false.
// Pre: El iterador existe.
// Post: El iterador apunta a la siguiente clave (en orden de comparación) del ABB.
bool abb_iter_in_avanzar(abb_iter_t *iter);


// Muestra la clave del elemento actual del ABB apuntado por el iterador. 
// Devuelve NULL si el ABB está vacío o el iterador se encuentra al final.
// Pre: El iterador existe.
// Post: Ninguno.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);


// Devuelve false si el iterador puede seguir avanzando en el ABB, y true si ya recorrió todo el árbol. 
// Pre: El iterador existe.
// Post: Ninguno.
bool abb_iter_in_al_final(const abb_iter_t *iter);


// Destruye el iterador externo del ABB.
// Pre: El iterador existe.
// Post: El iterador externo del ABB ya no existe. 
void abb_iter_in_destruir(abb_iter_t* iter);


/***************************************************************************************************************************************/

/*//////////////////////////////////  PRUEBAS DEL ABB  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

void pruebas_abb_estudiante(void);


/***************************************************************************************************************************************/

#endif  // ABB_H
