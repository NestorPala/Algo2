#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"


/*
    Complejidad de la solución implementada: O(N + K), donde N es el largo del abecedario, y K es el largo de la palabra.

    Para insertar los caracteres del abecedario en un hash tenemos que realizar N inserciones, las cuales C/U cuesta O(1), por las propiedades del hash (búsqueda, pertenece, inserción y borrado de un elemento siempre son en tiempo constante).

    Después, para comprobar si la palabra se puede formar con las letras de ese abecedario tenenmos que realizar K comparaciones, una por cada letra de la palabra, preguntando si pertenece al hash (la operación hash_pertenece es O(1), como dijimos antes.)

    Luego:
    N * O(1) + K * O(1)  =  O(N + K) 

    >>>   Si N >> K, la complejidad se vuelve O(N), porque despreciamos el valor de K.
    >>>   Si N << K, la complejidad se vuelve O(K), porque despreciamos el valor de N.
    >>>   Si N == K, la complejidad queda O(2 * N), pero como descartamos la constante 2, las complejidad resulta O(N).
*/


void insertar_letras(hash_t* hash, const char* str) {
    for (size_t i=0; i<strlen(str); i++) {
        if (!hash_pertenece(hash, str[i])) {
            hash_guardar(hash, str[i], NULL);
        }
    }
}


bool se_puede_formar(const char* palabra, const char* abecedario) {

    hash_t* hash_abecedario = hash_crear(NULL);
    if (!hash_abecedario) return false;
    insertar_letras(hash_abecedario, abecedario);

    for (size_t i=0; i<strlen(palabra); i++) {
        if (!hash_pertenece(hash_abecedario, palabra[i])) {
            return false;
        }
    }
    
    free(hash_abecedario);
    return true;
}


// int main() {

//     if (se_puede_formar("filo", "aeiflko")) {
//         printf("1:  SE PUEDE FORMAR LA PALABRA CON ESE ABECEDARIO");
//     } else {
//         printf("1:  NO SE PUEDE FORMAR LA PALABRA CON ESE ABECEDARIO");
//     }

//     if (se_puede_formar("filo", "aeiflko")) {
//         printf("\n2:  SE PUEDE FORMAR LA PALABRA CON ESE ABECEDARIO");
//     } else {
//         printf("\n2:  NO SE PUEDE FORMAR LA PALABRA CON ESE ABECEDARIO");
//     }

//     return 0;
// }
