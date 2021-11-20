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


// REVISAR
void aux_inorder(abb_nodo_t* nodo_actual, pila_t* inorder, const char* clave) {
    if (!nodo_actual) return;
    aux_inorder(nodo_actual->izq, inorder, clave);
    pila_apilar(inorder, nodo_actual);
    aux_inorder(nodo_actual->der, inorder, clave);
}


// #####
abb_nodo_t* abb_nodo_crear(abb_nodo_t* izq, abb_nodo_t* der, const char* clave, void* dato) {

    abb_nodo_t* nuevo_nodo = malloc(sizeof(abb_nodo_t));
    if (!nuevo_nodo) return NULL;

    nuevo_nodo->izq = izq;
    nuevo_nodo->der = der;
    nuevo_nodo->clave = strdup(clave);
    nuevo_nodo->dato = dato;

    return nuevo_nodo;
}


// #####
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


// #####
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


// #####
void abb_destruir_2(abb_nodo_t* nodo, destr_t destruir_dato) {
    if (!nodo) return;
    abb_destruir_2(nodo->izq, destruir_dato);
    abb_destruir_2(nodo->der, destruir_dato);
    abb_nodo_destruir(nodo, destruir_dato);
}


// REVISAR
void* abb_borrar_hoja (abb_nodo_t* borrado, abb_nodo_t* padre_borrado, abb_t* abb, const char* clave) {

    void* dato_borrado = NULL;
    cmp_t cmp = abb->cmp;
    destr_t destruir_dato = abb->destruir_dato;

    // Si es la raíz
    if (cmp(clave, abb->raiz->clave) == 0) {
        dato_borrado = abb_nodo_destruir(borrado, NULL);
    } else {

        // Si no es la raíz
        // Lo comparamos con el padre para saber si estaba a la izquierda o a la derecha
        // Recordar que el hijo nunca va a ser igual al padre
        if (cmp(borrado->clave, padre_borrado->clave) < 0) {

            dato_borrado = abb_nodo_destruir(padre_borrado->izq, NULL);
            padre_borrado->izq = NULL;
        }
        else {
            dato_borrado = abb_nodo_destruir(padre_borrado->der, NULL);
            padre_borrado->der = NULL;
        }  
    }

    return dato_borrado;
}


// REVISAR
void* abb_borrar_1_hijo (abb_nodo_t* borrado, abb_nodo_t* padre_borrado, abb_t* abb, const char* clave) {

    void* dato_borrado = NULL;
    cmp_t cmp = abb->cmp;
    destr_t destruir_dato = abb->destruir_dato;


    // Si es la raíz
    // Si la raíz tiene un solo hijo, entonces el hijo pasa a ser la raíz
    if (cmp(clave, abb->raiz->clave) == 0 ) {

        abb_nodo_t* nodo_aux;

        if (borrado->izq) {
            nodo_aux = abb->raiz->izq;
        } else {
            nodo_aux = abb->raiz->der;
        }

        dato_borrado = abb_nodo_destruir(abb->raiz, NULL);
        abb->raiz = nodo_aux;
    } else {
        // Si no es la raíz
        /*
        Tenemos 4 casos a la hora de borrar:
            1) Borrar un nodo izquierdo con hijo izquierdo
            2) Borrar un nodo izquierdo con hijo derecho
            3) Borrar un nodo derecho con hijo izquierdo
            4) Borrar un nobo derecho con hijo derecho
        */

        if ( cmp(borrado->clave, padre_borrado->clave) < 0 ) {

            if (borrado->izq) {
                padre_borrado->izq = borrado->izq;  // CASO 1
            } else {
                padre_borrado->izq = borrado->der;  // CASO 2
            }
            
        } else {

            if (borrado->izq) {
                padre_borrado->der = borrado->izq;  // CASO 3
            } else {
                padre_borrado->der = borrado->der;  // CASO 4
            }
        }

        dato_borrado = abb_nodo_destruir(borrado, NULL); 
    }

    return dato_borrado;
}


// REVISAR
void* abb_borrar_2(abb_nodo_t* borrado, abb_nodo_t* padre_borrado, abb_t* abb, const char* clave) {


    void* dato_borrado = NULL;
    cmp_t cmp = abb->cmp;
    destr_t destruir_dato = abb->destruir_dato;
   

    // El caso de que el nodo exista se comprueba antes de entrar a esta función
    // En cada uno de los 3 casos tengo que chequear si estoy en la raíz


    // Nodo con ningún hijo
    if (!borrado->izq && !borrado->der) {

        return abb_borrar_hoja(borrado, padre_borrado, abb, clave);
    }


    // Nodo con 1 hijo
    // Puede tener hijo izquierdo y no derecho, o tener hijo derecho y no izquierdo
    if (  (borrado->izq && !borrado->der)  ||  (!borrado->izq && borrado->der) ) {

        return abb_borrar_1_hijo(borrado, padre_borrado, abb, clave);
    }


    // Nodo con 2 hijos (caso común)
        /*
        Si el nodo tiene dos hijos no se elimina el nodo, sino que se reemplaza con el siguiente inorder 
        (es decir, con el menor de sus hijos mayores) o con el anterior inorder (el mayor de sus hijos menores).
        
        Luego se llama a la eliminación recursiva en el subárbol correspondiente de acuerdo a la estrategia 
        de eliminación elegida. Como se eligió o bien el menor de sus hijos mayores o el mayor de sus hijos menores, 
        obligatoriamente al nodo a borrar le va a faltar un hijo, haciendo que se caiga en alguno de los dos primeros casos.
        */
    

    if (cmp(clave, abb->raiz->clave) == 0 ) {  // Si el nodo es la raíz

        //Tenemos que reemplazar el nodo raíz con el nodo más grande en el subárbol izquierdo
        
        //Hacemos "uno a la izquierda y todo a la derecha"
        abb_nodo_t* maximo_subabb_izq = abb->raiz->izq;
        abb_nodo_t* padre_maximo_subabb_izq;

        while(maximo_subabb_izq->der) {

            // Guardamos el padre del más grande a la izquierda
            if (maximo_subabb_izq->der->der == NULL) {
                padre_maximo_subabb_izq = maximo_subabb_izq;
            }

            maximo_subabb_izq = maximo_subabb_izq->der;
        }

        //Borramos el nodo reemplazante habiendo primero guardado los datos
        char* aux_clave = maximo_subabb_izq->clave;
        void* aux_dato = abb_borrar_2(maximo_subabb_izq, padre_maximo_subabb_izq, abb, clave);

        //Guardamos el "dato viejo" para devolverlo después de borrarlo
        dato_borrado = abb->raiz->dato;

        //Pisamos el nodo a borrar
        //abb->raiz->clave = aux_clave;
        strcpy(abb->raiz->clave, aux_clave);
        if (destruir_dato) destruir_dato(abb->raiz->dato);
        abb->raiz->dato = aux_dato;


    } else {  // Si el nodo NO ES la raíz
        
        if (borrado->izq && borrado->der) {

            //Nosotros elegiremos la estrategia de usar el anterior inorder del elemento que queremos borrar como el reemplazante

            pila_t* pila_inorder = pila_crear();
            
            aux_inorder(abb->raiz, pila_inorder, clave);

            // Vamos a encontrar el elemento actual (el que queremos borrar) y luego el siguiente desapilado es el anterior inorder
            while ( cmp( ((abb_nodo_t*)pila_desapilar(pila_inorder))->clave, clave) != 0);

            // Desapilamos una vez más para encontrar el anterior inorder
            abb_nodo_t* anterior_inorder = pila_desapilar(pila_inorder);
            pila_destruir(pila_inorder);

            // Guardamos la clave del reemplazante
            char* nodo_reempl_clave = anterior_inorder->clave;

            // Buscamos el padre del nodo que vamos a borrar en el paso siguiente
            abb_nodo_t* padre_anterior_inorder;
            abb_nodo_buscar(abb->raiz, clave, cmp, &padre_anterior_inorder);

            // Borramos al reemplazante, y guardamos su valor (sí o sí debería tener 0 o 1 hijos, sino se rompe todo)
            void* nodo_reempl_dato = abb_borrar_2(anterior_inorder, padre_anterior_inorder, abb, clave);

            // Guardamos el dato cuya clave estamos por borrar
            dato_borrado = borrado->dato;

            // Pisamos la clave y el valor del que nodo que teníamos que borrar
            //borrado->clave = nodo_reempl_clave;
            strcpy(borrado->clave, nodo_reempl_clave);
            if (destruir_dato) destruir_dato(borrado->dato);
            borrado->dato = nodo_reempl_dato;
        }
    }

    return dato_borrado;
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


// #####
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


// #####
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

    abb_nodo_t* padre_encontrado = NULL;
    abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, &padre_encontrado);
    
    //...

    return true;
}


// #####
void *abb_obtener(const abb_t *abb, const char *clave) {
    if (!abb || !clave) return NULL;
    abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL);
    return encontrado ? encontrado->dato : NULL;
}


// #####
bool abb_pertenece(const abb_t *abb, const char *clave) {
    if (!abb || !clave) return false;
    return abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL) ? true : false;
}


// #####
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


// #####
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

