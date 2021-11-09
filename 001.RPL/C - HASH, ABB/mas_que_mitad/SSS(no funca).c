#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool mas_de_mitad(void* arreglo[], size_t n) {
    
    hash_t* hash = hash_crear(NULL);

    for (size_t i=0; i<n; i++) {
        char clave[999];
        sprintf(clave, "%d", *(int*)arreglo[i]); 

        if(n == 1) {
            hash_destruir(hash);
            return true;
        }

        if(!hash_pertenece(hash, clave)) {
            int a = 1;
            hash_guardar(hash, clave, &a);
        } else {
            int b = *(int*)hash_obtener(hash, clave);
            b++;
            hash_guardar(hash, clave, &b);

            if ( b > n/2 ) {
                hash_destruir(hash);
                return true;
            }
        }
    }

    hash_destruir(hash);
    return false;
}


int main() {

    int a[] = {1,2,1,2,3};
    void* numeros[] = {&a[0], &a[1], &a[2], &a[3], &a[4], &a[5]};

    if (mas_de_mitad(numeros, 5)) {
        printf("\nMÁS de la mitad de los elementos del arreglo son el mismo número\n");
    } else {
        printf("\nMENOS O LA MISMA CANTIDAD de la mitad de los elementos del arreglo son el mismo número\n");
    }

    return 0;
}


