#include "cola.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()



// Cantidad masiva de pruebas
const size_t N = 1000;



/*
HACER PRUEBAS SIMILARES A LAS DE PILA


Las pruebas deberán verificar que:

Se pueda crear y destruir correctamente la estructura.
Se puedan apilar elementos, que al desapilarlos se mantenga el invariante de pila.

Prueba de volumen: Se pueden apilar muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la pila hasta un valor sabido mucho mayor que el tamaño inicial, y desapilar elementos hasta que esté vacía, comprobando que siempre cumpla el invariante. Recordar no apilar siempre el mismo puntero, validar que se cumpla siempre que el tope de la pila sea el correcto paso a paso, y que el nuevo tope después de cada desapilar también sea el correcto.

El apilamiento del elemento NULL es válido.
Condición de borde: comprobar que al desapilar hasta que está vacía hace que la pila se comporte como recién creada.
Condición de borde: las acciones de desapilar y ver_tope en una pila recién creada son inválidas.
Condición de borde: la acción de esta_vacía en una pila recién creada es verdadero.
Condición de borde: las acciones de desapilar y ver_tope en una pila a la que se le apiló y desapiló hasta estar vacía son inválidas.


ADEMÁS:

demás de las pruebas obligatorias análogas a las pedidas para la pila, también tienen que incluir unas pruebas para probar la destrucción con y sin funciones de destrucción de los elementos.
*/



// Obtener muchos números para pruebas masivas
int** obtener_numeros(size_t cantidad_num) {
    
    srand((unsigned int)time(0));

    int* lista_numeros = malloc(cantidad_num * sizeof(int));

    // Asignamos los números aleatorios
    for (size_t i=0; i<cantidad_num; i++) {
        lista_numeros[i] = rand() % 101;
    }

    // Creamos los punteros que apuntan a cada posición de numeros[]
    int** punteros_num = malloc(cantidad_num * sizeof(int*));

    // Asignamos los punteros
    for (size_t i=0; i<cantidad_num; i++) {
        punteros_num[i] = &lista_numeros[i];
    }

    return punteros_num;
}



void prueba_cola_1_int() {

    cola_t* cola = cola_crear();
    int num1 = 112;

    print_test("La cola está vacía:  ", cola_esta_vacia(cola));
    print_test("Se puede encolar el primer elemento en la cola:  ", cola_encolar(cola, &num1));
    print_test("La cola ahora NO está vacía:  ", !cola_esta_vacia(cola));
    print_test("El primer elemento de la cola es 112:  ", *(int*)cola_ver_primero(cola) == 112);
    print_test("Se pudo desencolar el primer elemento de la cola:  ", *(int*)cola_desencolar(cola) == 112);
    print_test("La cola ahora está vacía:  ", cola_esta_vacia(cola));
    print_test("El elemento '112' no está más en la cola:  ", cola_ver_primero(cola) == NULL);
    
    cola_destruir(cola, NULL);
}



void prueba_cola_muchos_int() {


    cola_t* cola = cola_crear();
    int** punteros_num = obtener_numeros(N);


    print_test("La cola está vacía:  ", cola_esta_vacia(cola));
    print_test("No se puede ver el primer elemento de la cola vacía:  ", cola_ver_primero(cola) == NULL);
    print_test("No se puede desencolar la cola vacía:  ", cola_desencolar(cola) == NULL);


    /******************************************************************************/
    bool muchos_encolados = true;

    for (size_t i=0; i<N; i++) {
        if (!cola_encolar(cola, punteros_num[i])) {
            muchos_encolados = false;
        }
        
        printf("%d\t", *punteros_num[i]);
    }

    print_test("\nSe pudieron encolar muchos elementos", muchos_encolados);
    printf("\n\n");
    /******************************************************************************/


    /******************************************************************************/
    bool muchos_desencolados = true;

    for (size_t i=0; i<N; i++) {
        int aux = *(int*)cola_desencolar(cola);

        if (aux != *punteros_num[i]) {
            muchos_desencolados = false;
        }

        printf("%d\t", aux);
    }

    print_test("\nSe pudieron desencolar todos los elementos:  ", muchos_desencolados);

    print_test("La cola ahora está vacía:  ", cola_esta_vacia(cola));

    print_test("No se puede ver el primer elemento de la cola vacía:  ", cola_ver_primero(cola) == NULL);

    print_test("No se puede desencolar la cola vacía:  ", cola_desencolar(cola) == NULL);
    
    /******************************************************************************/


    cola_destruir(cola, NULL);
    free(punteros_num[0]);
    free(punteros_num);
}



void pruebas_pila_estudiante() {

    printf("\n-----------------------------------------------------------------------------------\n");
    printf("\nPRUEBA BÁSICA DE TODAS LAS FUNCIONES CON 1 SOLO ENTERO\n\n");
    prueba_cola_1_int();
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("\nPRUEBA DE TODAS LAS FUNCIONES CON MUCHOS ENTEROS\n\n");
    prueba_cola_muchos_int();
    printf("\n-----------------------------------------------------------------------------------\n");
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
