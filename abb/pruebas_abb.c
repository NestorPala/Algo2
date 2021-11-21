#include "abb.h"
#include "testing.h"
#include <stdio.h> //printf()
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <string.h> //strcmp()
#include <stdbool.h>
#include "generador.h" //archivo propio


/***********************************************************************************/

// Cantidad masiva de pruebas
const size_t N = 2000;

/***********************************************************************************/


bool obtenemos(abb_t* abb, char** claves, int** valores, size_t n) {
    for (size_t i=0; i<n; i++) {
        if (*(int*)abb_obtener(abb, claves[i]) != *valores[i]) {
            return false;
        }
    }
    return true;
}


bool pertenencia(abb_t* abb, char** claves, size_t n) {
    for (size_t i=0; i<n; i++) {
        if (!abb_pertenece(abb, claves[i])) return false;
    }
    return true;
}


bool borramos(abb_t* abb, char** claves, int** valores, size_t n) {
    for (size_t i=0; i<n; i++) {
        if (*(int*)abb_borrar(abb, claves[i]) != *valores[i]) {
            return false;
        }
    }
    return true;
}


bool guardamos(abb_t* abb, char** claves, int** valores, size_t n) {
    for (size_t i=0; i<n; i++) {
        if (!abb_guardar(abb, claves[i], valores[i])) return false;
    }
    return true;
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


void pruebas_abb(abb_t* abb, char** claves, int** valores, size_t cant) {
    bool todos_guardados = guardamos(abb, claves, valores, cant);
    print_test("Se pudieron guardar todos los elementos en el árbol: ", todos_guardados);

    bool todos_pertenecen = pertenencia(abb, claves, cant);
    print_test("Todos los elementos guardados pertenecen al árbol: ", todos_pertenecen);

    bool todos_obtenidos = obtenemos(abb, claves, valores, cant);
    print_test("Todos los elementos del árbol se pueden ver: ", todos_obtenidos);

    //probamos_iterador(abb);

    bool todos_borrados = borramos(abb, claves, valores, cant);
    print_test("SE BORRARON TODOS LOS ELEMENTOS DEL ABB  >>>  ", todos_borrados);
}


void pruebas_volumen() {
    printf("\n------------------------------------------------ PRUEBAS DE VOLUMEN ------------------------------------------------\n");  

    abb_t* abb = abb_crear(strcmp, NULL);
    size_t cant = N;

    char** claves = arreglo_cadenas_crear(cant);
    int** valores = arreglo_numeros_crear(cant);

    pruebas_abb(abb, claves, valores, cant);

    abb_destruir(abb);

    arreglo_numeros_destruir(valores, cant);
    arreglo_cadenas_destruir(claves, cant);
}


void pruebas_unitarias() {
    printf("\n------------------------------------------------ PRUEBAS UNITARIAS ------------------------------------------------\n");  

    abb_t* abb = abb_crear(strcmp, NULL);

    // ARBOL "INCOMPLETO" 
    char* claves[] = {"pepe", "carl", "soto", "este", "fred", 
                      "mari", "jose", "toma", "ceci", "casp",
                      "cane", "maro", "tomi", "tara", "sisi"};
    // ARBOL "LISTA"
    char* claves2[] = {"aa", "ab", "ac", "ad", "ae", "ba", "bb", "bc", "bd", "be", "ca", "cb", "cc", "cd", "ce"};

    // ARBOL "ZIGZAG" 
    char* claves3[] = { "0001", "1000",
                        "0002", "0999",
                        "0003", "0998",
                        "0004", "0997",
                        "0005", "0996",
                        "0006", "0995",
                        "0007", "0994",
                        "0008",
    };
    
    // ARBOL "V INVERTIDA" 
    char* claves4[] = {                             "500",
                                                "499",  "501",
                                            "498",          "502",
                                        "497",                   "503",
                                    "496",                          "504",
                                "495",                                  "505",
                            "494",                                          "506",
                        "493",                                                  "507",
    };
    
    // ARBOL COMPLETO 
    char* claves5[] = {
                                                    "5000",

                                    "2500",                         "7500",

                            "1250",         "3750",         "6250",         "8750",

                        "0625", "1875", "3125", "4375", "5625", "6875", "8125", "9375",
    };

    int val[] = {12, 32, 43, -99, 126, 
                45, 65, 177, 277, 144, 
                -2, 345, 7, 65, -3231};

    size_t cant = 15;

    int** valores = malloc(cant * sizeof(int*));
    for (size_t i=0; i<cant; i++) valores[i] = &val[i];


    printf("\n\nPRUEBA ARBOL INCOMPLETO\n");
    pruebas_abb(abb, claves, valores, cant);

    printf("\n\nPRUEBA ARBOL TIPO LISTA\n");
    pruebas_abb(abb, claves2, valores, cant);

    printf("\n\nPRUEBA ARBOL TIPO ZIGZAG\n");
    pruebas_abb(abb, claves3, valores, cant);

    printf("\n\nPRUEBA ARBOL TIPO V INVERTIDA\n");
    pruebas_abb(abb, claves4, valores, cant);

    printf("\n\nPRUEBA ARBOL COMPLETO\n");
    pruebas_abb(abb, claves5, valores, cant);


    abb_destruir(abb);
    free(valores);
}


void pruebas_abb_vacio() {
    printf("\n------------------------------------------------ PRUEBAS ABB VACIO  ------------------------------------------------\n"); 

    abb_t* arbol = abb_crear(strcmp, NULL);

    print_test("El arbol está vacío: ", abb_cantidad(arbol) == 0);
    print_test("No se pueden obtener elementos: ", abb_obtener(arbol, "Pepe") == NULL);
    print_test("Obtener pertenencia en un árbol vacío: ", !abb_pertenece(arbol, "15"));
    print_test("No se puede borrar en un árbol vacío: ", abb_borrar(arbol, "Juan") == NULL);

    abb_destruir(arbol);
}


/***********************************************************************************/


void pruebas_abb_estudiante() {
    pruebas_abb_vacio();
    pruebas_unitarias();
    pruebas_volumen();
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
