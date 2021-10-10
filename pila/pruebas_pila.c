#include "pila.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()


bool auxiliar_prueba_null_2(pila_t* pila) {

    bool prueba_null = false;

    srand((unsigned int)time(0));


    // Insertamos en la pila 520 elementos int, luego un NULL, y luego otros 519 enteros
    /*************************************************/
    int num_a = 0;

    for (size_t i=0; i<520; i++) {
        num_a = rand() % 1001;
        pila_apilar(pila, &num_a);
    }

    pila_apilar(pila, NULL);

    int num_b = 0;

    for (size_t i=0; i<519; i++) {
        num_b = rand() % 1001;
        pila_apilar(pila, &num_b);
    }
    /*************************************************/


    // Desapilamos los elementos y comprobamos que el NULL esté en la pila
    for (size_t i = 0; i < 1040; i++)
    {
        if (pila_desapilar(pila) == NULL) {
            prueba_null = true;
        }
    }


    return prueba_null;
}


bool auxiliar_prueba_null_1(pila_t* pila) {

    bool prueba_null = true;


    // Insertamos en la pila 1040 elementos NULL
    for (size_t i=0; i<1040; i++) {
        pila_apilar(pila, NULL);
    }


    // Desapilamos los elementos y comprobamos todos los NULL estén en la pila
    for (size_t i = 0; i < 1040; i++)
    {
        if (pila_desapilar(pila) != NULL) {
            prueba_null = false;
        }
    }


    return prueba_null;
}


bool prueba_null_valido(bool solo_elementos_null) {

    pila_t* pila = pila_crear();
    bool prueba_null = false;

    if (solo_elementos_null) {
        prueba_null = auxiliar_prueba_null_1(pila);
    } else {
        prueba_null = auxiliar_prueba_null_2(pila);
    }

    pila_destruir(pila);

    return prueba_null;
}


static void prueba_pila_vacia(pila_t* pila) {

    printf("\n");

    //Comprobamos que la pila esté vacía sin ingresar elementos
    print_test("La pila ahora está vacía:  ", pila_esta_vacia(pila));

    //Comprobamos que pila_desapilar(pila) en una pila vacía sea inválido
    print_test("Ya no se puede desapilar más la pila:  ", pila_desapilar(pila) == NULL);

    //Comprobamos que pila_ver_tope(pila) en una pila vacía sea inválido
    print_test("Ya no se puede ver más el tope de la pila:  ", pila_ver_tope(pila) == NULL);
}


bool desapilar(pila_t* pila, size_t cantidad_veces, int** punteros_num) {

    bool desapilar_ok = true;
    int tope_antes_desapilar = 0, tope_despues_desapilar = 0, numero_original = 0;


    // Mostraremos los elementos desapilados
    // (No para números grandes, para no marear al corrector cuando vea la consola)
    if (cantidad_veces < 22) {
        printf("\nLos elementos que se están desapilando son: \n");
    }


    for (int i=0; i<cantidad_veces; i++) {


        /* Esta variable sirve para comprobar que no se acceda a una posición negativa de punteros_num[] 
           a la hora de comparar el número desapilado con el que se encontraba en punteros_num[].
           Por ende, necesito que pueda ser negativa. */

        int aux_comparar = (int)cantidad_veces;


        // Comprobamos el tope ANTES de desapilar
        /******************************************************************/
        tope_antes_desapilar = *(int*)pila_ver_tope(pila);
        numero_original = *punteros_num[aux_comparar - 1 - i];

        if (tope_antes_desapilar != numero_original) {
            desapilar_ok = false;
        }
        /******************************************************************/


        // Comprobamos el tope DESPUÉS de desapilar
        /******************************************************************/
        tope_despues_desapilar = *(int*)pila_desapilar(pila);

        if (aux_comparar-1-i  >=  0) {
            numero_original = *punteros_num[aux_comparar - 1 - i]; ///
        }

        if (tope_despues_desapilar != numero_original) {
            desapilar_ok = false;
        }
        else if (cantidad_veces < 22) {
            printf("%d\t", tope_despues_desapilar);
        }
        /******************************************************************/
    }

    printf("\n");

    return desapilar_ok;
}


bool apilar(pila_t* pila, size_t cantidad_veces, int** punteros_num) {

    bool apilar_ok = true;
    int tope_apilar = 0;

    // Mostraremos los elementos apilados
    // (No para números grandes, para no marear al corrector cuando vea la consola)
    if (cantidad_veces < 22) {
        printf("\nLos elementos que se están apilando son: \n");
    }

    for (size_t i=0; i<cantidad_veces; i++) {

        pila_apilar(pila, punteros_num[i]);

        tope_apilar = *(int*)pila_ver_tope(pila);

        if (tope_apilar != *punteros_num[i]) {
            apilar_ok = false;
        } else if (cantidad_veces < 22) {
            printf("%d\t", tope_apilar);
        }   
    }

    printf("\n");

    return apilar_ok;
}


int** obtener_numeros(size_t cantidad_veces, int* lista_numeros) {
    
    srand((unsigned int)time(0));

    // Asignamos los números aleatorios
    for (size_t i=0; i<cantidad_veces; i++) {
        lista_numeros[i] = rand() % 101;
    }

    // Creamos los punteros que apuntan a cada posición de numeros[]
    int** punteros_num = malloc(cantidad_veces * sizeof(int*));

    // Asignamos los punteros
    for (size_t i=0; i<cantidad_veces; i++) {
        punteros_num[i] = &lista_numeros[i];
    }

    return punteros_num;
}


bool apilar_desapilar(pila_t* pila, size_t cantidad_veces) {

    bool resultado_apilar, resultado_desapilar;

    // Creamos un arreglo de números aleatorios
    int* lista_numeros = malloc(cantidad_veces * sizeof(int));

    // Creamos el arreglo de punteros a los números de arriba
    int** punteros_num = obtener_numeros(cantidad_veces, lista_numeros);

    /* EJECUTAMOS LAS PRUEBAS PRINCIPALES */
    resultado_apilar = apilar(pila, cantidad_veces, punteros_num);
    resultado_desapilar = desapilar(pila, cantidad_veces, punteros_num);

    free(lista_numeros);
    free(punteros_num);
    
    return resultado_apilar && resultado_desapilar;
}


bool prueba_apilar_desapilar(size_t cantidad_veces) {

    bool resultado;
    pila_t* pila = pila_crear();

    printf("\nAPILAR Y DESAPILAR %zu ELEMENTOS DE UNA PILA\n\n\n", cantidad_veces);

    resultado = apilar_desapilar(pila, cantidad_veces);
    prueba_pila_vacia(pila);

    pila_destruir(pila);

    return resultado;
}


void torturar_pila(pila_t* pila, size_t cantidad_veces) {

    int* lista_numeros = malloc(cantidad_veces * sizeof(int));
    int** punteros_num = obtener_numeros(cantidad_veces, lista_numeros);

    printf("\n>>>>>>> Apilar y desapilar (sin vaciar la pila) con %zu elementos:\n", cantidad_veces);

    bool prueba_apilar = apilar(pila, cantidad_veces, punteros_num);
    bool prueba_desapilar = desapilar(pila, cantidad_veces, punteros_num);
    
    print_test("\nRESULTADO:  ", prueba_apilar && prueba_desapilar);

    printf("\n                                         ----------------------- # -----------------------\n");
    
    free(lista_numeros);
    free(punteros_num);
}


void prueba_torturar_pila() {

    pila_t* pila = pila_crear();

    // Primero llenamos la pila con algunos elementos
    int* lista_numeros = malloc(80 * sizeof(int));
    int** punteros_num = obtener_numeros(80, lista_numeros);
    apilar(pila, 80, punteros_num);

    // La prueba
    torturar_pila(pila, 1);
    torturar_pila(pila, 15);
    torturar_pila(pila, 21);
    torturar_pila(pila, 50);
    torturar_pila(pila, 890);
    torturar_pila(pila, 42069);

    free(lista_numeros);
    free(punteros_num);
    pila_destruir(pila);
}


void pruebas_pila_estudiante() {

    printf("\nLA CANTIDAD BASE DE ELEMENTOS DE UNA PILA SE ESTABLECIÓ EN 20\n\n");

    printf("\n-------------------------------------------------------------------------------------\n");


    //Comprobamos que una pila vacía se comporte como tal
    printf("\nPRUEBA EN UNA PILA VACÍA\n");
    pila_t* pila = pila_crear();
    prueba_pila_vacia(pila);
    pila_destruir(pila);


    printf("\n-------------------------------------------------------------------------------------\n");


    //Comprobamos que la pila funcione correctamente ingresando un elemento y luego desapilándolo
    bool prueba1 = prueba_apilar_desapilar(1);
    print_test("..........  Apilar y luego desapilar 1 elemento  .......", prueba1);
    

    printf("\n-------------------------------------------------------------------------------------\n");


    //Comprobamos que la pila funcione correctamente luego de ingresar entre 2 y 20 elementos y luego desapilarlos
    bool prueba2 = prueba_apilar_desapilar(9);
    print_test("..........  Apilar y luego desapilar 9 elementos  .......", prueba2);


    printf("\n-------------------------------------------------------------------------------------\n");


    //Comprobamos que la pila funcione correctamente luego de ingresar más de 20 elementos y luego desapilarlos
    bool prueba3 = prueba_apilar_desapilar(21);
    print_test("..........  Apilar y luego desapilar 21 elementos  .......", prueba3);


    printf("\n-------------------------------------------------------------------------------------\n");


    /*Comprobamos que la pila funcione correctamente luego 
      de ingresar un número ridículamente grande de elementos y luego desapilarlos*/
    bool prueba4 = prueba_apilar_desapilar(60000);
    print_test("..........  Apilar y luego desapilar 60.000 elementos  .......", prueba4);


    printf("\n-------------------------------------------------------------------------------------\n");


    printf("\nAPILAR ELEMENTOS 'NULL' y NO 'NULL' EN LA PILA");

    //Comprobamos que insertar NULL en cualquier posición de la pila sea válido
    bool prueba5 = prueba_null_valido(false);
    print_test("\n\nSe pueden insertar elementos 'null' en la pila:  ", prueba5);

    //Comprobamos que insertar sólo elementos NULL en la pila sea válido
    bool prueba6 = prueba_null_valido(true);
    print_test("\nSe pueden insertar sólo elementos 'null' en la pila:  ", prueba6);


    printf("\n-------------------------------------------------------------------------------------\n");


    // Comprobamos que apilar y desapilar sucesivamente (sin vaciar la pila por completo) funciona
    printf("\nPRUEBA MASIVA DE APILAR Y LUEGO DESAPILAR (SIN VACIAR LA PILA)\n\n");
    prueba_torturar_pila();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
