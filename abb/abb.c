#include "abb.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h> //strcpy(), strcmp()
#include "pila.h"  //para el iterador externo


typedef struct abb_nodo abb_nodo_t;
typedef abb_comparar_clave_t cmp_t;
typedef abb_destruir_dato_t destr_t;


struct abb_nodo {
    abb_nodo_t* izq;
    abb_nodo_t* der;
    char* clave;
    void* dato;
};


struct abb {
    abb_nodo_t* raiz;
    size_t cantidad;
    cmp_t cmp;
    destr_t destruir_dato; 
};


struct abb_iter {
    abb_t* abb;
    abb_nodo_t* anterior;
    abb_nodo_t* actual;
    pila_t* recursion;
};


//-----------------------------------------------------------------------------------------//


char* strdup(const char* org) {
    char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
    if (!cpy) return NULL;
    strcpy(cpy, org);
    return cpy;
}


//-------------------------------------------- AUXILIARES ---------------------------------------------//


abb_nodo_t* abb_nodo_crear(abb_nodo_t* izq, abb_nodo_t* der, const char* clave, void* dato) {

    abb_nodo_t* nuevo_nodo = malloc(sizeof(abb_nodo_t));
    if (!nuevo_nodo) return NULL;

    nuevo_nodo->izq = izq;
    nuevo_nodo->der = der;
    nuevo_nodo->clave = strdup(clave);
    nuevo_nodo->dato = dato;

    return nuevo_nodo;
}


// La funcion empieza siempre por la raiz
// El nodo padre es el padre del ultimo nodo valido recorrido
abb_nodo_t* abb_nodo_buscar(abb_nodo_t* actual, const char* clave, cmp_t cmp, abb_nodo_t** padre) {

    if (!actual) return NULL;

    // Si lo encuentro en la raiz
    if (cmp(clave, actual->clave) == 0) {
        return actual;
    }

    if (actual->izq && actual->der) {
        *padre = actual;
    }

    if (cmp(clave, actual->clave) < 0)
        return abb_nodo_buscar(actual->izq, clave, cmp, padre);
    else
        return abb_nodo_buscar(actual->der, clave, cmp, padre);
}


void* abb_nodo_destruir(abb_nodo_t* nodo, destr_t destruir_dato) {

    if (!nodo) return NULL;

    free(nodo->clave);

    void* dato_borrado = nodo->dato;

    if (destruir_dato) {
        destruir_dato(nodo->dato);
        dato_borrado = NULL;
    }
    
    free(nodo);

    return dato_borrado;
}


void abb_destruir_2(abb_nodo_t* nodo, destr_t destruir_dato) {
    if (!nodo) return;
    abb_destruir_2(nodo->izq, destruir_dato);
    abb_destruir_2(nodo->der, destruir_dato);
    abb_nodo_destruir(nodo, destruir_dato);
}


// #####?
bool abb_nodo_swap(abb_nodo_t* viejo, abb_nodo_t* nuevo, abb_nodo_t* padre_nuevo, cmp_t cmp, bool reempl_izq, bool reempl_der) {
    if (!viejo || !nuevo) return false;

    free(viejo->clave);
    viejo->clave = strdup(nuevo->clave);

    viejo->dato = nuevo->dato;

    if (reempl_izq) {
        viejo->izq = nuevo->izq;
    }

    if (reempl_der) {
        viejo->der = nuevo->der;
    }

    // Esto solo funciona para reemplazar un nodo con una hoja
    if (padre_nuevo) {
        if (cmp(nuevo->clave, padre_nuevo->clave) < 0) {
            abb_nodo_destruir(padre_nuevo->izq, NULL);
            padre_nuevo->izq = NULL;
        } else {
            abb_nodo_destruir(padre_nuevo->der, NULL);
            padre_nuevo->der = NULL;
        }
    }
}


bool abb_nodo_swap_hoja(abb_nodo_t* padre, abb_nodo_t* hijo) {
    return abb_nodo_swap(padre, hijo, NULL, NULL, false, false);
}


// #####?
bool abb_guardar_hoja(abb_nodo_t* padre, abb_nodo_t* hijo, cmp_t cmp) {

    if (cmp(hijo->clave, padre->clave) == 0) {
        return abb_nodo_swap_hoja(padre, hijo);
    } else if (cmp(hijo->clave, padre->clave) < 0) {
        padre->izq = hijo;
    } else {
        padre->der = hijo;
    }

    return true;
}


//-------------------------------------------- PRIMITIVAS ---------------------------------------------//


abb_t* abb_crear(cmp_t cmp, destr_t destruir_dato) {

    // La función de comparación es vital; si el usuario no la pasa, no hacemos nada
    if (!cmp) return NULL;
    
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;

    return abb;
}


// EN PROCESO
bool abb_guardar(abb_t *abb, const char *clave, void *dato) {

    if (!abb || !clave) {
        return false;
    }

    abb_nodo_t* nuevo_nodo = abb_nodo_crear(NULL, NULL, clave, dato);
    if (!nuevo_nodo) return false;

    // Si no hay elementos
    if (abb->cantidad == 0) {
        abb->raiz = nuevo_nodo;
        abb->cantidad = 1;
        return true;
    } else if (abb->cantidad == 1) {  // Si esta solo la raiz
        if (!abb_guardar_hoja(abb->raiz, nuevo_nodo, abb->cmp)) {
            return false;
        }
        abb->cantidad = 2;
        return true;
    }

    // abb_nodo_t* padre_encontrado = NULL;
    // abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, &padre_encontrado);
    //...

    return true;
}


void *abb_obtener(const abb_t *abb, const char *clave) {
    if (!abb || !clave) return NULL;
    abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL);
    return encontrado ? encontrado->dato : NULL;
}


bool abb_pertenece(const abb_t *abb, const char *clave) {
    if (!abb || !clave) return false;
    return abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL) ? true : false;
}


size_t abb_cantidad(const abb_t *abb) {
    if(!abb || !abb->raiz) return 0;
    return abb->cantidad;
}


// REHACER
void *abb_borrar(abb_t *abb, const char *clave) {

    /*
    VER COMO BORRAR EN ABB
    https://docs.google.com/presentation/d/1Iyq_N7JBe19e4TZRlWe4qC3CdQ_SArpQ/edit#slide=id.p23
    */

    if (!abb || !clave || abb->cantidad == 0) return NULL;
    
    // abb_nodo_t* encontrado_padre = NULL;
    // abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, &encontrado_padre);

    // // Si el elemento no pertenece al ABB no se puede borrar, obviamente
    // if (!encontrado) return NULL;

    // abb->cantidad--; ///

    // // Hacemos el borrado en el lugar encontrado (si hay)
    // return abb_borrar_2(encontrado, encontrado_padre, abb, clave);
}


void abb_destruir(abb_t *abb) {
    
    if (!abb) return;

    if (abb->cantidad != 0) {
        if (abb->cantidad == 1) {
            abb_nodo_destruir(abb->raiz, abb->destruir_dato);
        } else {
            abb_destruir_2(abb->raiz, abb->destruir_dato);
        }
    } 

    free(abb);
}


/////////////////////////////////////////////////////////////////////////////////////////////

