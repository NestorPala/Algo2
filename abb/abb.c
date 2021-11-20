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

    abb_nodo_t* nodo_nuevo = malloc(sizeof(abb_nodo_t));
    if (!nodo_nuevo) return NULL;

    nodo_nuevo->izq = izq;
    nodo_nuevo->der = der;
    nodo_nuevo->clave = strdup(clave);
    nodo_nuevo->dato = dato;

    return nodo_nuevo;
}


// AUXILIAR
// La funcion empieza siempre por la raiz
abb_nodo_t* abb_nodo_buscar(abb_nodo_t* nodo_actual, const char* clave, cmp_t cmp, abb_nodo_t** padre) {

    // Caso base (si la raiz es NULL o llegué a una hoja)
    if (!nodo_actual) return NULL;

    // Si lo encuentro en la raiz o en el nodo padre
        // cmp_t(c1, c2) == 0  =>  c1 = c2
    if (cmp(clave, nodo_actual->clave) == 0) {
        return nodo_actual;
    }

    // Vamos guardando el nodo padre para luego poder insertar si no se encuentra la clave
    if (padre) {
        *padre = nodo_actual;
    }

    // Busco a izquierda y derecha del nodo actual
        // cmp_t(c1, c2) < 0  =>  c1 < c2
        // cmp_t(c1, c2) > 0  =>  c1 > c2
    if (cmp(clave, nodo_actual->clave) < 0)
        return abb_nodo_buscar(nodo_actual->izq, clave, cmp, padre);
    else
        return abb_nodo_buscar(nodo_actual->der, clave, cmp, padre);
}


// AUXILIAR 
void* abb_nodo_borrar(abb_nodo_t* nodo, destr_t destruir_dato) {

    void* dato_borrado = nodo->dato;

    if (destruir_dato) {
        destruir_dato(nodo->dato);
        dato_borrado = NULL;
    }
    
    //free(nodo->clave);
    free(nodo);

    return dato_borrado;
}


// AUXILIAR
void aux_inorder(abb_nodo_t* nodo_actual, pila_t* inorder, const char* clave) {
    if (!nodo_actual) return;
    aux_inorder(nodo_actual->izq, inorder, clave);
    pila_apilar(inorder, nodo_actual);
    aux_inorder(nodo_actual->der, inorder, clave);
}


// AUXILIAR
void* abb_borrar_hoja (abb_nodo_t* borrado, abb_nodo_t* padre_borrado, abb_t* abb, const char* clave) {

    void* dato_borrado = NULL;
    cmp_t cmp = abb->cmp;
    destr_t destruir_dato = abb->destruir_dato;

    // Si es la raíz
    if (cmp(clave, abb->raiz->clave) == 0) {
        dato_borrado = abb_nodo_borrar(borrado, destruir_dato);
    } else {

        // Si no es la raíz
        // Lo comparamos con el padre para saber si estaba a la izquierda o a la derecha
        // Recordar que el hijo nunca va a ser igual al padre
        if (cmp(borrado->clave, padre_borrado->clave) < 0) {

            dato_borrado = abb_nodo_borrar(padre_borrado->izq, destruir_dato);
            padre_borrado->izq = NULL;
        }
        else {
            dato_borrado = abb_nodo_borrar(padre_borrado->der, destruir_dato);
            padre_borrado->der = NULL;
        }  
    }

    return dato_borrado;
}


// AUXILIAR
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

        dato_borrado = abb_nodo_borrar(abb->raiz, destruir_dato);
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

        dato_borrado = abb_nodo_borrar(borrado, destruir_dato); 
    }

    return dato_borrado;
}


// AUXILIAR
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


// AUXILIAR
void abb_destruir_2(abb_nodo_t* nodo, destr_t destruir_dato) {

    // Caso base
    if (!nodo) return;

    abb_destruir_2(nodo->izq, destruir_dato);
    abb_destruir_2(nodo->der, destruir_dato);

    abb_nodo_borrar(nodo, destruir_dato);
}


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


bool abb_guardar(abb_t *abb, const char *clave, void *dato) {

    if (!abb || !clave) return false;

    abb_nodo_t* nodo_nuevo = abb_nodo_crear(NULL, NULL, clave, dato);
    if (!nodo_nuevo) return false;

    // Si no hay elementos
    if (abb->raiz == NULL) {
        abb->raiz = nodo_nuevo;
        abb->cantidad = 1;
        return true;
    }

    cmp_t cmp = abb->cmp;

    //paso su dirección de memoria (pasaje por referencia) para mantener el puntero aislado del stack de recursión
    abb_nodo_t* padre_encontrado;

    abb_nodo_t* nodo_encontrado = abb_nodo_buscar(abb->raiz, clave, cmp, &padre_encontrado);
    
    //si el elemento ya está, lo piso (no aumenta la cantidad)
    if (nodo_encontrado) {

        // Si la funcion destruir dato es NULL asumimos que no hay que liberar memoria
        if (!abb->destruir_dato) {
            nodo_encontrado->dato = dato;
            return true;
        }

        // Si no, usamos la función de "Bárbara"
        abb->destruir_dato(nodo_encontrado->dato);
        nodo_encontrado->dato = dato;
        return true;
    }
    
    //si el elemento no está, lo ubico a izq. o der. de donde estará su padre
    //la clave nunca va a ser igual a su padre (porque sino pasa lo del if de arriba)
    if (cmp(clave, padre_encontrado->clave) < 0) {

        padre_encontrado->izq = nodo_nuevo;
    } else {
        padre_encontrado->der = nodo_nuevo;
    }

    abb->cantidad++;

    return true;
}


void *abb_obtener(const abb_t *abb, const char *clave) {
    if (!abb) return NULL;
    if (!clave) return NULL;
    abb_nodo_t* nodo_encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL);
    return nodo_encontrado ? nodo_encontrado->dato : NULL;
}


bool abb_pertenece(const abb_t *abb, const char *clave) {
    if (!abb) return false;
    if (!clave) return false;  
    return (abb_nodo_buscar(abb->raiz, clave, abb->cmp, NULL) != NULL) ? true : false;
}


size_t abb_cantidad(const abb_t *abb) {
    if(!abb) return 0; else return abb->cantidad;
}


void *abb_borrar(abb_t *abb, const char *clave) {

    /*
    VER COMO BORRAR EN ABB
    https://docs.google.com/presentation/d/1Iyq_N7JBe19e4TZRlWe4qC3CdQ_SArpQ/edit#slide=id.p23
    */

    if (!abb || !clave) return NULL;
    
    abb_nodo_t* encontrado_padre = NULL;
    abb_nodo_t* encontrado = abb_nodo_buscar(abb->raiz, clave, abb->cmp, &encontrado_padre);

    // Si el elemento no pertenece al ABB no se puede borrar, obviamente
    if (!encontrado) return NULL;

    abb->cantidad--; ///

    // Hacemos el borrado en el lugar encontrado (si hay)
    return abb_borrar_2(encontrado, encontrado_padre, abb, clave);
}


void abb_destruir(abb_t *abb) {
    
    if (!abb) return;

    //usamos recorrido POST-ORDER
    if (abb->cantidad != 0) {
        abb_destruir_2(abb->raiz, abb->destruir_dato);
    }

    free(abb);
}


/////////////////////////////////////////////////////////////////////////////////////////////

