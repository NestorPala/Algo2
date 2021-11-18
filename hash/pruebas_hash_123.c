#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "lista.h"


void printb(bool a) {
    a ? printf("TRUE\n") : printf("FALSE\n");
}


void esta_o_no(hash_t* hash, const char* clave) {
    void* elemento = hash_obtener(hash, clave);
    elemento ? printf("\n%s: %d", clave, *(int*)elemento) : printf("\n%s: NULL", clave);
}


void borramos(hash_t* hash, const char* clave) {
    hash_borrar(hash, clave);
    printf("\n%s BORRADO", clave);
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));
}


int main() {
    hash_t* hash = hash_crear(NULL);

    int num[] = {12, 32, 43, -99, 126};
    int* x[] = {&num[0], &num[1], &num[2], &num[3], &num[4]};

    printb(hash_guardar(hash, "pepe", x[0]));
    printb(hash_guardar(hash, "carlos", x[1]));
    printb(hash_guardar(hash, "juana", x[2]));
    printb(hash_guardar(hash, "estefania", x[3]));
    printb(hash_guardar(hash, "freddy", x[4]));

    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    printf("\n\n");

    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));
    printb(hash_pertenece(hash, "freddy"));
    //printb(hash_pertenece(hash, "tomas"));

    printf("\n\n");

    esta_o_no(hash, "pepe");
    esta_o_no(hash, "carlos");
    esta_o_no(hash, "juana");
    esta_o_no(hash, "estefania");
    esta_o_no(hash, "freddy");


    printf("\n\n\nPRUEBAS DEL ITERADOR DE HASH\n\n\n");

    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter) {printf("NO SE PUDO CREAR EL ITERADOR"); return -1;}

    while(!hash_iter_al_final(iter)) {
        const char* clave_actual = hash_iter_ver_actual(iter);
        if (clave_actual) printf("\nCLAVE: %s", clave_actual);
        hash_iter_avanzar(iter);
    }


    printf("\n\n");

    borramos(hash, "pepe");
    borramos(hash, "carlos");
    borramos(hash, "juana");
    borramos(hash, "estefania");
    borramos(hash, "freddy");

    printf("\n\n");

    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));
    printb(hash_pertenece(hash, "freddy"));

    hash_iter_destruir(iter);
    hash_destruir(hash);

    return 0;
}