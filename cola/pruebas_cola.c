#include "cola.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()



// Cantidad masiva de pruebas
const size_t N = 100000;

// Cantidad máxima de elementos a mostrar
const size_t MOSTRAR_MAX = 100;



/*
HACER PRUEBAS SIMILARES A LAS DE PILA

ADEMÁS:

demás de las pruebas obligatorias análogas a las pedidas para la pila, también tienen que incluir unas pruebas para probar la destrucción con y sin funciones de destrucción de los elementos.
*/



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

            if (cant_enteros <= MOSTRAR_MAX) {
                printf("%d\t", aux);
            }

        }
        else {


            if (lista_numeros[i] != NULL) {
                desencolados_correctos = false;
            }

            if (cola_desencolar(cola) != NULL) {
                muchos_desencolados = false;
            }

            if (cant_enteros <= MOSTRAR_MAX) {
                printf("NULL\t");
            }


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

        if (cant_enteros <= MOSTRAR_MAX) {
            imprimir_entero(lista_numeros[i], contenido_null);
        }
    }

    print_test("\nSe pudieron encolar todos los elementos:  ", muchos_encolados);
    printf("\n");
}



// Obtener muchos números para pruebas masivas
// contenido_null  -->>  0: no hay NULL; 1: todos son NULL; 2: hay algunos NULL 
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



void prueba_vaciedad_cola(cola_t* cola) {

    print_test("\nLa cola está vacía:  ", cola_esta_vacia(cola));
    print_test("No se puede ver el primer elemento de la cola vacía:  ", cola_ver_primero(cola) == NULL);
    print_test("No se puede desencolar la cola vacía:  ", cola_desencolar(cola) == NULL);
    printf("\n");
}



void prueba_encolar_desencolar(size_t cantidad_elementos, int contenido_null) {

    cola_t* cola = cola_crear();

    printf("ENCOLAR Y DESENCOLAR   %zu   ELEMENTOS\n\n", cantidad_elementos);

    prueba_vaciedad_cola(cola);
    encolar_desencolar_enteros(cola, cantidad_elementos, contenido_null);
    prueba_vaciedad_cola(cola);

    cola_destruir(cola, NULL);

    printf("\n-----------------------------------------------------------------------------------\n");
}



//todos_null -->> 0: se encolan solo elementos NULL; 1: se encolan algunos elementos no nulos
void prueba_encolar_desencolar_null(size_t cantidad_elementos, bool todos_null) {

    if (todos_null) {
        printf("\nPRUEBA DEL TDA INSERTANDO >>> SOLAMENTE <<< ELEMENTOS NULL\n");
    } else {
        printf("\nPRUEBA DEL TDA INSERTANDO >>> ALGUNOS <<< ELEMENTOS NULL\n");
    }

    prueba_encolar_desencolar(cantidad_elementos, (todos_null) ? 1 : 2);
    printf("\n-----------------------------------------------------------------------------------\n");
}



void prueba_null(size_t cantidad_elementos) {

    prueba_encolar_desencolar_null(cantidad_elementos, false);
    prueba_encolar_desencolar_null(cantidad_elementos, true);
}



void pruebas_pila_estudiante() {

    printf("\n-----------------------------------------------------------------------------------\n\n");
    printf("PRUEBA DE TODAS LAS PRIMITIVAS DEL TDA COLA");
    printf("\n\n-----------------------------------------------------------------------------------\n");

    prueba_encolar_desencolar(1, 0);
    prueba_encolar_desencolar(10, 0);
    prueba_encolar_desencolar(MOSTRAR_MAX, 0);
    prueba_encolar_desencolar(1000, 0);
    prueba_encolar_desencolar(N, 0);

    prueba_null(1);
    prueba_null(10);
    prueba_null(MOSTRAR_MAX);
    prueba_null(1000);
    prueba_null(N);
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
