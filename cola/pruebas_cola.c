#include "cola.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()



// Cantidad masiva de pruebas
const size_t N = 1000;



void desencolar_enteros(cola_t* cola, int** lista_numeros, size_t cant_enteros, int contenido_null) {


    bool muchos_desencolados = true, desencolados_correctos = true;

    for (size_t i=0; i<cant_enteros; i++) {

        if (cola_ver_primero(cola) != NULL) {
        

            if (*(int*)cola_ver_primero(cola) != *lista_numeros[i]) {
                desencolados_correctos = false;
            }

            int aux = *(int*)cola_desencolar(cola);

            if (aux != *lista_numeros[i]) {
                muchos_desencolados = false;
            }

            printf("%d\t", aux);

        }
        else {


            if (lista_numeros[i] != NULL) {
                desencolados_correctos = false;
            }

            if (cola_desencolar(cola) != NULL) {
                muchos_desencolados = false;
            }

            printf("NULL\t");

        }
    }

    print_test("\nSe pudieron desencolar todos los elementos:  ", muchos_desencolados);
    print_test("Cada elemento desencolado era el correcto:  ", desencolados_correctos);
}



void imprimir_entero(int* valor, int contenido_null) {

    if (contenido_null == 0) {
        printf("%d\t", *valor);
    } else if (contenido_null == 1) {
        printf("NULL\t");
    } else if (valor == NULL) {
        printf("NULL\t");
    } else {
        printf("%d\t", *valor);
    }
}



void encolar_enteros(cola_t* cola, int** lista_numeros, size_t cant_enteros, int contenido_null) {

    bool muchos_encolados = true;

    for (size_t i=0; i<cant_enteros; i++) {
        if (!cola_encolar(cola, lista_numeros[i])) {
            muchos_encolados = false;
        }

        imprimir_entero(lista_numeros[i], contenido_null);
    }

    print_test("\nSe pudieron encolar todos los elementos:  ", muchos_encolados);
    printf("\n");
}



// Obtener muchos números para pruebas masivas
int** obtener_numeros(size_t cantidad_num, int* container_numeros, int contenido_null) {
    
    srand((unsigned int)time(0));


    for (size_t i=0; i<cantidad_num; i++) {
        container_numeros[i] = rand() % 101;
    }


    int** lista_numeros = malloc(cantidad_num * sizeof(int*));

    for (size_t i=0; i<cantidad_num; i++) {
        if (contenido_null == 0) {
            lista_numeros[i] = &container_numeros[i];
        }
        else if (contenido_null == 1) {
            lista_numeros[i] = NULL;
        }
        else {
            int aux = rand() % 2;
            lista_numeros[i] = aux ? &container_numeros[i] : NULL;
        }
    }


    return lista_numeros;
}



void encolar_desencolar_enteros(cola_t* cola, size_t cant_enteros, int contenido_null) {

    int* container_numeros = malloc(cant_enteros * sizeof(int));
    int** lista_numeros = obtener_numeros(cant_enteros, container_numeros, contenido_null);

    encolar_enteros(cola, lista_numeros, cant_enteros, contenido_null);
    desencolar_enteros(cola, lista_numeros, cant_enteros, contenido_null);

    free(container_numeros);
    free(lista_numeros);
}



void pruebas_vaciedad_cola(cola_t* cola) {

    print_test("\nLa cola está vacía:  ", cola_esta_vacia(cola));
    print_test("No se puede ver el primer elemento de la cola vacía:  ", cola_ver_primero(cola) == NULL);
    print_test("No se puede desencolar la cola vacía:  ", cola_desencolar(cola) == NULL);
    printf("\n");
}


// contenido_null  -->>  0: no hay NULL; 1: todos son NULL; 2: hay algunos NULL 
void pruebas_encolar_desencolar(size_t cantidad_elementos, int contenido_null) {

    cola_t* cola = cola_crear();

    printf("ENCOLAR Y DESENCOLAR   %zu   ELEMENTOS\n\n", cantidad_elementos);

    pruebas_vaciedad_cola(cola);
    encolar_desencolar_enteros(cola, cantidad_elementos, contenido_null);
    pruebas_vaciedad_cola(cola);

    cola_destruir(cola, NULL);

    printf("\n-----------------------------------------------------------------------------------\n");
}



//todos_null  -->>  0: se encolan solo elementos NULL; 1: se encolan algunos elementos no nulos
void pruebas_encolar_desencolar_null(size_t cantidad_elementos, bool todos_null) {

    if (todos_null) {
        printf("\nPRUEBA DEL TDA INSERTANDO >>> SOLAMENTE <<< ELEMENTOS NULL\n");
    } else {
        printf("\nPRUEBA DEL TDA INSERTANDO >>> ALGUNOS <<< ELEMENTOS NULL\n");
    }

    pruebas_encolar_desencolar(cantidad_elementos, (todos_null) ? 1 : 2);
    printf("\n-----------------------------------------------------------------------------------\n");
}



void pruebas_null(size_t cantidad_elementos) {

    pruebas_encolar_desencolar_null(cantidad_elementos, false);
    pruebas_encolar_desencolar_null(cantidad_elementos, true);
}



void pruebas_cola_estudiante() {

    printf("\n-----------------------------------------------------------------------------------\n\n");
    printf("PRUEBA DE TODAS LAS PRIMITIVAS DEL TDA COLA");
    printf("\n\n-----------------------------------------------------------------------------------\n");

    pruebas_encolar_desencolar(1, 0);
    pruebas_encolar_desencolar(10, 0);
    pruebas_encolar_desencolar(100, 0);
    pruebas_encolar_desencolar(1000, 0);
    pruebas_encolar_desencolar(N, 0);

    pruebas_null(1);
    pruebas_null(10);
    pruebas_null(100);
    pruebas_null(1000);
    pruebas_null(N);
}
    


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {

    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
