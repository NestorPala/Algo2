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


// Funcion para probar el iterador interno
bool sumar(const char* clave, void* dato, void* total) {
    
    if (dato) {
        *(int*)total += *(int*)dato;
    }

    return true;
}


void pruebas_abb(abb_t* abb, char** claves, int** valores, size_t cant) {
    bool todos_guardados = guardamos(abb, claves, valores, cant);
    print_test("Se pudieron guardar todos los elementos en el árbol: ", todos_guardados);

    bool todos_pertenecen = pertenencia(abb, claves, cant);
    print_test("Todos los elementos guardados pertenecen al árbol: ", todos_pertenecen);

    bool todos_obtenidos = obtenemos(abb, claves, valores, cant);
    print_test("Todos los elementos del árbol se pueden ver: ", todos_obtenidos);

    bool todos_borrados = borramos(abb, claves, valores, cant);
    print_test("SE BORRARON TODOS LOS ELEMENTOS DEL ABB  >>>  ", todos_borrados);
}


void pruebas_iterador_externo_abb_vacio() {
    printf("\n-------------------------------------- PRUEBAS ITERADOR EXTERNO CON ABB VACIO --------------------------------------\n"); 
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);

    print_test("Se creó con éxito el iterador: ", iter);
    print_test("El iterador esta al final ", abb_iter_in_al_final(iter));
    print_test("No se puede avanzar un iterador de un ABB vacío: ", !abb_iter_in_avanzar(iter));

    abb_guardar(abb, "01", NULL);
    abb_guardar(abb, "00", NULL);
    abb_guardar(abb, "10", NULL);
    abb_guardar(abb, "09", NULL);
    abb_guardar(abb, "11", NULL);

    while(!abb_iter_in_al_final(iter)) {
        abb_iter_in_avanzar(iter);
    }

    print_test("El iterador está al final: ", abb_iter_in_al_final(iter));
    print_test("No se puede avanzar un iterador de un ABB vaciado: ", !abb_iter_in_avanzar(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


void pruebas_iterador_externo() {
    printf("\n------------------------------------------ PRUEBAS ITERADOR EXTERNO DEL ABB ------------------------------------------\n"); 
    abb_t* abb = abb_crear(strcmp, NULL);

    char* claves[] = {"06", "01", "15", "04", "10", "16", "08", "13", "11", "14"};
    int valores[] =  {  6,    1,   15,    4,   10,   16,    8,   13,   11,   14 };

    for (size_t i=0; i<10; i++) {
        abb_guardar(abb, claves[i], &valores[i]);
    }

    abb_iter_t* iter = abb_iter_in_crear(abb);
    size_t visitados = 0;
    bool iter_todos = true, iter_orden = true;

    const char* aux = abb_iter_in_ver_actual(iter);
    print_test("El iterador externo comienza apuntado al elemento mas a la izquierda de la raiz: ", strcmp(aux, "01") == 0);

    while(!abb_iter_in_al_final(iter)) {
        const char* clave_actual = abb_iter_in_ver_actual(iter);
        abb_iter_in_avanzar(iter);
        visitados++;
        const char* clave_siguiente = abb_iter_in_ver_actual(iter);

        if (!abb_iter_in_al_final(iter)) {
            if (strcmp(clave_siguiente, clave_actual) < 0) {
                iter_orden = false;
            }
        }
    }

    if (visitados != 10) {
        iter_todos = false;
    }

    print_test("El iterador externo recorre todos los elementos: ", iter_todos);
    print_test("Todos los elementos estan en orden (el recorrido es efectivamente INORDER): ", iter_orden);

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


void pruebas_iterador_interno() {
    printf("\n------------------------------------------ PRUEBAS ITERADOR INTERNO DEL ABB ------------------------------------------\n"); 

    abb_t* abb = abb_crear(strcmp, NULL);

    char** claves = arreglo_cadenas_crear(100);
    int** valores = arreglo_numeros_crear(100);
    int resultado_inorder = 0, resultado_real = 0;

    for (size_t i=0; i<100; i++) {
        resultado_real += *valores[i];
    }

    for (size_t i=0; i<100; i++) {
        abb_guardar(abb, claves[i], valores[i]);
    }

    abb_in_order(abb, sumar, &resultado_inorder);
    print_test("El iterador interno funciona correctamente: ", resultado_inorder == resultado_real);

    arreglo_cadenas_destruir(claves, 100);
    arreglo_numeros_destruir(valores, 100);
    abb_destruir(abb);
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
                      "cane", "maro", "tomi", "tara", ""};
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
    pruebas_iterador_interno();
    pruebas_iterador_externo();
    pruebas_iterador_externo_abb_vacio();
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
