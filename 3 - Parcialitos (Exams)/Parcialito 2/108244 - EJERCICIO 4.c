#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "lista.h"


/*
    Complejidad de la función implementada: O(N), siendo N la cantidad de elementos de la lista que nos pasaron por parámetro.

    ¿Por qué es O(N)? Porque "Alan" fue tan malvado que no quiso implementar una primitiva "lista_borrar_ultimo(lista)", así que nos cae la responsabilidad a nosotros, "Bárbara", de recorrer toda la lista, hasta el final, pasando por los N elementos, con un iterador externo (porque con el interno no se deben borrar elementos), hasta que la comparación lista_iter_ver_actual(iter) == lista_ver_ultimo(lista) se cumpla, y entonces ahí recién podemos borrar el último elemento de la lista, y devolverlo.

    TL;DR: La complejidad es O(N) porque tenemos que recorrer los N elementos de la lista hasta llegar al último y borrarlo, luego devolverlo.
*/


void* borrar_ultimo_de_lista(lista_t* lista) {

    if (lista_esta_vacia(lista)) {
        return NULL;
    }

    lista_iter_t* iter = lista_iter_crear(lista);

    if (!iter) {
        return NULL;
    }

    while (!lista_iter_al_final(iter)) {
        if (lista_iter_ver_actual(iter) == lista_ver_ultimo(lista)) {
            void* elemento_borrado = lista_iter_borrar(iter);
            lista_iter_destruir(iter);
            return elemento_borrado;
        }

        lista_iter_avanzar(iter);
    }

    if (iter) {
        lista_iter_destruir(iter);
    }

    return NULL;
}


// int main() {

//     lista_t* lista = lista_crear();

//     int num[] = {1, 2, 3, 868};
//     int* num2[] = {&num[0], &num[1], &num[2], &num[3]};

//     lista_insertar_ultimo(lista, num2[0]);
//     lista_insertar_ultimo(lista, num2[1]);
//     lista_insertar_ultimo(lista, num2[2]);
//     lista_insertar_ultimo(lista, num2[3]);

//     void* ultimo = borrar_ultimo_de_lista(lista);

//     printf("EL ULTIMO ELEMENTO DE LA LISTA ERA: %d\n", *(int*)ultimo);

//     return 0;
// }
