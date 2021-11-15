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

    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    printf("\n\n");

    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));
    printb(hash_pertenece(hash, "tomas"));

    printf("\n\n");

    printf("\nPEPE: %d",*(int*)hash_obtener(hash, "pepe"));
    printf("\nCARLOS: %d",*(int*)hash_obtener(hash, "carlos"));
    printf("\nJUANA: %d",*(int*)hash_obtener(hash, "juana"));

    printf("\n\n");

    printf("\nPEPE BORRADO: %d", *(int*)hash_borrar(hash, "pepe"));
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));
    printf("\nCARLOS BORRADO: %d", *(int*)hash_borrar(hash, "carlos"));
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));
    printf("\nJUANA BORRADO: %d", *(int*)hash_borrar(hash, "juana"));
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));
    hash_borrar(hash, "estefania");
    printf("\nESTEFANIA BORRADO");
    printf("\nCANTIDAD DE ELEMENTOS: %zu", hash_cantidad(hash));

    printf("\n\n");

    printb(hash_pertenece(hash, "pepe"));
    printb(hash_pertenece(hash, "carlos"));
    printb(hash_pertenece(hash, "juana"));
    printb(hash_pertenece(hash, "estefania"));

    hash_destruir(hash);

    return 0;
}