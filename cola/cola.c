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



// Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t *cola_crear(void) {

    cola_t* cola = malloc(sizeof(cola_t));

    cola -> primer_elemento  =  NULL;
    cola -> ultimo_elemento  =  NULL;

    return cola;
}



// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {

    if (cola == NULL) {
        return;
    }

    if (destruir_dato == NULL)
    {
        free(cola); //CAMBIAR LUEGO
    }
}



// Uso interno
bool cola_unico_elemento(const cola_t* cola) {

    return  cola -> primer_elemento  ==  cola -> ultimo_elemento;
}



// Devuelve verdadero si la cola no tiene elementos encolados, false en caso contrario.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola) {

    if (cola == NULL) {
        return true;
    }

    return (cola -> primer_elemento == NULL) && (cola -> ultimo_elemento == NULL); //verificar luego si esto es correcto
}



// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
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
    }


    // Apunto el último nodo al nuevo nodo
    cola -> ultimo_elemento -> siguiente_nodo  =  nodo_nuevo;

    // Ahora el "nuevo" nodo es el último nodo
    cola -> ultimo_elemento = nodo_nuevo;


    return true;
}



// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void *cola_ver_primero(const cola_t *cola) {

    if (cola == NULL  ||  cola_esta_vacia(cola)) {
        return NULL;
    }

    return cola -> primer_elemento -> dato;
}



// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void *cola_desencolar(cola_t *cola) {


    if (cola == NULL  ||  cola_esta_vacia(cola)) {
        return NULL;
    }


    // Obtengo el valor del nodo que voy a quitar
    void* tope_anterior_cola  =  cola -> primer_elemento -> dato;

    cola -> primer_elemento  =  cola -> primer_elemento -> siguiente_nodo;
    

    free(cola -> primer_elemento);


    if (cola_unico_elemento(cola)) {
        cola -> primer_elemento = NULL;
        cola -> ultimo_elemento = NULL;
    }
    

    return tope_anterior_cola;
}
