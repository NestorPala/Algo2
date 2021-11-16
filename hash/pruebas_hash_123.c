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


int main() {
    hash_t* hash = hash_crear(NULL);

    int num[] = {12,32,43};
    int* x[] = {&num[0], &num[1], &num[2]};

    printb(hash_guardar(hash, "pepe", x[0]));
    printb(hash_guardar(hash, "carlos", x[1]));
    printb(hash_guardar(hash, "juana", x[2]));
    printb(hash_guardar(hash, "estefania", NULL));
    printb(hash_guardar(hash, "freddy", NULL));

    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    printf("\n\n");

    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));
    printb(hash_pertenece(hash, "freddy"));
    //printb(hash_pertenece(hash, "tomas"));

    printf("\n\n");


    void* pepe = hash_obtener(hash, "pepe");
    pepe ? printf("\nPEPE: %d", *(int*)pepe) : printf("\nPEPE: NULL");

    void* carlos = hash_obtener(hash, "carlos");
    carlos ? printf("\nCARLOS: %d", *(int*)carlos) : printf("\nCARLOS: NULL");

    void* juana = hash_obtener(hash, "juana");
    juana ? printf("\nJUANA: %d", *(int*)juana) : printf("\nJUANA: NULL");


    printf("\n\n");


    hash_borrar(hash, "pepe");
    printf("\nPEPE BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    hash_borrar(hash, "carlos");
    printf("\nCARLOS BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    hash_borrar(hash, "juana");
    printf("\nJUANA BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    hash_borrar(hash, "estefania");
    printf("\nESTEFANIA BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    hash_borrar(hash, "freddy");
    printf("\nFREDDY BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));


    printf("\n\n");


    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));
    printb(hash_pertenece(hash, "freddy"));


    hash_destruir(hash);

    return 0;
}