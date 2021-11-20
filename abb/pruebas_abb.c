#include "abb.h"
#include "testing.h"
#include <stdio.h> //printf()
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <string.h> //strcmp()
#include <stdbool.h>


/***********************************************************************************/

// Cantidad masiva de pruebas
const size_t N = 10000;

/***********************************************************************************/


void printb(bool a) {
    a ? printf("TRUE\n") : printf("FALSE\n");
}


void obtenemos(abb_t* abb, char* claves[], size_t n) {
    for (size_t i=0; i<n; i++) {
        void* elemento = abb_obtener(abb, claves[i]);
        elemento ? printf("\n%s: %d", claves[i], *(int*)elemento) : printf("\n%s: NULL", claves[i]);
    }
}


void pertenencia(abb_t* abb, char* clave) {
    printf("LA CLAVE '%s' PERTENECE AL abb   >>>  ", clave);
    printb(abb_pertenece(abb, clave));
}


void borramos(abb_t* abb, char* claves[], size_t n) {
    for (size_t i=0; i<n; i++) {
        abb_borrar(abb, claves[i]);
        printf("\n%s BORRADO", claves[i]);
        printf("\nCANTIDAD DE ELEMENTOS: %zu", abb_cantidad(abb));
        pertenencia(abb, claves[i]);
    }   
}


void guardamos(abb_t* abb, char* claves[], int** numeros, size_t n) {
    for (size_t i=0; i<n; i++) {
        printf("\nGUARDANDO: %s\n", claves[i]);
        printb(abb_guardar(abb, claves[i], numeros[i]));
        printf("CANTIDAD DE ELEMENTOS: %zu\n", abb_cantidad(abb));
        pertenencia(abb, claves[i]);
    }
}


// void probamos_iterador(abb_t* abb) {
//     printf("\n\n\nPRUEBAS DEL ITERADOR DE abb\n");

//     abb_iter_t* iter = abb_iter_crear(abb);
//     if (!iter) {printf("NO SE PUDO CREAR EL ITERADOR"); return;}

//     while(!abb_iter_al_final(iter)) {
//         const char* clave_actual = abb_iter_ver_actual(iter);
//         if (clave_actual) printf("\nCLAVE: %s", clave_actual);
//         abb_iter_avanzar(iter);
//     }
//     abb_iter_destruir(iter);
// }


int** crear_arreglo_numeros(int numeros[], size_t n) {
    int** arr = malloc(n * sizeof(int*));
    if (!arr) return NULL;
    for (size_t i=0; i<n; i++) arr[i] = &numeros[i];
    return arr;
}


void pruebas_abb_vacio() {
    abb_t* arbol = abb_crear(strcmp, NULL);
    print_test("El arbol está vacío: ", abb_cantidad(arbol) == 0);
    print_test("No se pueden obtener elementos: ", abb_obtener(arbol, "Pepe") == NULL);
    print_test("Obtener pertenencia en un árbol vacío: ", !abb_pertenece(arbol, "15"));
    print_test("No se puede borrar en un árbol vacío: ", abb_borrar(arbol, "Juan") == NULL);
    abb_destruir(arbol);
}


void pruebas_unitarias() {
    
    abb_t* abb = abb_crear(strcmp, NULL);

    int num[] = {12, 32, 43, -99, 126, 
                45, 65, 177, 277, 144, 
                -2, 345, 7, 65};

    char* claves[] = {
                "pepe", "carl", "soto", "estefania", "freddy", 
                "mariana", "jose", "tomas", "cecilio", "casper",
                "cane", "maro", "tomi", "tara"};

    //size_t cant = sizeof(num) * 2 / sizeof(int*);
    size_t cant = 3;

    int** x = crear_arreglo_numeros(num, cant);

    guardamos(abb, claves, x, cant);
    printf("\n\n");
    obtenemos(abb, claves, cant);
    //probamos_iterador(abb);
    printf("\n\n");
    borramos(abb, claves, cant);
    printf("\n\n");

    abb_destruir(abb);
    free(x);
}


/***********************************************************************************/

void pruebas_abb_estudiante() {
    pruebas_abb_vacio();
    pruebas_unitarias();
}

/***********************************************************************************/
/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {

    pruebas_abb_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
