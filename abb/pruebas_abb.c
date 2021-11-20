#include "abb.h"
#include "testing.h"
#include <stdio.h> //printf()
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <time.h> //srand()
#include <string.h> //strcmp()


/***********************************************************************************/

// Cantidad masiva de pruebas
const size_t N = 10000;

/***********************************************************************************/


void pruebas_abb_vacio() {

    abb_t* arbol = abb_crear(strcmp, NULL);

    print_test("El arbol está vacío: ", abb_cantidad(arbol) == 0);
    print_test("No se pueden obtener elementos: ", abb_obtener(arbol, "Pepe") == NULL);
    print_test("Obtener pertenencia en un árbol vacío: ", !abb_pertenece(arbol, "15"));
    // print_test("No se puede borrar en un árbol vacío: ", abb_borrar(arbol, "Juan") == NULL);
    //abb_destruir(arbol);

    abb_destruir(arbol);
}


void pruebas_unitarias_insertar_abb() {
    
    abb_t* arbol = abb_crear(strcmp, NULL);

    char* claves[] = {"pepe", "juan", "maria", "diana"};
    int numeros[] = {2,17,-23, 0};

    print_test("ABB guardar:  arbol[clave1] = valor1   ", 
                abb_guardar(arbol, claves[0], &numeros[0]));
    
    print_test("ABB obtener en clave1 es igual a valor1: ", 
                *(int*)abb_obtener(arbol, "pepe") == numeros[0]);

    print_test("ABB guardar:  arbol[clave2] = valor2   ", 
                abb_guardar(arbol, claves[1], &numeros[1]));
    
    print_test("ABB obtener en clave2 es igual a valor2: ", 
                *(int*)abb_obtener(arbol, "juan") == numeros[1]); 

    print_test("ABB guardar:  arbol[clave3] = valor3   ", 
                abb_guardar(arbol, claves[2], &numeros[2]));
    
    print_test("ABB obtener en clave3 es igual a valor3: ", 
                *(int*)abb_obtener(arbol, "maria") == numeros[2]);

    print_test("ABB guardar:  arbol[clave4] = valor4   ", 
                abb_guardar(arbol, claves[3], &numeros[3]));
    
    print_test("ABB obtener en clave4 es igual a valor4: ", 
                *(int*)abb_obtener(arbol, "diana") == numeros[3]); 
    
    abb_destruir(arbol);
}


void pruebas_unitarias_borrar_abb() {

    abb_t* arbol = abb_crear(strcmp, NULL);

    printf("Inserto 5 elementos. Ahora los voy a borrar uno por uno");

    int n[] = {12, 9, 14, 7, 10};
    abb_guardar(arbol, "12", &n[0]);
    abb_guardar(arbol, "09", &n[1]);
    abb_guardar(arbol, "14", &n[2]);
    // abb_guardar(arbol, "07", &n[3]);
    // abb_guardar(arbol, "10", &n[4]);

    
    print_test("La clave '12' pertenece al árbol:  ", abb_pertenece(arbol, "12"));
    int a = *(int*)abb_borrar(arbol, "12");
    print_test("Se pudo borrar el valor en la clave '12': ", a == n[0] );
    print_test("La clave '12' ya no pertenece al árbol:  ", !abb_pertenece(arbol, "12"));
    

    print_test("La clave '09' pertenece al árbol:  ", abb_pertenece(arbol, "09"));
    int b = *(int*)abb_borrar(arbol, "09");
    print_test("Se pudo borrar el valor en la clave '09': ", b == n[1] );
    print_test("La clave '09' ya no pertenece al árbol:  ", !abb_pertenece(arbol, "09"));


    print_test("La clave '14' pertenece al árbol:  ", abb_pertenece(arbol, "14"));
    int c = *(int*)abb_borrar(arbol, "14");
    print_test("Se pudo borrar el valor en la clave '14': ", c == n[2] );
    print_test("La clave '14' ya no pertenece al árbol:  ", !abb_pertenece(arbol, "14"));


    // print_test("Se pudo borrar el valor en la clave '07': ", *(int*)abb_borrar(arbol, "07") == n[3] );
    // print_test("Se pudo borrar el valor en la clave '10': ", *(int*)abb_borrar(arbol, "10") == n[4] );

    abb_destruir(arbol);
}


/***********************************************************************************/

void pruebas_abb_estudiante() {
    pruebas_abb_vacio();
    //pruebas_unitarias_insertar_abb();  ///
    pruebas_unitarias_borrar_abb();


    //pruebas_volumen_insertar_abb();
    //pruebas_volumen_borrar_abb();
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
