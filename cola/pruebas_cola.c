#include "cola.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()



//el resto de funciones



void pruebas_pila_estudiante() {

    cola_t* cola = cola_crear();
    int num1 = 112;

    print_test("La cola está vacía:  ", cola_esta_vacia(cola));

    print_test("Se puede encolar el primer elemento en la cola:  ", cola_encolar(cola, &num1));

    print_test("El primer elemento de la cola es 112:  ", *(int*)cola_ver_primero(cola) == 112);

    print_test("Se pudo desencolar el primer elemento de la cola:  ", *(int*)cola_desencolar(cola) == 112);

    print_test("La cola ahora está vacía:  ", cola_esta_vacia(cola));

    print_test("El elemento '112' no está más en la cola:  ", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL); //LUEGO CAMBIAR LA FUNCION
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
