#include "lista.h"

#include <stdio.h>

//
// Imprimir una lista con iterador externo
//
void imprimir_iter_externo(lista_t *lista) {
    lista_iter_t *iter = lista_iter_crear(lista);
    int num_items = 0;

    while (!lista_iter_al_final(iter)) {
        char *elemento = lista_iter_ver_actual(iter);
        printf("%d. %s\n", ++num_items, elemento);

        lista_iter_avanzar(iter);
    }
    printf("Tengo que comprar %d ítems\n", num_items);
    lista_iter_destruir(iter);
}


//
// Imprimir una lista con iterador interno
//

bool imprimir_un_item(void *elemento, void *extra) {
    // Sabemos que ‘extra’ es un entero, por tanto le podemos hacer un cast.
    int *contador = extra;
    printf("%d. %s\n", ++(*contador), (char *) elemento);

    return true;  // seguir iterando
}

void imprimir_iter_interno(lista_t *lista) {
    int num_items = 0;
    lista_iterar(lista, imprimir_un_item, &num_items);
    printf("Tengo que comprar %d ítems\n", num_items);
}

//
// Ejemplo de uso
//


void ejemplo_iteradores() {
    lista_t *super = lista_crear();

    lista_insertar_ultimo(super, "leche");
    lista_insertar_ultimo(super, "huevos");
    lista_insertar_ultimo(super, "pan");
    lista_insertar_ultimo(super, "mermelada");

    imprimir_iter_externo(super);
    imprimir_iter_interno(super);

    lista_destruir(super, NULL);
}
