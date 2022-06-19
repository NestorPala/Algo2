#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h> //size_t



/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

//struct lista; //(agregar si no compila)

typedef struct lista lista_t;

// Iterador externo de la lista
typedef struct lista_iter lista_iter_t;



/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una lista vacía.
lista_t *lista_crear(void);

// Devuelve true si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un elemento al inicio de la lista, y devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se inserto un nuevo elemento al inicio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un elemento al final de la lista, devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se inserto un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Se elimina el primer elemento de la lista. Devuelve el elemento eliminado, en caso
// de no estar vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: la lista tiene todos los elementos a excepcion del primero.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el primer elemento de la lista, devuelve NULL en caso de estar vacia.
// Pre: la lista fue creada.
// Post: devuelve el primer elemento de la lista o NULL si esta vacía.
// La lista permanece intacta.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el ultimo elemento de la lista, devuelve NULL en caso de estar vacia.
// Pre: la lista fue creada.
// Post: devuelve el ultimo elemento de la lista o NULL si esta vacia.
// La lista permanece intacta.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos o su largo.
// Pre: la lista fue creada.
// Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si recibe el parametro destruir_dato, lo ejecuta para
// cada elemento de la lista.
// Pre: la lista fue creada.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));



/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Recorre la lista elemento a elemento y aplica la funcion visitar, se puede pasar un argumento extra (2do argumento).
// Pre: la lista fue creada.
// Post: se ejecuto la funcion visitar para cada elemento.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);



/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador externo.
// Pre: La lista a iterar fue creada.
// Post: Iterador externo listo para recorrer la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);


// Apunta al elemento de la lista siguiente al apuntado por el iterador externo, a menos que el iterador ya se encuentre apuntando al último elemento de la lista.
// Pre: El iterador fue creado.
// Post: El iterador se encuentra en la siguiente posición de la lista
bool lista_iter_avanzar(lista_iter_t *iter);


// Muestra cual es el elemento de la lista apuntado por el iterador externo, o devuelve NULL si la lista está vacía o el iterador se encuentra al final de la lista.
// Pre: El iterador existe.
// Post: La lista no se modifica.
void *lista_iter_ver_actual(const lista_iter_t *iter);


// Devuelve true si el iterador externo recorrió toda la lista, y false si todavía le quedan elementos por recorrer. Definimos que el iterador recorrió toda la lista cuando el elemento apuntado por éste es NULL.
// Pre: El iterador fue creado.
// Post: El iterador no se modifica.
bool lista_iter_al_final(const lista_iter_t *iter);


// Destruye el iterador externo. No se destruye la lista original ni ningún elemento de ella.
// Pre: El iterador fue creado.
// Post: El iterador ya no existe y su memoria se ha liberado.
void lista_iter_destruir(lista_iter_t *iter);


// Inserta un elemento en la posición de la lista apuntada por el iterador externo. Devuelve true si el elemento se ha podido insertar correctamente. Devuelve false en caso de error.
// Pre: El iterador fue creado.
// Post: La lista contiene un nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);


// Borra el elemento de la lista en la posición apuntada por el iterador externo. Devuelve el elemento borrado o NULL si la lista está vacía.
// Pre: El iterador fue creado.
// Post: El elemento que originalmente estaba apuntado por el iterador ya no existe y su memoria se ha liberado.
void *lista_iter_borrar(lista_iter_t *iter);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);


#endif  // LISTA_H
