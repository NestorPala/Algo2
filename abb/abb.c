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


// AUXILIAR
char* strdup(const char* org) {
    char* cpy = malloc(sizeof(char) * (strlen(org) + 1));
    if (!cpy) return NULL;
    strcpy(cpy, org);
    return cpy;
}


// AUXILIAR
abb_nodo_t* abb_nodo_crear(abb_nodo_t* izq, abb_nodo_t* der, const char* clave, void* dato) {

    abb_nodo_t* nuevo_nodo = malloc(sizeof(abb_nodo_t));

    if (!nuevo_nodo) {
        return NULL;
    }

    nuevo_nodo->izq = izq;
    nuevo_nodo->der = der;
    nuevo_nodo->clave = strdup(clave);
    nuevo_nodo->dato = dato;

    return nuevo_nodo;
}


// AUXILIAR
// La funcion empieza siempre por la raiz
// El nodo padre es el padre del ultimo nodo valido recorrido
abb_nodo_t* abb_nodo_buscar(abb_nodo_t* actual, const char* clave, cmp_t cmp, abb_nodo_t** padre) {

    if (!actual) {
        return NULL;
    }

    // Si lo encuentro en la raiz o en el nodo actual
    if (cmp(clave, actual->clave) == 0) {
        return actual;
    }

    if (actual->izq || actual->der) {
        *padre = actual;
    }

    if (cmp(clave, actual->clave) < 0) {
        return abb_nodo_buscar(actual->izq, clave, cmp, padre);
    } else {
        return abb_nodo_buscar(actual->der, clave, cmp, padre);
    }
}


// AUXILIAR
void* abb_nodo_destruir(abb_nodo_t* nodo, destr_t destruir_dato) {

    if (!nodo) {
        return NULL;
    }

    free(nodo->clave);

    void* dato_borrado = nodo->dato;

    if (destruir_dato) {
        destruir_dato(nodo->dato);
        dato_borrado = NULL;
    }
    
    free(nodo);

    return dato_borrado;
}


// AUXILIAR
bool abb_nodo_swap(abb_nodo_t* viejo, abb_nodo_t* nuevo, destr_t destruir_dato, bool reempl_izq, bool reempl_der) {
    if (!viejo || !nuevo) {
        return false;
    }

    free(viejo->clave);
    viejo->clave = strdup(nuevo->clave);

    if (destruir_dato) {
        destruir_dato(viejo->dato);
    }

    viejo->dato = nuevo->dato;

    if (reempl_izq) {
        viejo->izq = nuevo->izq;
    }

    if (reempl_der) {
        viejo->der = nuevo->der;
    }

    abb_nodo_destruir(nuevo, NULL);

    return true;
}


// AUXILIAR
void abb_destruir_postorder(abb_nodo_t* nodo, destr_t destruir_dato) {
    if (!nodo) {
        return;
    }
    abb_destruir_postorder(nodo->izq, destruir_dato);
    abb_destruir_postorder(nodo->der, destruir_dato);
    abb_nodo_destruir(nodo, destruir_dato);
}


// AUXILIAR
// Me dan un nodo X y busco el nodo mas grande del subarbol izquierdo de X
// Avanzo 1 a la izquierda de X y luego todo hacia la derecha hasta que ya no tenga hijo derecho
abb_nodo_t* abb_buscar_maximo_izq(abb_nodo_t* nodo, abb_nodo_t** padre_nodo)  {

    nodo = nodo->izq;

    while (nodo->der) {
        *padre_nodo = nodo;
        nodo = nodo->der;
    }

    return nodo;
}


// AUXILIAR
void* abb_borrar_nodo_con_2_hijos(abb_nodo_t* nodo, destr_t destruir_dato) {

    void* dato_borrado = nodo->dato;

    // Como ya sabemos que el nodo tiene 2 hijos, preguntamos directamente si el hijo izquierdo tiene hijo derecho

    // Si el hijo izquierdo no tiene hijo derecho, entonces sera el reemplazante de su padre
        // El subarbol izquierdo del hijo izquierdo se conserva
        // El subarbol derecho del hijo izquierdo sera heredado de su padre

    if (!nodo->izq->der) {
        return abb_nodo_swap(nodo, nodo->izq, destruir_dato, true, false) ? dato_borrado : NULL;
    }

    // Si el hijo izquierdo tiene hijo derecho, entonces buscamos su hijo lo mas a la derecha posible

    abb_nodo_t* padre_maximo_izq = NULL;
    abb_nodo_t* maximo_izq = abb_buscar_maximo_izq(nodo, &padre_maximo_izq);

    // Ese hijo todo a la derecha (lo llamamos 'maximo_izq') sera el reemplazante del nodo que queriamos borrar
        // Si 'maximo_izq' no tiene hijo izquierdo, lo sacamos directo y lo colocamos donde queremos borrar el nodo, el padre de 'maximo_izq' ahora apunta a NULL
        // Si 'maximo_izq' tiene hijo izquierdo, hacemos el traspaso de ese hijo al padre de 'maximo_izq'

    if (maximo_izq->izq) {
        padre_maximo_izq->der = maximo_izq->izq;
    } else {
        padre_maximo_izq->der = NULL;
    }

    return abb_nodo_swap(nodo, maximo_izq, destruir_dato, false, false) ? dato_borrado : NULL;
} 


// AUXILIAR
void* abb_borrar_nodo_interno_con_solo_1_hijo(abb_nodo_t* nodo, abb_nodo_t* padre_nodo, cmp_t cmp, destr_t destruir_dato) {
    
    void* dato_borrado = NULL;

    // Hay 4 casos:

    if ( cmp(nodo->clave, padre_nodo->clave) < 0  &&  nodo->izq ) {

        // El nodo es hijo izquierdo y tiene hijo izquierdo
        padre_nodo->izq = nodo->izq;
        dato_borrado = abb_nodo_destruir(nodo, destruir_dato);

    } else if ( cmp(nodo->clave, padre_nodo->clave) < 0  &&  nodo->der ) {

        // El nodo es hijo izquierdo y tiene hijo derecho
        padre_nodo->izq = nodo->der;
        dato_borrado = abb_nodo_destruir(nodo, destruir_dato);

    } else if ( cmp(nodo->clave, padre_nodo->clave) > 0  &&  nodo->izq ) {

        // El nodo es hijo derecho y tiene hijo izquierdo
        padre_nodo->der = nodo->izq;
        dato_borrado = abb_nodo_destruir(nodo, destruir_dato);

    } else if ( cmp(nodo->clave, padre_nodo->clave) > 0  &&  nodo->der ) {

        // El nodo es hijo derecho y tiene hijo derecho 
        padre_nodo->der = nodo->der;
        dato_borrado = abb_nodo_destruir(nodo, destruir_dato);
        
    }

    return dato_borrado;
}


// AUXILIAR
void* abb_borrar_raiz_con_solo_1_hijo(abb_t* abb, bool izq) {
    abb_nodo_t* aux = izq ? abb->raiz->izq : abb->raiz->der;

    void* dato_borrado = abb_nodo_destruir(abb->raiz, abb->destruir_dato);
    abb->raiz = aux;

    return dato_borrado;
}


// AUXILIAR
void* abb_borrar_hoja(abb_nodo_t* nodo, abb_nodo_t* padre_nodo, cmp_t cmp, destr_t destruir_dato) {

    if (cmp(nodo->clave, padre_nodo->clave) < 0) {
        padre_nodo->izq = NULL;
    } else {
        padre_nodo->der = NULL;
    }

    return abb_nodo_destruir(nodo, destruir_dato);
}


// AUXILIAR 
void* abb_borrar_3_o_mas_nodos(abb_t* abb, const char* clave) {
    
    abb_nodo_t* padre_encontrado = abb->raiz;
    abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, &padre_encontrado);


    if (encontrado == abb->raiz) {
        
        if (!abb->raiz->der) {

            // Si no tiene subarbol del lado derecho, el hijo izquierdo pasa a ser la nueva raiz
            return abb_borrar_raiz_con_solo_1_hijo(abb, true); 

        } else if (!abb->raiz->izq) {

            // Si no tiene subarbol del lado izquierdo, el hijo derecho pasa a ser la nueva raiz
            return abb_borrar_raiz_con_solo_1_hijo(abb, false); 

        }

        // Si tiene los dos, reemplazamos la raiz con el hijo mas grande del subarbol izquierdo
        return abb_borrar_nodo_con_2_hijos(abb->raiz, abb->destruir_dato);   

    } else if (!encontrado->izq && !encontrado->der) { 

        // pregunto si el nodo encontrado es una hoja (tiene 0 hijos)
        return abb_borrar_hoja(encontrado, padre_encontrado, abb->cmp, abb->destruir_dato);   
              
    } else if ((encontrado->izq && !encontrado->der) || (!encontrado->izq && encontrado->der)) { 

        // pregunto si el nodo es interno y tiene 1 hijo
        return abb_borrar_nodo_interno_con_solo_1_hijo(encontrado, padre_encontrado, abb->cmp, abb->destruir_dato);                                                      

    }

    // cuando el nodo es interno y tiene 2 hijos
    return abb_borrar_nodo_con_2_hijos(encontrado, abb->destruir_dato);  
}


// AUXILIAR
void* abb_borrar_raiz_con_2_hojas(abb_t* abb, const char* clave) {

    void* dato_borrado = NULL;

    if (abb->cmp(clave, abb->raiz->clave) == 0) {

        abb_nodo_t* aux1 = abb->raiz->izq;
        abb_nodo_t* aux2 = abb->raiz->der;
        dato_borrado = abb_nodo_destruir(abb->raiz, abb->destruir_dato);
        aux1->der = aux2;
        abb->raiz = aux1;

    } else if (abb->cmp(clave, abb->raiz->izq->clave) == 0) {

        dato_borrado = abb_nodo_destruir(abb->raiz->izq, abb->destruir_dato);
        abb->raiz->izq = NULL;

    } else {

        dato_borrado = abb_nodo_destruir(abb->raiz->der, abb->destruir_dato);
        abb->raiz->der = NULL;

    }

    return dato_borrado;
}


// AUXILIAR
void* abb_borrar_cantidad_de_2(abb_t* abb, const char* clave, bool izq) {

    abb_nodo_t* aux = izq ? abb->raiz->izq : abb->raiz->der;
    void* dato_borrado = NULL;

    if (abb->cmp(clave, abb->raiz->clave) == 0) {
        dato_borrado = abb_nodo_destruir(abb->raiz, abb->destruir_dato);
        abb->raiz = aux;
    } else {
        dato_borrado = abb_nodo_destruir(aux, abb->destruir_dato);

        if (izq) {
            abb->raiz->izq = NULL;
        } else {
            abb->raiz->der = NULL;
        }
    }

    return dato_borrado;
} 


// AUXILIAR
bool abb_guardar_hoja(abb_nodo_t* padre, abb_nodo_t* hijo, cmp_t cmp) {

    if (cmp(hijo->clave, padre->clave) == 0) {
        return abb_nodo_swap(padre, hijo, NULL, false, false);
    } else if (cmp(hijo->clave, padre->clave) < 0) {
        padre->izq = hijo;
    } else {
        padre->der = hijo;
    }

    return true;
}


// AUXILIAR
bool abb_guardar_cantidad_mayor_a_1(abb_nodo_t* actual, abb_t *abb, const char *clave, void *dato) {
    
    if (!actual) {
        return false;
    }
    
    if (!actual->izq && !actual->der) {

        abb_nodo_t* nuevo = abb_nodo_crear(NULL, NULL, clave, dato);

        if (abb->cmp(clave, actual->clave) == 0) {
            if (!abb_nodo_swap(actual, nuevo, NULL, true, true)) {
                return false;
            }
            abb->cantidad++;
            return true;
        } else if (abb->cmp(clave, actual->clave) < 0) {
            actual->izq = nuevo;
        } else {
            actual->der = nuevo;
        }

        abb->cantidad++;
        return true;
    }

    if (abb->cmp(clave, actual->clave) == 0) {
        abb_nodo_t* nuevo = abb_nodo_crear(actual->izq, actual->der, clave, dato);
        if (!abb_nodo_swap(actual, nuevo, NULL, true, true)) {
            return false;
        }
        abb->cantidad++;
        return true;
    } else if (abb->cmp(clave, actual->clave) < 0) {
        if (!actual->izq) {
            abb_nodo_t* nuevo = abb_nodo_crear(NULL, NULL, clave, dato);
            if (!nuevo) {
                return false;
            }
            actual->izq = nuevo;
            abb->cantidad++;
            return true;
        }
        return abb_guardar_cantidad_mayor_a_1(actual->izq, abb, clave, dato);
    } else {
        if (!actual->der) {
            abb_nodo_t* nuevo = abb_nodo_crear(NULL, NULL, clave, dato);
            if (!nuevo) {
                return false;
            }
            actual->der = nuevo;
            abb->cantidad++;
            return true;
        }
        return abb_guardar_cantidad_mayor_a_1(actual->der, abb, clave, dato);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {

    // La función de comparación es vital; si el usuario no la pasa, no hacemos nada
    if (!cmp) {
        return NULL;
    }
    
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol) {
        return NULL;
    }

    arbol->raiz = NULL;
    arbol->cantidad = 0;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;

    return arbol;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {

    if (!arbol || !clave) {
        return false;
    }

    abb_nodo_t* nuevo_nodo = abb_nodo_crear(NULL, NULL, clave, dato);
    if (!nuevo_nodo) {
        return false;
    }

    // Si no hay elementos
    if (arbol->cantidad == 0) {
        arbol->raiz = nuevo_nodo;
        arbol->cantidad = 1;
        return true;
    } else if (arbol->cantidad == 1) {  // Si esta solo la raiz
        if (!abb_guardar_hoja(arbol->raiz, nuevo_nodo, arbol->cmp)) {
            return false;
        }
        arbol->cantidad = 2;
        return true;
    }

    abb_nodo_destruir(nuevo_nodo, NULL);

    return abb_guardar_cantidad_mayor_a_1(arbol->raiz, arbol, clave, dato);
}


void *abb_obtener(const abb_t *arbol, const char *clave) {
    if (!arbol || !clave || arbol->cantidad == 0) {
        return NULL;
    }

    abb_nodo_t* aux = arbol->raiz;
    abb_nodo_t* encontrado = abb_nodo_buscar(arbol->raiz, clave, arbol->cmp, &aux);

    return encontrado ? encontrado->dato : NULL;
}


bool abb_pertenece(const abb_t *arbol, const char *clave) {
    if (!arbol || !clave || arbol->cantidad == 0) {
        return false;
    }

    abb_nodo_t* aux = arbol->raiz;

    return abb_nodo_buscar(arbol->raiz, clave, arbol->cmp, &aux) ? true : false;
}


size_t abb_cantidad(const abb_t *arbol) {
    if(!arbol || !arbol->raiz) {
        return 0;
    }

    return arbol->cantidad;
}


void *abb_borrar(abb_t *arbol, const char *clave) {

    if (!arbol || !clave || arbol->cantidad == 0) {
        return NULL;
    }

    if (!abb_pertenece(arbol, clave)) {
        return NULL;
    }
    
    // A partir de acá sabemos que la clave a borrar existe

    void* dato_borrado = NULL;

    if (arbol->cantidad == 1) {
        dato_borrado = abb_nodo_destruir(arbol->raiz, arbol->destruir_dato);
        arbol->raiz = NULL;
        arbol->cantidad = 0;
    } else if (arbol->cantidad == 2) {
        if (arbol->raiz->izq) {
            dato_borrado = abb_borrar_cantidad_de_2(arbol, clave, true);
        } else {
            dato_borrado = abb_borrar_cantidad_de_2(arbol, clave, false);
        }
        arbol->cantidad = 1;
    } else if (arbol->cantidad == 3 && arbol->raiz->izq && arbol->raiz->der) {
        dato_borrado = abb_borrar_raiz_con_2_hojas(arbol, clave);
        arbol->cantidad = 2;
    } else {
        dato_borrado = abb_borrar_3_o_mas_nodos(arbol, clave);
        arbol->cantidad--;
    }
    
    return dato_borrado;
}


void abb_destruir(abb_t *arbol) {
    
    if (!arbol) {
        return;
    }

    if (arbol->cantidad != 0) {
        if (arbol->cantidad == 1) {
            abb_nodo_destruir(arbol->raiz, arbol->destruir_dato);
        } else {
            abb_destruir_postorder(arbol->raiz, arbol->destruir_dato);
        }
    } 

    free(arbol);
}


/////////////////////////////////////////////////////////////////////////////////////////////


// AUXILIAR
void abb_in_order_(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
    
    if (!nodo) {
        return;
    }

    abb_in_order_(nodo->izq, visitar, extra);
    
    if (!visitar(nodo->clave, nodo->dato, extra)) {
        return;
    }

    abb_in_order_(nodo->der, visitar, extra);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    return abb_in_order_(arbol->raiz, visitar, extra);
}


/////////////////////////////////////////////////////////////////////////////////////////////

