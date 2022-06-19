#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "multiconj.h"

int main() {

    multiconj_t* mc = multiconj_crear();

    multiconj_guardar(mc, "1");
    multiconj_pertenece(mc, "1") ? printf(" '1' PERTENECE\n") : printf(" '1' NO PERTENECE\n"); 

    multiconj_guardar(mc, "1");
    multiconj_pertenece(mc, "1") ? printf(" '1' PERTENECE\n") : printf(" '1' NO PERTENECE\n"); 

    multiconj_destruir(mc);

    return 0;
}
