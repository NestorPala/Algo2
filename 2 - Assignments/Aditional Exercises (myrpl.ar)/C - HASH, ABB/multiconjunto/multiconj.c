#include <stdlib.h>
#include "hash.h"
#include "multiconj.h"


struct multiconj {
    hash_t* hash;
};


multiconj_t* multiconj_crear() {

    multiconj_t* mc = malloc(sizeof(multiconj_t*));
    if (!mc) return NULL;

    hash_t* hash = hash_crear(free);
    if (!hash) {
        free(mc);
        return NULL;
    }

    mc->hash = hash;

    return mc;
}


bool multiconj_guardar(multiconj_t* multiconj, const char* elem) {
    
    multiconj_t* mc = multiconj; //renaming

    if (!elem) return false;

    void* ap;

    if (!hash_pertenece(mc->hash, elem)) {
        ap = malloc(sizeof(int));
        if (!ap) return false;
        *(int*)ap = 1;
        hash_guardar(mc->hash, elem, ap);    
    } else {
        ap = hash_obtener(mc->hash, elem);
        (*(int*)ap)++;
    }

    return true;
}


bool multiconj_pertenece(const multiconj_t* multiconj, const char* elem) {

    return hash_pertenece(multiconj->hash, elem);
}


bool multiconj_borrar(multiconj_t* multiconj, char* elem) {

    multiconj_t* mc = multiconj; //renaming

    if (hash_cantidad(mc->hash) == 0) return false;
    if (!hash_pertenece(mc->hash, elem)) return false;

    void* ap = hash_obtener(mc->hash, elem);
    (*(int*)ap)--;

    if (*(int*)ap == 0) hash_borrar(mc->hash, elem); //No hay elementos con cantidad 0
    
    return true;
}


void multiconj_destruir(multiconj_t* multiconj) {
    
    hash_destruir(multiconj->hash);
    free(multiconj);
}
